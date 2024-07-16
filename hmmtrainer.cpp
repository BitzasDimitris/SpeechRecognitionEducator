#include "hmmtrainer.h"

HMMTrainer::HMMTrainer(QWidget *parent)
{

}

HMMTrainer::~HMMTrainer(){

}


void HMMTrainer::Initialize(MFCCHolder* mh,PhonemesLibrary* pl,int fsize,int fstep,int SR,double pc,int maxvec,double Crosspc,bool silence,OpenCLInterface * opc,bool cl){
    mfccholder=mh;
    phnlib=pl;

    framesize=SR/1000.0*fsize;
    framestep=SR/1000.0*fstep;
    SampleRate=SR;
    opencl=cl;
    opclint=opc;
    canceled=false;
    paused=false;
    Observations.clear();
    percent=pc;
    MaxVecs=maxvec;
    Crosspercent=Crosspc;
    Modelsilence=silence;
    ConfusionMatrix.clear();

}

void HMMTrainer::StartProcess(){
    start();
}

void HMMTrainer::CancelProcess(){
    mutex.lock();
    canceled=true;
    mutex.unlock();
}

void HMMTrainer::PauseProcess(){
    mutex.lock();
    paused=true;
    mutex.unlock();
}

void HMMTrainer::ResumeProcess(){
    mutex.lock();
    paused=false;
    mutex.unlock();
    Paused.wakeAll();
}

//THREAD
void HMMTrainer::run(){
    QTime totaltime;
    totaltime.start();
    int totaltiming=0;
    int sizemfcc=mfccholder->GetSizeOfMFCCs();
    emit ConsoleLog("MFCC step had");
    emit TimingReport(-1);
    emit ConsoleLog("Quallity of HMM "+QString::number(percent*100)+"%");

    if(MaxVecs!=0){
        emit ConsoleLog("Max Samples per Phoneme are "+QString::number(MaxVecs));
    }
    else{
        emit ConsoleLog("All Samples are used for each Phoneme");
    }
    if(Crosspercent>0){
        emit ConsoleLog("Cross Validation of "+QString::number(Crosspercent*100)+"%");
        emit ConsoleLog("Using "+QString::number((int)(sizemfcc*(1-Crosspercent)))+" files for Training and Evaluating "+QString::number((int)(sizemfcc*Crosspercent))+" files.");
    }
    else{
        emit ConsoleLog("Evaluation of complete Training Set");
    }
    emit ConsoleLog(QString::number(phnlib->PhonemesCount())+" Phonemes in HMM.");
    emit Progress(0);
    emit Progress2(0);
    //Fill Matrices
    FillMatrices();

    emit ConsoleLog("Initialization done.");


    //start recursions
    std::vector<std::vector<std::vector<double>>> ObVecPS(States);
    std::vector<int> statesnum(States,0);
    std::vector<int > StateAssignments;
    std::vector<std::vector<double >> MFCC;
    std::vector<int* > PHN;
    emit ConsoleLog(QString::number(sizemfcc)+" MFCCS");
    for(int i=0;i<sizemfcc*(1-Crosspercent);i++){
        MFCC.clear();
        PHN.clear();
        mfccholder->GetItem(i,&MFCC,&PHN);
        StateAssignments.clear();
        StateAssignments= std::vector<int>(MFCC.size());
        StateAssignments=InitialAssignment(MFCC,PHN,StateAssignments);


        for(int j=0;j<MFCC.size();j++){
            if(statesnum[StateAssignments[j]]<MaxVecs||MaxVecs==0){
                ObVecPS[StateAssignments[j]].push_back(MFCC[j]);
                statesnum[StateAssignments[j]]++;
            }
        }

    }
    float succrate=0;
    int lastnumofg=0;
    bool converged=false;
    int minimumruns=0;
    int maxrecursions=50;
    int timing=0;
    while (succrate<98&&!converged&&maxrecursions>minimumruns){
        minimumruns++;
        emit Progress2((int)((float)(minimumruns)/maxrecursions*100));
        QTime time;
        time.start();
        timing=0;
        int gaussians=0;
        for(int i=0;i<States;i++){
            QString phonemename=phnlib->GetPhoneme(i)->toString();
            if(phonemename!="sil"||Modelsilence){
                Observations[i].Gmeans(ObVecPS[i]);
                gaussians+=Observations[i].numofG;
                //emit ConsoleLog("State"+QString::number(i)+" , "+phnlib->GetPhoneme(i)->toString()+"  has "+QString::number(statesnum[i])+" with "+QString::number(Observations[i].numofG)+" gaussians.");
            }
            else{
                avgenergy=ObVecPS[i][0][0];
                for(int j=1;j<ObVecPS[i].size();j++){
                    double temp=ObVecPS[i][j][0];
                    avgenergy+=temp;
                }
                avgenergy/=ObVecPS[i].size();
                //emit ConsoleLog("State"+QString::number(i)+" , "+phnlib->GetPhoneme(i)->toString()+"  has "+QString::number(avgenergy)+" average energy .");
            }
        }
        avgenergy*=0.8;
        emit ConsoleLog("Training done in "+QString::number(time.elapsed())+" ms. # of Gaussians "+QString::number(gaussians)+" ,iteration: "+QString::number(minimumruns));
        time.restart();
        float total=0;
        float right=0;
        double min=100;
        double max=0;
        int starting=0;
        if(Crosspercent>0){
            starting=sizemfcc*(1-Crosspercent);
        }
        AVGtime=0;
        for(int i=starting;i<sizemfcc;i++){
            mutex.lock();
            if(paused){
                totaltiming+=totaltime.elapsed();
                timing+=time.elapsed();
                Paused.wait(&mutex);
                totaltime.restart();
            }
            else{
                timing+=time.elapsed();
            }

            if(canceled){
                break;
            }
            mutex.unlock();

            time.restart();
            std::vector<std::vector<double >> MFCC;
            std::vector<int* > PHN;
            mfccholder->GetItem(i,&MFCC,&PHN);
            std::vector<int > StateAssignments(MFCC.size(),0);
            int w=i;
            CheckClusters(MFCC,PHN,&StateAssignments,&w);
            SegmentalK_means(MFCC,PHN,&StateAssignments,1-(float)(MFCC.size()-w)/MFCC.size());
            if(i>=starting){
                right+=MFCC.size()-w;
                total+=MFCC.size();
                if(min>(100*(float)(MFCC.size()-w)/MFCC.size())){
                    min=(100*(float)(MFCC.size()-w)/MFCC.size());
                }
                if(max<(100*(float)(MFCC.size()-w)/MFCC.size())){
                    max=(100*(float)(MFCC.size()-w)/MFCC.size());
                }
            }
            emit Progress((float)(i+1)/sizemfcc*100);
        }
        AVGtime/=sizemfcc;
        emit ConsoleLog("Success rate :"+QString::number(100*right/total)+"%");
        emit ConsoleLog("Minimum Success rate :"+QString::number(min)+"%");
        emit ConsoleLog("Maximum Success rate :"+QString::number(max)+"%");
        emit ConsoleLog("Viterbi Algorithm had");
        emit ConsoleLog("Minimum Time :"+QString::number(mintime)+" ms");
        emit ConsoleLog("Maximum Time :"+QString::number(maxtime)+" ms");
        emit TimingReport(-1);
        timing+=time.elapsed();
        if(timing<1000){
            emit ConsoleLog("Evaluation done in "+QString::number(timing)+" ms");
        }
        else if (timing<60000){
           emit ConsoleLog("Evaluation done in "+QString::number(timing/1000)+" s + "+QString::number(timing%1000)+" ms");
        }
        else{
            emit ConsoleLog("Evaluation done in "+QString::number(timing/60000)+" m + "+QString::number((timing/1000)%60)+" s + "+QString::number(timing%1000)+" ms");
        }


        double comparator=0.001*pow(10,(float)(minimumruns/maxrecursions)*3+1);
        if(succrate==0&& AVGtime<200){
            succrate=100*right/total;
            lastnumofg=gaussians;
        }
        else if(fabs(succrate-100*right/total)<comparator&& abs(lastnumofg-gaussians)<0.01*gaussians||AVGtime>200){
            converged=true;
        }
        else{
           succrate=100*right/total;
           lastnumofg=gaussians;
        }

    }
    for(int i=0;i<States;i++){
        QString line=phnlib->GetPhoneme(i)->toString()+": ";
        for(int j=0;j<States;j++){
            if(ConfusionMatrix[i][j]!=0){
                line+= phnlib->GetPhoneme(j)->toString()+":"+QString::number(ConfusionMatrix[i][j])+",";
            }
        }
        emit ConsoleLog(line);
    }

    totaltiming+=totaltime.elapsed();
    if(totaltiming<1000){
        emit ConsoleLog("Training Process done in "+QString::number(totaltiming)+" ms");
    }
    else if (totaltiming<60000){
       emit ConsoleLog("Training Process done in "+QString::number(totaltiming/1000)+" s + "+QString::number(totaltiming%1000)+" ms");
    }
    else{
        emit ConsoleLog("Training Process done in "+QString::number(totaltiming/60000)+" m + "+QString::number((totaltiming/1000)%60)+" s + "+QString::number(totaltiming%1000)+" ms");
    }
    emit Progress2(100);
    emit TrainingDone();
}
// /THREAD

void HMMTrainer::SaveFile(QString filename){
    // to do write to file
    return;
    QFile hmmfile(filename);

    for(int i=0;i<States;i++){

    }
    hmmfile.close();
}

std::vector<int> HMMTrainer::InitialAssignment(std::vector<std::vector<double>> ObVec,std::vector<int* > States,std::vector<int > StateAssignments){
    std::vector<int> StartEndFrame;
    StartEndFrame.push_back(0);
    int sum=0;
    for(int i=0;i<States.size();i++){
        int start=States[i][1];
        int end= States[i][2];
        int temp=1+(int)((end-framesize)/framestep);
        if((end-framesize)%framestep>framesize/2.0){
            temp++;
        }
        if((start-framesize)%framestep>framesize/2.0){
            temp--;
        }
        sum=temp;
        StartEndFrame.push_back(temp);
    }
    if(ObVec.size()!=sum){
        StartEndFrame.pop_back();
        StartEndFrame.push_back(ObVec.size());
        sum=ObVec.size();
    }
    if(sum!=ObVec.size()){
        emit ConsoleLog("not equal "+QString::number(sum)+"  ,  "+QString::number(ObVec.size()));
    }
    else{
        int j=0;
        int curi=0;
        for(int i=0;i<StateAssignments.size();i++){
            StateAssignments.at(i)=States[j][0];
            if(curi>=StartEndFrame[j+1]-StartEndFrame[j]){
                j++;
                curi=1;
            }
            else{
                curi++;
            }
        }
    }
    return StateAssignments;
}

void HMMTrainer::SegmentalK_means(std::vector<std::vector<double> > ObVec,std::vector<int* > PHN,std::vector<int > *StateAssingments,float percentage){

    std::vector<std::vector<double>> ObVecPS;
    for(int run=1;run<States;run++){
        ObVecPS.clear();
        for(int i=0;i<ObVec.size();i++){
            if(run==(*StateAssingments)[i]){
                ObVecPS.push_back(ObVec[i]);
            }
        }
        // run G-means
        int temp=Observations[run].numofG;

        if(ObVecPS.size()>0){
            Observations[run].Gmeans(ObVecPS);
        }
        if(Observations[run].numofG!=temp||Observations[run].lastmerged!=0){
           //emit ConsoleLog("Mog "+QString::number(run)+" has "+QString::number( Observations[run].numofG)+" centers. And merged "+QString::number( Observations[run].lastmerged));
        }
    }

}



bool HMMTrainer::CheckClusters(std::vector<std::vector<double>> ObVec,std::vector<int* > States,std::vector<int > *StateAssignments,int * wrongs){
    std::vector<int> Sequence ;
    int file=*wrongs;
    QTime time;

    time.start();
    if(opencl){
        Sequence=ViterbiCL(ObVec);
    }
    else{
        Sequence=Viterbi(ObVec);
    }
    int timing= time.elapsed();
    AVGtime+=timing;
    //emit ConsoleLog("File "+QString::number(file+1)+" Viterbi done in "+QString::number(timing)+" ms.");
    emit TimingReport(timing);
    if(mintime<0||mintime>timing){
        mintime=timing;
    }
    if(maxtime<0||maxtime<timing){
        maxtime=timing;
    }
    // exw to sequence
    std::vector<int> StartEndFrame;
    StartEndFrame.push_back(0);
    int sum=0;
    for(int i=0;i<States.size();i++){
        int start=States[i][1];
        int end= States[i][2];
        int temp=1+(int)((end-framesize)/framestep);
        if((end-framesize)%framestep>framesize/2.0){
            temp++;
        }
        if((start-framesize)%framestep>framesize/2.0){
            temp--;
        }
        sum=temp;
        StartEndFrame.push_back(temp);
    }
    if(Sequence.size()!=sum){
        StartEndFrame.pop_back();
        StartEndFrame.push_back(Sequence.size());
        sum=Sequence.size();
    }
    if(sum!=Sequence.size()){
        emit ConsoleLog("not equal "+QString::number(sum)+"  ,  "+QString::number(Sequence.size()));
    }
    else{
        int j=0;
        int curi=0;
        int changenum=0;
        bool changed=false;
        bool allzero=true;
        for(int i=0;i<Sequence.size();i++){
            ConfusionMatrix[Sequence[i]][States[j][0]]++;
            if(Sequence[i]!=States[j][0]){
                //emit ConsoleLog(QString::number(i)+": State found " +QString::number(Sequence[i])+" "+phnlib->GetPhoneme(Sequence[i])->toString()+" State should be "+phnlib->GetPhoneme(States[j][0])->toString() +" " +QString::number(States[j][0]));
                //if(!(Sequence[i]==0&&States[j][0]==1||Sequence[i]==1&&States[j][0]==0)){
                    States[j][0]!=0?StateAssignments->at(i)=States[j][0]:-1;
                    changed=true;
                    changenum++;
                    if(Sequence[i]!=0){
                        allzero=false;
                    }
                /*}
                else{
                    StateAssignments->at(i)=-1;
                }
                //*/
            }
            else{
                //emit ConsoleLog("State right " +QString::number(i)+" and is "+QString::number(Sequence[i]));
                StateAssignments->at(i)=-1;
                if(Sequence[i]!=0){
                    allzero=false;
                }
            }
            if(curi>=StartEndFrame[j+1]-StartEndFrame[j]){
                j++;       
                curi=1;
            }
            else{
                curi++;
            }
        }
        if(allzero){
            //emit ConsoleLog("File "+QString::number(file+1)+" Found wrong :"+QString::number(changenum)+" from "+QString::number(Sequence.size())+" success rate:"+QString::number(100*(1-(float)(changenum)/Sequence.size()))+"% allzero");
        }
        else{
            //emit ConsoleLog("File "+QString::number(file+1)+" Found wrong :"+QString::number(changenum)+" from "+QString::number(Sequence.size())+" success rate:"+QString::number(100*(1-(float)(changenum)/Sequence.size()))+"%");
        }
        *wrongs=changenum;
        return changed;
    }
    return false;


}


std::vector<int> HMMTrainer::ViterbiCL(std::vector<std::vector<double >> ObVec){
    std::vector<std::vector<double>> means;
    std::vector<std::vector<double>> sigmas;
    std::vector<double> priors;
    std::vector<double> Dets;
    std::vector<int> sumindex;
    sumindex.push_back(Observations[0].GetData(&means,&sigmas,&Dets,&priors));
    for(int i=1;i<States;i++){
        sumindex.push_back(sumindex[sumindex.size()-1]+Observations[i].GetData(&means,&sigmas,&Dets,&priors));
    }

    std::vector<std::vector<double>>Matrix=opclint->ViterbiStep1(ObVec,States,means,sigmas,priors,Dets,sumindex,avgenergy);
    std::vector<int> Sequence= opclint->ViterbiStep2(Matrix,Initials,Transitions,States,ObVec.size());
    return Sequence;
}


std::vector<int> HMMTrainer::Viterbi(std::vector<std::vector<double >> ObVec){
    std::vector<int> Sequence;
    std::vector<std::vector<double>> Matrix;
    std::vector<std::vector<int>> TransitionMat;
    std::vector<double> curcol;
    std::vector<int> curtrancol;
    for(int i=0;i<States;i++){
        double TiO1=Observations[i].evaluate(ObVec[0],avgenergy);
        curcol.push_back(Initials[i]+TiO1);
        curtrancol.push_back(-1);
    }
    Matrix.push_back(curcol);
    for(int j=1;j<ObVec.size();j++){
        curcol.clear();
        curtrancol.clear();
        for(int i=0;i<States;i++){
            double TiOj=Observations[i].evaluate(ObVec[j],avgenergy);
            double max=Matrix[j-1][0]+Transitions[0][i];
            double temp;
            int maxindex=0;
            for(int k=1;k<States;k++){
                temp=Matrix[j-1][k]+Transitions[k][i];
                if(temp>max){
                    max=temp;
                    maxindex=k;
                }
            }

            curcol.push_back(max+TiOj);
            curtrancol.push_back(maxindex);
        }
        Matrix.push_back(curcol);
        TransitionMat.push_back(curtrancol);
    }

    double max=Matrix[Matrix.size()-1][0];
    double temp;
    int maxindex=0;
    for(int ind=1;ind<States;ind++){
        temp=Matrix[Matrix.size()-1][ind];
        if(temp>max){
            max=temp;
            maxindex=ind;
        }
    }
    Sequence.push_back(maxindex);
    for(int jnd=1;jnd<TransitionMat.size();jnd++){
        maxindex=TransitionMat[TransitionMat.size()-jnd][maxindex];
        Sequence.push_back(maxindex);
    }
    Sequence.push_back(0);
    std::reverse(Sequence.begin(),Sequence.end());
    return Sequence;
}



void HMMTrainer::FillMatrices(){
    States=phnlib->PhonemesCount();
    Initials.clear();
    double initsum=0;
    for(int i=0;i<States;i++){
        Initials.push_back((phnlib->GetPhoneme(i))->TimesFoundFirst());
        initsum+=Initials[i];
    }
    if(initsum==0){
        for(int i=0;i<States;i++){
            Initials[i]=log(1.0/States);
        }
    }
    else{
        for(int i=0;i<States;i++){
            Initials[i]=log((1.0/States+Initials[i]/initsum)/2.0);
        }
    }
    emit Progress(2);
    Transitions.clear();
    std::vector<double> sums;
    for(int i=0;i<States;i++){
        sums.push_back(0);
        Transitions.push_back(std::vector<double>(States));
        ConfusionMatrix.push_back(std::vector<int>(States,0));
        for(int j=0;j<States;j++){
            Transitions[i][j]=phnlib->GetTransition(i,j);
            sums[i]+=Transitions[i][j];
        }
    }
    emit Progress(4);
    for(int i=0;i<States;i++){
        for(int j=0;j<States;j++){
            if(i==j){
                if(i==0){
                    Transitions[i][j]=log(0.9);
                }
                else{
                    Transitions[i][j]=log(0.6);
                }

            }
            else{
                if(i==0){
                    Transitions[i][j]=log((1.0/(2*States)+Transitions[i][j]/(2*sums[i]))*0.1);
                }
                else{
                    Transitions[i][j]=log((1.0/(2*States)+Transitions[i][j]/(2*sums[i]))*0.4);
                }
            }
        }
    }
    emit Progress(6);

    ObservationSize=mfccholder->GetSizeofObservations();
    for(int i=0;i<States;i++){
        Observations.push_back(MoG(ObservationSize,percent));
    }
    emit Progress(8);
}
