#include "SoundDataOperators.h"



void SoundDataOperators::FFT(std::vector<double>* frame,std::vector<double>* dftframe){
    int n=pow(2,ceil(log2(frame->size())));
    std::vector<double> dframe;
    dframe.clear();

    for(int i=0;i<frame->size();i++){
        dframe.push_back(frame->at(i));
        dframe.push_back(0.0);
    }
    for(int i=0;i<n-frame->size();i++){
        dframe.push_back(0.0);
        dframe.push_back(0.0);
    }
    FFTLoop(&dframe);
    double max=0;
    for(int i=0;i<dframe.size()/2;i+=2){
        dftframe->push_back(sqrt(pow(dframe.at(i),2)+pow(dframe.at(i+1),2)));
        /*
        if(max<dftframe->at(dftframe->size()-1)){
            max=dftframe->at(dftframe->size()-1);
        }
        //*/
    }
    /*
    for(int i=0;i<dftframe->size();i++){
        dftframe->at(i)/=max;
    }
    //*/
}


void SoundDataOperators::FCT(std::vector<double>* frame,std::vector<double>* dftframe){
    int n=pow(2,ceil(log2(frame->size())));
    std::vector<double> dframe;
    dframe.clear();
    for(int i=0;i<frame->size();i++){
        dframe.push_back(0.0);
        dframe.push_back(frame->at(i));
    }
    for(int i=0;i<n-frame->size();i++){
        dframe.push_back(0.0);
        dframe.push_back(0.0);
    }
    for(int i=0;i<n-frame->size();i++){
        dframe.push_back(0.0);
        dframe.push_back(0.0);
    }
    for(int i=frame->size()-1;i>=0;i--){
        dframe.push_back(0.0);
        dframe.push_back(frame->at(i));
    }

    FFTLoop(&dframe);
    double max=0;
    for(int i=0;i<dframe.size()/2;i+=2){
        dftframe->push_back(sqrt(pow(dframe.at(i),2)+pow(dframe.at(i+1),2)));
        //dftframe->push_back(dframe.at(i));
        /*
        if(max<dftframe->at(dftframe->size()-1)){
            max=dftframe->at(dftframe->size()-1);
        }
        //*/
    }
    /*
    for(int i=0;i<dftframe->size();i++){
        dftframe->at(i)/=max;
    }
    //*/
}

 void SoundDataOperators::Swap(std::vector<double>* data,unsigned long i,unsigned long j){
    double temp=data->at(i);
    data->at(i)=data->at(j);
    data->at(j)=temp;
 }



void SoundDataOperators::FFTLoop(std::vector<double>* data){
        unsigned long n, mmax, m, j, istep, i;
        double wtemp, wr, wpr, wpi, wi, theta;
        double tempr, tempi;

        // reverse-binary reindexing
        n= data->size();
        //*
        j=0;
        for (i=0; i<n/2; i+=2) {
            if (j>i) {
                Swap(data,j,i);
                Swap(data,j+1,i+1);
                if(j/2<n/4){
                    Swap(data,n-i-2,n-j-2);
                    Swap(data,n-i-1,n-j-1);
                }
            }
            m = n/2;
            while (m>=2 && j>=m) {
                j -= m;
                m >>= 1;
            }
            j += m;
        }
        //*/
        mmax=2;
        while (n>mmax) {
            istep = mmax<<1;
            theta = -(2*M_PI/mmax);
            wtemp = sin(0.5*theta);
            wpr = -2.0*wtemp*wtemp;
            wpi = sin(theta);
            wr = 1.0;
            wi = 0.0;
            for (m=1; m < mmax; m += 2) {
                for (i=m; i <= n; i += istep) {
                    j=i+mmax;
                    tempr = wr*data->at(j-1) - wi*data->at(j);
                    tempi = wr * data->at(j) + wi*data->at(j-1);

                    data->at(j-1) = data->at(i-1) - tempr;
                    data->at(j) = data->at(i) - tempi;
                    data->at(i-1) += tempr;
                    data->at(i) += tempi;
                }
                wtemp=wr;
                wr += wr*wpr - wi*wpi;
                wi += wi*wpr + wtemp*wpi;
            }
            mmax=istep;
        }
}

void SoundDataOperators::Preemphasis(std::vector<double>* frame, std::vector<double>* pframe,double z){
    double minimum=frame->at(0);
    pframe->push_back(frame->at(0));
    for(int i=1;i<frame->size();i++){
        pframe->push_back(frame->at(i)-z*frame->at(i-1));
        if(minimum>=pframe->at(pframe->size()-1)){
            minimum=pframe->at(pframe->size()-1);
        }
    }
    for(int i=0;i<pframe->size();i++){
        pframe->at(i)-=minimum;
    }
}

void SoundDataOperators::HammingFilter(std::vector<double>* frame,std::vector<double>* hframe){
    for(int i=0;i<frame->size();i++){
        hframe->push_back(frame->at(i)*(0.54-0.46*cos(2*M_PI*(i/((double)(frame->size()-1))))));
    }
}


void SoundDataOperators::CreateMelFilterBank(int num,float low,float high,int samplesnum,std::vector<std::vector<double> >*FilterBank,std::vector<int>* inits,std::vector<double>* mids){
    double mel_low=2595*log10(1+low/700);
    double mel_high=2595*log10(1+high/700);
    double mel_step=(mel_high-mel_low)/num;
    double SpH=samplesnum/high;
    for(int i=0;i<num-1;i++){
        double hz_low=700*(pow(10,(mel_low+i*mel_step)/2595)-1);
        double hz_high=700*(pow(10,(mel_low+(i+2)*mel_step)/2595)-1);
        double hz_mid=(hz_high+hz_low)/2.0f;
        double firsthalf=((hz_mid-hz_low)*SpH);
        double secondhalf=((hz_high-hz_mid)*SpH);
        std::vector<double> current=std::vector<double>();
        for(int i=0;i<=(int)firsthalf;i++){
            current.push_back(i/firsthalf);
        }
        for(int i=0;i<=(int) secondhalf;i++){
            current.push_back(1-i/secondhalf);
        }
        inits->push_back((int)hz_low*SpH);
        if(inits->at(inits->size()-1)+current.size()>=samplesnum){
            inits->at(inits->size()-1)--;
        }
        mids->push_back(hz_mid);
        FilterBank->push_back(current);
    }

}


void SoundDataOperators::ApplyMelFilterBank(std::vector<std::vector<double> >* FilterBank,std::vector<int>* inits,std::vector<double>* data,std::vector<double>* output){
    for(int i=0;i<FilterBank->size();i++){
        int start=inits->at(i);
        double sum=0;
        for(int j=0;j<(FilterBank->at(i)).size();j++){
            sum+=data->at(start+j)*(FilterBank->at(i)).at(j);
        }
        output->push_back(sum/(FilterBank->at(i)).size());
    }
}

void SoundDataOperators::Loging(std::vector<double>* frame,std::vector<double>* oframe){
    for(int i=0;i<frame->size();i++){
        oframe->push_back(-log(frame->at(i)));
    }
}


void SoundDataOperators::CalculateDeltas(int window,std::vector<std::vector<double> >* data,std::vector<std::vector<double> >* output){
    for(int i=window;i<data->size()-window;i++){
        std::vector<double> sum (data->at(0).size());
        std::vector<double> dsum (sum.size());
        std::vector<double> result=std::vector<double>(sum.size());
        for(int k=0;k<sum.size();k++){
            for(int j=1;j<=window;j++){
                double temp=data->at(i+j).at(k);
                double temp2=data->at(i-j).at(k);
                sum[k]+=j*(temp-temp2);
                dsum[k]+=j*j;
            }
            result.at(k)=sum[k]/(2*dsum[k]);
        }
        output->push_back(result);
        if(i==window||i==data->size()-window-1){
            for(int j=1;j<=window;j++){
                output->push_back(result);
            }
        }
    }
}



double SoundDataOperators::FrameEnergy(std::vector<double>* frame){
    double Energy=0;
    for(int i=0;i<frame->size();i++){
        Energy+=pow(frame->at(i),2.0);
    }
    return Energy;

}
