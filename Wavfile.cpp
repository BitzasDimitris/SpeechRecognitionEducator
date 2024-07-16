#include "WavFile.h"


WavFile::WavFile(QWidget *parent,bool destroy)
{
    opencl=false;
    if(destroy){
        connect(this,SIGNAL(finished()),this,SLOT(deleteLater()));
    }
}

WavFile::~WavFile(){
    Frames.clear();
    PFrames.clear();
    HFrames.clear();
    FFrames.clear();
    MFrames.clear();
    MLFrames.clear();
    MLDFrames.clear();
    MDFrames.clear();
    inits.clear();
    mids.clear();
}

QTextStream& qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}

bool WavFile::Initiallize(int fsize,int fstep, int SR, QString fn,bool ocl,OpenCLInterface *oclint){
    framesize=fsize;
    framestep=fstep;
    SampleRate=SR; 
    filename=fn.toStdString();
    opencl=ocl;
    opclint=oclint;
    if(!LoadWavFile()){
        return false;
    }

    return true;
}


bool WavFile::LoadWavFile() {
    //Local Declarations
    FILE* soundFile = NULL;

    try {
    soundFile=fopen(filename.c_str(), "rb");
    if (!soundFile)
        throw (filename);

    // Read in the first chunk into the struct
    fread(&(Rheader), sizeof(RIFF_Header), 1, soundFile);

    //check for RIFF and WAVE tag in memeory
    if ((Rheader.chunkID[0] != 'R' ||
            Rheader.chunkID[1] != 'I' ||
            Rheader.chunkID[2] != 'F' ||
            Rheader.chunkID[3] != 'F') ||
        (Rheader.format[0] != 'W' ||
            Rheader.format[1] != 'A' ||
            Rheader.format[2] != 'V' ||
            Rheader.format[3] != 'E')){
            return false;
    }

    //Read in the 2nd chunk for the wave info
    fread(&(Wformat), sizeof(WAVE_Format), 1, soundFile);
    //check for fmt tag in memory
    if (Wformat.subChunkID[0] != 'f' ||
        Wformat.subChunkID[1] != 'm' ||
        Wformat.subChunkID[2] != 't' ||
        Wformat.subChunkID[3] != ' '){
                return false;
    }
    //check for extra parameters;
    if (Wformat.subChunkSize > 16)
        fseek(soundFile, sizeof(short), SEEK_CUR);
    //Read in the the last byte of data before the sound file
    fread(&(Wdata), sizeof(WAVE_Data), 1, soundFile);
    //check for data tag in memory
    if (Wdata.subChunkID[0] != 'd' ||
        Wdata.subChunkID[1] != 'a' ||
        Wdata.subChunkID[2] != 't' ||
        Wdata.subChunkID[3] != 'a'){
                return false;
    }
    //Allocate memory for data
    (data) = new unsigned char[Wdata.subChunk2Size]();
    // Read in the sound data into the soundData variable
    if (!fread(data,Wdata.subChunk2Size,1, soundFile))
        throw ("error loading WAVE data into struct!");


    //clean up and return true if successful
    fclose(soundFile);
    return true;
    } catch(std::string error) {
    if (soundFile != NULL)
        fclose(soundFile);
    //return false to indicate the failure to load wave
    return false;
    }
}


void WavFile::StartProcess(){
    this->start();
}

void WavFile::freememory(){
    Frames.clear();
    PFrames.clear();
    HFrames.clear();
    FFrames.clear();
    MFrames.clear();
    MLFrames.clear();
    MLDFrames.clear();
    MDFrames.clear();
    inits.clear();
    mids.clear();
    Frames.shrink_to_fit();
    PFrames.shrink_to_fit();
    HFrames.shrink_to_fit();
    FFrames.shrink_to_fit();
    MFrames.shrink_to_fit();
    MLFrames.shrink_to_fit();
    MLDFrames.shrink_to_fit();
    MDFrames.shrink_to_fit();
    inits.shrink_to_fit();
    mids.shrink_to_fit();
}

void WavFile::run(){
    QTime t;


    if(opencl){
        t.start();
        // edw 3ekinaei na trexei opencl oti kanei kai apo katw
        Waveform.clear();
        Waveform=opclint->Execute<uchar,double>(data,Wdata.subChunk2Size,Wdata.subChunk2Size/2,Wdata.subChunk2Size/2,0,"CreateWaveForm");
        CreateFramesCL();
        FFTCL();
        MELCL();
        FCTCL();
        DeltasCL();
    }
    else{
        t.start();
        CreateWaveForm();
        framesnum=1+((int)(Waveform.size()*SampleRate/Wformat.sampleRate) -framesize)/framestep;
        padding=((int)(Waveform.size()*SampleRate/Wformat.sampleRate)-framesize)%framestep;
        CreateFrames();
        Preemphasis();
        Hamming();
        FFT();
        MEL();
        Logs();
        FCT();
        Deltas();
    }
    int timing=t.elapsed();
    QMutex m;
    m.lock();
    emit Threadfinished(id,timing);
    m.unlock();
}

void WavFile::MELCL(){
    MLFrames.clear();
    std::vector<std::vector<double> > FilterBank=std::vector<std::vector<double>>();
    inits=std::vector<int>();
    mids=std::vector<double>();
    SoundDataOperators::CreateMelFilterBank(26,10,SampleRate/2,FFrames[0].size(),&FilterBank,&inits,&mids);
    std::vector<double> Vec2pass=std::vector<double>();
    Vec2pass.insert(Vec2pass.begin(),FFrames[0].begin(),FFrames[0].end());
    for(int i=1;i<FFrames.size();i++){
        Vec2pass.insert(Vec2pass.end(),FFrames[i].begin(),FFrames[i].end());
    }
    std::vector<double> filters=std::vector<double>();
    std::vector<int> fsizes=std::vector<int>();
    filters.insert(filters.begin(),FilterBank[0].begin(),FilterBank[0].end());
    fsizes.push_back(FilterBank[0].size());
    for(int i=1;i<FilterBank.size();i++){
        filters.insert(filters.end(),FilterBank[i].begin(),FilterBank[i].end());
        fsizes.push_back(FilterBank[i].size());
    }

    MLFrames=opclint->Execute<double,double>(Vec2pass,FFrames[0].size(),filters,fsizes,inits,framesnum,FFrames[0].size(),"ApplyMelwithLog");

}

void WavFile::MEL(){
    std::vector<std::vector<double> >* FilterBank= new std::vector<std::vector<double> >;
    inits=std::vector<int>();
    mids=std::vector<double>();
    SoundDataOperators::CreateMelFilterBank(26,10,SampleRate/2,FFrames[0].size(),FilterBank,&inits,&mids);
    for(int i=0;i<FFrames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::ApplyMelFilterBank(FilterBank,&inits,&(FFrames[i]),current);
        MFrames.push_back(*current);
    }
}

void WavFile::Logs(){
    for(int i=0;i<MFrames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::Loging(&(MFrames[i]),current);
        MLFrames.push_back(*current);
    }
}

void WavFile::DeltasCL(){
    std::vector<std::vector<double> > delta;
    std::vector<std::vector<double> > deltadelta;
    std::vector<double> Vec2pass=std::vector<double>();
    Vec2pass.insert(Vec2pass.begin(),MLDFrames[0].begin(),MLDFrames[0].end());
    for(int i=1;i<MLDFrames.size();i++){
        Vec2pass.insert(Vec2pass.end(),MLDFrames[i].begin(),MLDFrames[i].end());
    }
    delta=opclint->Execute<double,double>(Vec2pass,MLDFrames[0].size(),MLDFrames.size(),MLDFrames[0].size(),2,"CalculateDeltas");
    Vec2pass.clear();
    Vec2pass.insert(Vec2pass.begin(),delta[0].begin(),delta[0].end());
    for(int i=1;i<delta.size();i++){
        Vec2pass.insert(Vec2pass.end(),delta[i].begin(),delta[i].end());
    }
    deltadelta=opclint->Execute<double,double>(Vec2pass,delta[0].size(),delta.size(),delta[0].size(),2,"CalculateDeltas");
    for(int i=0;i<MLDFrames.size();i++){
        std::vector<double>* current;
        current =new std::vector<double>(MLDFrames[i]);
        current->insert(current->end(),delta[i].begin(),delta[i].end());
        current->insert(current->end(),deltadelta[i].begin(),deltadelta[i].end());
        MDFrames.push_back(*current);
    }
}


void WavFile::Deltas(){
    std::vector<std::vector<double> > delta;
    std::vector<std::vector<double> > deltadelta;
    SoundDataOperators::CalculateDeltas(2,&MLDFrames,&delta);
    SoundDataOperators::CalculateDeltas(2,&delta,&deltadelta);

    for(int i=0;i<MLDFrames.size();i++){
        std::vector<double>* current;
        current =new std::vector<double>(MLDFrames[i]);
        current->insert(current->end(),delta[i].begin(),delta[i].end());
        current->insert(current->end(),deltadelta[i].begin(),deltadelta[i].end());
        MDFrames.push_back(*current);
    }

}


void WavFile::Hamming(){
    for(int i=0;i<PFrames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::HammingFilter(&(PFrames[i]),current);
        HFrames.push_back(*current);
    }
}

void WavFile::FFTCL(){
    FFrames.clear();
    std::vector<double> Vec2pass=std::vector<double>();
    Vec2pass.insert(Vec2pass.begin(),HFrames[0].begin(),HFrames[0].end());
    for(int i=1;i<HFrames.size();i++){
        Vec2pass.insert(Vec2pass.end(),HFrames[i].begin(),HFrames[i].end());
    }
    if(framesize>1024){
        FFT();
        return;
    }
    opclint->Execute<double,double>(Vec2pass,framesize,framesnum,framesize,"CreateFFTFrames",&FFrames);
    /*
    for(int i=0;i<FFrames.size();i++){

        double max=FFrames[i][0];
        for(int j=1;j<FFrames[i].size();j++){
            if(max<FFrames[i][j]){
                max=FFrames[i][j];
            }
        }

        for(int j=0;j<FFrames[i].size();j++){
            FFrames[i][j]/=max;
        }

    }
    //*/

}


void WavFile::FFT(){
    FFrames.clear();
    for(int i=0;i<HFrames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::FFT(&(HFrames[i]),current);
        FFrames.push_back(*current);
    }
}

void WavFile::FCTCL(){
    MLDFrames.clear();

    std::vector<double> Vec2pass=std::vector<double>();
    Vec2pass.insert(Vec2pass.begin(),MLFrames[0].begin(),MLFrames[0].end());
    for(int i=1;i<MLFrames.size();i++){
        Vec2pass.insert(Vec2pass.end(),MLFrames[i].begin(),MLFrames[i].end());
    }

    opclint->Execute<double,double>(Vec2pass,MLFrames[0].size(),framesnum,MLFrames[0].size(),"CreateFCTFrames",&MLDFrames);

    for(int i=0;i<MLDFrames.size();i++){
        MLDFrames[i].erase(MLDFrames[i].begin()+MLFrames[i].size(),MLDFrames[i].end());
        /*
        double max=MLDFrames[i][0];
        for(int j=0;j<MLDFrames[i].size();j++){
            if(max<MLDFrames[i][j]){
                max=MLDFrames[i][j];
            }
        }
        for(int j=0;j<MLDFrames[i].size();j++){
            MLDFrames[i][j]/=max;
        }
        //*/
    }
    MLDFrames[0][0]=-log(0.00001);
    for(int i=1;i<HFrames.size();i++){
        MLDFrames[i][0]=-log(SoundDataOperators::FrameEnergy(&(Frames[i])));
    }
}

void WavFile::FCT(){
    MLDFrames.clear();
    for(int i=0;i<MLFrames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::FCT(&(MLFrames[i]),current);
        current->at(0)=-log(SoundDataOperators::FrameEnergy(&(Frames[i])));
        MLDFrames.push_back(std::vector<double>(current->begin(),current->begin()+MLFrames[i].size()));
    }
    MLDFrames[0][0]=-log(0.00001);
}



void WavFile::CreateWaveForm(){
    Waveform.clear();

    for(int i=0;i<Wdata.subChunk2Size;i+=2){
        uchar first=data[i+1];
        uchar second=data[i];
        unsigned long sample=first*256+second;
        (sample>32768)?sample-=65536:sample=sample;
        int value=(int) sample-32768;
        (value<0)?value=-(32768+value):value=value;
        Waveform.push_back(value/32768.0);
    }
}

void WavFile::Preemphasis(){
    for(int i=0;i<Frames.size();i++){
        std::vector<double>* current=new std::vector<double>;
        SoundDataOperators::Preemphasis(&(Frames[i]),current,0.97);
        PFrames.push_back(*current);
    }
}
void WavFile::CreateFramesCL(){
    framesnum=1+((int)(Waveform.size()*SampleRate/Wformat.sampleRate) -framesize)/framestep;
    padding=((int)(Waveform.size()*SampleRate/Wformat.sampleRate)-framesize)%framestep;
    double step=Wformat.sampleRate/SampleRate;
    Frames.clear();
    HFrames.clear();
    HFrames=opclint->Execute<double,double>(Waveform,framesnum,framesize,framestep,step,framesnum,framesize,&Frames,"CreateFramesPlusHam");
}


void WavFile::CreateFrames(){
    double step=Wformat.sampleRate/SampleRate;
    for(int i=0;i<framesnum;i++){
        std::vector<double>* current=new std::vector<double>();
        int start=(int)( i*framestep*step);
        for(int j=0;j<framesize;j++){
           if(start+(int)j*step+1>=Waveform.size()){
               current->push_back(0.0);
           }
           else{
               int low=start+(int)(j*step);
               int high=low+1;
               double diff=j*step-(int)(j*step);
               double value= Waveform[low]*(1-diff)+Waveform[high]*diff;
               current->push_back(value);
           }
        }
        Frames.push_back(*current);
    }
}

int WavFile::GetFrameNumber(){
    return framesnum;
}


void WavFile::GetFrame(int j,QVector<double>* x,QVector<double>* y,float xstep,float *size){
    for(int i=0;i<Frames[j].size();i++){
        x->append(i*xstep);
        y->append(Frames[j].at(i));
    }
    *size=Frames[j].size()*xstep;
}

void WavFile::GetPFrame(int j,QVector<double>* x,QVector<double>* y,float xstep,float *size){
    for(int i=0;i<PFrames[j].size();i++){
        x->append(i*xstep);
        y->append(PFrames[j].at(i));
    }
    *size=PFrames[j].size()*xstep;
}

void WavFile::GetHFrame(int j,QVector<double>* x,QVector<double>* y,float xstep,float *size){
    for(int i=0;i<HFrames[j].size();i++){
        x->append(i*xstep);
        y->append(HFrames[j].at(i));
    }
    *size=HFrames[j].size()*xstep;
}
void WavFile::GetFFrame(int j,QVector<double>* x,QVector<double>* y,float *size){
    float xstep=(SampleRate/2)/FFrames[j].size();
    for(int i=0;i<FFrames[j].size();i++){
        x->append(i*xstep);
        y->append(FFrames[j].at(i));
    }
    *size=SampleRate/2;
}

void WavFile::GetMFrame(int j,QVector<double>* x,QVector<double>* y,float *size){
    for(int i=0;i<MFrames[j].size();i++){
        x->append(mids.at(i));
        y->append(MFrames[j].at(i));
    }
    *size=SampleRate/2;
}

void WavFile::GetMLFrame(int j,QVector<double>* x,QVector<double>* y,float *size){
    for(int i=0;i<MLFrames[j].size();i++){
        x->append(mids.at(i));
        y->append(MLFrames[j].at(i));
    }
    *size=SampleRate/2;
}

void WavFile::GetMLDFrame(int j,QVector<double>* x,QVector<double>* y,float *size){
    for(int i=0;i<MLDFrames[j].size();i++){
        x->append(i);
        y->append(MLDFrames[j].at(i));
    }
    *size=MLDFrames[0].size();
}

void WavFile::GetMDFrame(int j,QVector<double>* x,QVector<double>* y,float *size){
    for(int i=0;i<MDFrames[j].size();i++){
        x->append(i);
        y->append(MDFrames[j].at(i));
    }
    *size=MDFrames[0].size();
}

void WavFile::GetWaveForm(QVector<double>* x,QVector<double>* y,float xstep,float *size){
    for(int i=0;i<Waveform.size();i++){
        x->append(i*xstep/Wformat.sampleRate);
        y->append(Waveform[i]);
    }
    *size=Waveform.size()*xstep/Wformat.sampleRate;
}

void WavFile::GetWaveForm(int SampleRate,QVector<double>* x,QVector<double>* y,float xstep,float *size){
    double step=Wformat.sampleRate/(double)(SampleRate);
    for(int i=0;i*step<Waveform.size()-1;i++){
        int low=(int)( i*step);
        int high=low+1;
        double diff=i*step-low;
        double value= Waveform[low]*(1-diff)+Waveform[high]*diff;
        x->append(i*xstep/SampleRate);
        y->append(value);
    }
    *size=x->size()*xstep/SampleRate;

}

void WavFile::GetMFCC(std::vector<std::vector<double> > *MFCCFrames){
    for(int i=0;i<MDFrames.size();i++){
        MFCCFrames->push_back(MDFrames[i]);
    }
}


int WavFile::GetId(){
    return id;
}

void WavFile::SetId( int i){
    id=i;
}




