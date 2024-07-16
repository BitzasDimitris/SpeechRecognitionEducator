#pragma OPENCL EXTENSION cl_khr_fp64 : enable

#define TWOPI 6.28318530718
#define PI 3.14159265358979323846
#define SQRT_2 0.707106781188


ushort ucharconcat(uchar c1,uchar c2){
    ushort res=0;
    res=res|(ushort)c2<<0;
    res=res|(ushort)c1<<8;
    return res;
}

void kernel CreateWaveForm(global const uchar* Data,global double* Result ,int size){
    int gid=get_global_id(0);

    if(gid>size){
        return;
    }

    unsigned long sample=Data[2*gid+1]*256+Data[2*gid];
    if(sample>32768){
        sample-=65536;
    }
    int value=(int) sample-32768;
    if(value<0){
        value=-(32768+value);
    }
    Result[gid]=value/32768.0;
}



void fft_radix2(local double2* FFTFrameIn,local double2* FFTFrameOut,short p,short t,ushort localid){
        int gid = localid;
        int k = gid & (p - 1);
        int out= ((gid-k) << 1) + k;

        double2 in1 = FFTFrameIn[gid];
        double2 in2 = FFTFrameIn[gid+t];

        double theta = -PI * (double)k /(double) p;
        double cs;
        double sn = sincos(theta, &cs);
        double2 temp = (double2) (in2.x * cs - in2.y * sn,in2.y * cs + in2.x * sn);

        FFTFrameOut[out] = in1 + temp;
        FFTFrameOut[out+p] = in1 - temp;
}



void fct_radix2(local double2* FFTFrameIn,local double2* FFTFrameOut,short p,short t,ushort localid){
        int gid = localid;
        int k = gid & (p - 1);
        int out= ((gid-k) << 1) + k;

        double2 in1 = FFTFrameIn[gid];
        double2 in2 = FFTFrameIn[gid+t];

        double theta = -PI * (double)k /(double) p;
        double cs;
        double sn = sincos(theta, &cs);
        double2 temp = (double2) (in2.x * cs - in2.y * sn,in2.y * cs + in2.x * sn);

        FFTFrameOut[out] = in1 + temp;
        FFTFrameOut[out+p] = in1 - temp;
}

void kernel CreateFCTFrames(global const double* MLFrames,global double* FCTFrames,short framesize,short n, local double2* FCTFrameIn,local double2* FCTFrameOut){
    uint frame=get_global_id(0);
    ushort localid=get_local_id(0);
    ushort worksize=get_local_size(0);
    frame/=worksize;

    if(localid<worksize/2){
        if(framesize<=localid){
            FCTFrameIn[localid]=(double2)(0,0);
        }
        else{
            FCTFrameIn[localid]=(double2)(0,MLFrames[frame*framesize+localid]);
        }
    }
    else{
        if(worksize-framesize-1>=localid){
            FCTFrameIn[localid]=(double2)(0,0);
        }
        else{
            FCTFrameIn[localid]=(double2)(0,MLFrames[frame*framesize+worksize-1-localid]);
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    short offset=0;
    //radix2
    for(short p=1;p<=n/2;p*=2){
        if(localid<n/2){
            if(offset==0){
                fct_radix2(FCTFrameIn,FCTFrameOut,p,n/2,localid);
                offset=1;
            }
            else{
                fct_radix2(FCTFrameOut,FCTFrameIn,p,n/2,localid);
                offset=0;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(localid<n/2){
        if(offset==0){
            FCTFrames[frame*n/2+localid]=sqrt(pown(FCTFrameIn[localid].x,2)+pown(FCTFrameIn[localid].y,2));
        }
        else{
            FCTFrames[frame*n/2+localid]=sqrt(pown(FCTFrameOut[localid].x,2)+pown(FCTFrameOut[localid].y,2));
        }


    }

}

void kernel CreateFFTFrames(global const double* HFrames,global double* FFTFrames,short framesize,short n, local double2* FFTFrameIn,local double2* FFTFrameOut){
    uint frame=get_global_id(0);
    ushort localid=get_local_id(0);
    ushort worksize=get_local_size(0);
    frame/=worksize;
    short numofites=framesize/worksize;
    if(framesize%worksize>localid){
        numofites++;
    }
    else{
       FFTFrameIn[localid+worksize*numofites]=(double2)(0,0);
    }
    // prepare data
    for(short i=0;i<numofites;i++){
        FFTFrameIn[localid+worksize*i]=(double2)(HFrames[frame*framesize+localid+worksize*i],0);
    }

    barrier(CLK_LOCAL_MEM_FENCE);
    short offset=0;

    //radix2
    for(short p=1;p<=n/2;p*=2){
        if(localid<n/2){
            if(offset==0){
                fft_radix2(FFTFrameIn,FFTFrameOut,p,n/2,localid);
                offset=1;
            }
            else{
                fft_radix2(FFTFrameOut,FFTFrameIn,p,n/2,localid);
                offset=0;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    short ites=(n/2)/worksize;
    if(ites==0){
        ites++;
    }

    if(offset==0){
        for(short i=0;i<ites;i++){
            FFTFrames[frame*n/2+i*worksize+localid]=sqrt(pown(FFTFrameIn[i*worksize+localid].x,2)+pown(FFTFrameIn[i*worksize+localid].y,2));
        }
    }
    else{
        for(short i=0;i<ites;i++){
            FFTFrames[frame*n/2+i*worksize+localid]=sqrt(pown(FFTFrameOut[i*worksize+localid].x,2)+pown(FFTFrameOut[i*worksize+localid].y,2));
        }
    }

}


void kernel CreateFramesPlusHam(global const double* Waveform,global double* HFrames,global double* Frames ,int Waveformsize,double itstep,short framesize,short framestep,local double* Frame,local double* PFrame){
    uint frame=get_global_id(0);
    ushort localid=get_local_id(0);
    ushort worksize=get_local_size(0);
    frame/=worksize;

    short numofites=framesize/worksize;
    if(framesize-(numofites*worksize)>localid){
        numofites++;
    }
    //ftiaxnw ta frames 1frame to kathe workgroup
    int start=(short)(frame*framestep*itstep);
    for(short i=0;i<numofites;i++){
        if(start+(int)(localid+worksize*i)*itstep+1>=Waveformsize){
            Frame[localid+worksize*i]=0;
        }
        else{
            int low=start+(int)((localid+worksize*i)*itstep);
            int high=low+1;
            double diff=(double)((localid+worksize*i)*itstep)-(int)((localid+worksize*i)*itstep);
            double value= Waveform[low]*(1-diff)+Waveform[high]*diff;
            Frame[localid+worksize*i]=value;
        }
    }
    for(short i=0;i<numofites;i++){
        Frames[frame*framesize+i*worksize+localid]=Frame[localid+worksize*i];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    // preemphasis
    if(localid!=0){
        for(short i=0;i<numofites;i++){
            PFrame[localid+worksize*i]=Frame[localid+worksize*i]-0.97*Frame[localid+worksize*i-1];

        }
    }
    else{
        PFrame[0]=Frame[0];
        for(short i=1;i<numofites;i++){
            PFrame[localid+worksize*i]=Frame[localid+worksize*i]-0.97*Frame[localid+worksize*i-1];
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    local double minimum;
    if(localid==0){
        minimum=PFrame[0];
        for(ushort j=1;j<framesize;j++){
            if(minimum>=PFrame[j]){
               minimum=PFrame[j];
            }
        }

    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for(short i=0;i<numofites;i++){
       Frame[localid+worksize*i]=PFrame[localid+worksize*i]-minimum;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    //Hamming filter
    for(short i=0;i<numofites;i++){
        Frame[localid+worksize*i]=Frame[localid+worksize*i]*(0.54-0.46*cos(2*M_PI*((localid+worksize*i)/((double)(framesize-1)))));
    }
    for(short i=0;i<numofites;i++){
        HFrames[frame*framesize+i*worksize+localid]=Frame[localid+worksize*i];
    }

}


void kernel ApplyMelwithLog(global const double* FFTFrames,global double* MLFrames,short framesize, local double* Frame,global const double* Filters,global const int* Fsizes,global const int* inits,short numofFilters,short fsize,local double* mults){
    uint frame=get_global_id(0);
    ushort localid=get_local_id(0);
    ushort worksize=get_local_size(0);
    frame/=worksize;
    short numofites=framesize/worksize;
    // prepare data
    for(short i=0;i<numofites;i++){
        Frame[i*worksize+localid]=FFTFrames[frame*framesize+i*worksize+localid];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    numofites=fsize/worksize;
    if(fsize%worksize>localid||numofites==0){
        numofites++;
    }
    for(short run=0;run<numofites;run++){
        short findex=0;
        short sum=0;
        for(short i=0;i<numofFilters;i++){
            if(run*worksize+localid<sum+Fsizes[i]){
                findex=i;
                break;
            }
            sum+=Fsizes[i];
        }
        mults[run*worksize+localid]=Filters[run*worksize+localid]*Frame[inits[findex]+run*worksize+localid-sum];
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    // calc sums
    if(localid<numofFilters){
        double sum=0;
        short sum1=0;
        for(short i=0;i<localid;i++){
            sum1+=Fsizes[i];
        }
        for(int i=0;i<Fsizes[localid];i++){
            sum+=mults[sum1+i];
        }
        MLFrames[frame*numofFilters+localid]=-log(sum/Fsizes[localid]);
    }
}


void kernel CalculateDeltas(global const double* Frames,global double* deltas,short framesize, local double* Frame,short deltawidth){
    uint frame=get_global_id(0);
    ushort localid=get_local_id(0);
    ushort worksize=get_local_size(0);
    uint globalworksize=get_global_size(0);
    frame/=worksize;
    if(frame>=deltawidth&&frame<globalworksize/worksize-deltawidth){
        for(short i=1;i<=deltawidth;i++){
            Frame[localid+(deltawidth-i)*framesize]=Frames[(frame-i)*framesize+localid];
            Frame[localid+(deltawidth+i-1)*framesize]=Frames[(frame+i)*framesize+localid];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
        double sum=0;
        short dsum=0;
        for(short i=1;i<=deltawidth;i++){
            sum+=i*(Frame[localid+(deltawidth+i-1)*framesize]-Frame[localid+(deltawidth-i)*framesize]);
            dsum+=i*i;
        }
        deltas[frame*framesize+localid]=sum/(2*dsum);
    }

    if(frame==deltawidth){
        for(short i=0;i<deltawidth;i++){
            deltas[i*framesize+localid]=deltas[frame*framesize+localid];
        }
    }
    if(frame==globalworksize/worksize-deltawidth-1){
        for(short i=0;i<deltawidth;i++){
            deltas[(frame+1+i)*framesize+localid]=deltas[frame*framesize+localid];
        }
    }
}

double MoG(global const double* Obvec,global const double* Means,global const double* Sigmas,int Vec,ushort initindex,ushort numofG,ushort col){
    double val=0;
    for(ushort i=0;i<Vec;i++){
        val+=pown(Obvec[col*Vec+i]-Means[(initindex+numofG)*Vec+i],2)/Sigmas[(initindex+numofG)*Vec+i];
    }

    return val;
}


void kernel ViterbiStep1(global const double* Obvec,global const double* Means,global const double* Sigmas,global const double* Priors,global const double* Dets,global const int* SumIndex,int M,int Vec,global double* Matrix,double avgenergy){
    uint State=get_global_id(0);
    ushort col=get_local_id(0);
    ushort worksize=get_local_size(0);
    State=State/worksize;
    ushort ites=M/worksize;
    if(M%worksize>col){
        ites++;
    }
    ushort numofG=SumIndex[State];
    ushort initindex=0;
    if(State>0){
        numofG-=SumIndex[State-1];
        initindex=SumIndex[State-1];
    }
    for(ushort run=0;run<ites;run++){
        double val=0;
        if(numofG==0){
            if(Obvec[(worksize*run+col)*Vec]>avgenergy){
                val=0.6;
            }
        }
        else{
            for(ushort i=0;i<numofG;i++){
                val+=Priors[initindex+i]*(pow(2*M_PI,-Vec/2.0)*1/sqrt(Dets[initindex+i]))*exp(-0.5*MoG(Obvec,Means,Sigmas,Vec,initindex,i,worksize*run+col));
            }
        }
        Matrix[State*M+run*worksize+col]=log(val);
    }
}


void kernel ViterbiStep2(global double* Matrix,global const double* Initials,global const double* Transitions,global int* Sequence,int N,int M,global ushort* Indexes,local double* colbuf){
    ushort row= get_local_id(0);
    Matrix[row]+=Initials[row];
    Indexes[row]=0;
    barrier(CLK_LOCAL_MEM_FENCE);

    for(uint i=1;i<M;i++){

        colbuf[row]=Matrix[(i-1)*N+row];
        barrier(CLK_LOCAL_MEM_FENCE);

        double maximum=colbuf[0]+Transitions[0+row];
        int index=0;
        for(ushort j=1;j<N;j++){
            if(maximum<colbuf[j]+Transitions[j*N+row]){
                maximum=colbuf[j]+Transitions[j*N+row];
                index=j;
            }
        }
        Matrix[i*N+row]+=maximum;
        Indexes[i*N+row]=index;
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    if(row==0){
        double maximum= Matrix[0+(M-1)*N];
        int index=0;
        for(ushort j=1;j<N;j++){
            if(maximum<Matrix[j+(M-1)*N]){
                maximum=Matrix[j+(M-1)*N];
                index=j;
            }
        }
        for(int s=M-1;s>0;s--){
            Sequence[s]=index;
            index=Indexes[s*N+index];
        }
    }
}


