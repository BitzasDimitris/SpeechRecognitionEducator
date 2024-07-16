#ifndef OPENCLINTERFACE_H
#define OPENCLINTERFACE_H


#include <CL/cl.hpp>
#include <vector>
#include <QObject>
#include <QString>
#include <iostream>
#include <QResource>
#include <QFile>


class OpenCLInterface : public QObject
{
    Q_OBJECT
public:
    OpenCLInterface();
    bool InitiallizeOpenCL();
    std::vector<QString> getplatforms();
    std::vector<QString> getdevices();
    QString getDeviceName();
    int getdefaultplatform();
    int getdefaultdevice();
    void setdefaultplatform(int);
    void setdefaultdevice(int);
    std::vector<std::vector<double>> ViterbiStep1(std::vector<std::vector<double>>obvec,int states,std::vector<std::vector<double>> means,std::vector<std::vector<double>>sigmas,std::vector<double> priors,std::vector<double> Dets,std::vector<int>sumindex,double avgenergy);
    std::vector<int> ViterbiStep2(std::vector<std::vector<double>>Matrix,std::vector<double> Initials,std::vector<std::vector<double>>Transitions,int states,int seqsize);
    //execute variables

    template<typename T,typename T2>
    std::vector<std::vector<T2>> Execute(std::vector<T> A,int Ressize1,int Ressize2,int Resstep,double info,int global,int local,std::vector<std::vector<T2>>* Frames ,std::string func){
        std::vector<T2> Result=std::vector<T2>(Ressize1*Ressize2);
        std::vector<T2> Result2=std::vector<T2>(Ressize1*Ressize2);
        cl::Buffer bufA(MainContext,CL_MEM_READ_ONLY,sizeof(T)*A.size());
        cl::Buffer bufB(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*Ressize1*Ressize2);
        cl::Buffer bufC(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*Ressize1*Ressize2);
        cl::LocalSpaceArg localbuf= cl::Local(sizeof(T2)*Ressize2);
        cl::LocalSpaceArg localbuf2= cl::Local(sizeof(T2)*Ressize2);
        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*A.size(),A.data());
        // call function
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        kernel.setArg(0,bufA);
        kernel.setArg(1,bufB);
        kernel.setArg(2,bufC);
        kernel.setArg(3,A.size());
        kernel.setArg(4,info);
        kernel.setArg(5,(short)Ressize2);
        kernel.setArg(6,(short)Resstep);
        kernel.setArg(7,localbuf);
        kernel.setArg(8,localbuf2);
        if(local!=0){
            if(local>maxworkitems[0]){
                 queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*maxworkitems[0]),cl::NDRange(maxworkitems[0]));
            }
            else{
                queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*local),cl::NDRange(local));
            }
        }
        else{
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);
        }
        queue.finish();
        // read results
        queue.enqueueReadBuffer(bufB, CL_TRUE, 0, sizeof(T2)*Ressize1*Ressize2,Result.data());
        queue.enqueueReadBuffer(bufC, CL_TRUE, 0, sizeof(T2)*Ressize1*Ressize2,Result2.data());
        std::vector<std::vector<T2>> result2return;
        for(int i=0;i<global;i++){
            std::vector<T2>current,current2;
            current.clear();
            current2.clear();
            for(int j=0;j<local;j++){
                current.push_back(Result[i*local+j]);
                current2.push_back(Result2[i*local+j]);
            }
            result2return.push_back(current);
            Frames->push_back(current2);
        }

        return result2return;
    }

    template<typename T,typename T2>
    std::vector<std::vector<T2>> Execute(std::vector<T> A,int framesize,std::vector<T>B,std::vector<int> C,std::vector<int> D,int global,int local, std::string func){
        std::vector<T2> Result=std::vector<T2>(global*C.size());
        cl::Buffer bufA(MainContext,CL_MEM_READ_ONLY,sizeof(T)*A.size());
        cl::Buffer bufout(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*global*C.size());
        cl::LocalSpaceArg localbuf= cl::Local(sizeof(T)*framesize);
        cl::LocalSpaceArg localbuf2= cl::Local(sizeof(T)*B.size());
        cl::Buffer bufB(MainContext,CL_MEM_READ_ONLY,sizeof(T)*B.size());
        cl::Buffer bufC(MainContext,CL_MEM_READ_ONLY,sizeof(int)*C.size());
        cl::Buffer bufD(MainContext,CL_MEM_READ_ONLY,sizeof(int)*D.size());

        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*A.size(),A.data());
        queue.enqueueWriteBuffer(bufB,CL_FALSE,0,sizeof(T)*B.size(),B.data());
        queue.enqueueWriteBuffer(bufC,CL_FALSE,0,sizeof(int)*C.size(),C.data());
        queue.enqueueWriteBuffer(bufD,CL_FALSE,0,sizeof(int)*D.size(),D.data());
        // call function
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        kernel.setArg(0,bufA);
        kernel.setArg(1,bufout);
        kernel.setArg(2,(short)framesize);
        kernel.setArg(3,localbuf);
        kernel.setArg(4,bufB);
        kernel.setArg(5,bufC);
        kernel.setArg(6,bufD);
        kernel.setArg(7,(short)C.size());
        kernel.setArg(8,(short)B.size());
        kernel.setArg(9,localbuf2);
        cl_int error=0;
        if(local!=0){
            if(local>maxworkitems[0]){
                 error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*maxworkitems[0]),cl::NDRange(maxworkitems[0]));
            }
            else{
                error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*local),cl::NDRange(local));
            }
        }
        else{
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);
        }
        queue.finish();
        // read results
        queue.enqueueReadBuffer(bufout, CL_TRUE, 0, sizeof(T2)*global*C.size(),Result.data());
        std::vector<std::vector<T2>> result2return;
        for(int i=0;i<global;i++){
            std::vector<T2>current;
            current.clear();
            for(int j=0;j<C.size();j++){
                current.push_back(Result[i*C.size()+j]);
            }
            result2return.push_back(current);
        }

        return result2return;
    }


    template<typename T,typename T2>
    std::vector<T2> Execute(std::vector<T> A,int framesize, std::string func){
        ushort n=2*pow(2,ceil(log2(framesize)));
        std::vector<T2> Result=std::vector<T2>(n);
        std::vector<double>init;
        for(int i=0;i<A.size();i++){
            init.push_back(A[i]);
            init.push_back(0);
        }
        for(int i=0;i<n/2-A.size();i++){
            init.push_back(0);
            init.push_back(0);
        }
        cl::Buffer bufA(MainContext,CL_MEM_READ_WRITE,sizeof(T)*init.size());
        cl::Buffer bufB(MainContext,CL_MEM_READ_WRITE,sizeof(T2)*n);
        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*init.size(),init.data());
        // call function
        int block_size=1;
        int global= 64;
        cl::Buffer  in=bufA;
        cl::Buffer  out=bufB;
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        for(int t=0;t<3;t++){
            kernel.setArg(0,in);
            kernel.setArg(1,out);
            kernel.setArg(2,block_size);
            kernel.setArg(3,n/8);
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);

            cl::Buffer  temp=in;
            in=out;
            out=temp;
            block_size*=8;
        }
        queue.finish();
        // read results
        queue.enqueueReadBuffer(in, CL_TRUE, 0, sizeof(T2)*n,Result.data());
        std::vector<double> result2pass;
        for(int i=0;i<n;i+=2){
            result2pass.push_back(sqrt(pow(Result[i],2)+pow(Result[i+1],2)));
        }

        return result2pass;
    }

    template<typename T,typename T2>
    void Execute(std::vector<T> A,int framesize,int global,int local, std::string func,std::vector<std::vector<T2>> *result2return){
        ushort n=pow(2,ceil(log2(framesize)));
        std::vector<T2> Result;
        cl::Buffer bufA(MainContext,CL_MEM_READ_ONLY,sizeof(T)*A.size());
        cl::Buffer bufB;
        cl::LocalSpaceArg localbuf;
        cl::LocalSpaceArg localbuf1;
        bool fct=false;
        if(std::strcmp(func.c_str(),"CreateFCTFrames")==0){
            fct=true;
            n=2*n;
        }
        Result=std::vector<T2>(global*n/2);
        bufB= cl::Buffer(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*global*n/2);
        localbuf= cl::Local(sizeof(T2)*2*n);
        localbuf1= cl::Local(sizeof(T2)*2*n);
        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*A.size(),A.data());
        // call function
        local=n;
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        kernel.setArg(0,bufA);
        kernel.setArg(1,bufB);
        kernel.setArg(2,(short)framesize);
        kernel.setArg(3,n);
        kernel.setArg(4,localbuf);
        kernel.setArg(5,localbuf1);
        cl_int error;
        if(local!=0){
            if(local>maxworkitems[0]){
                 error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*maxworkitems[0]),cl::NDRange(maxworkitems[0]));
            }
            else{
                error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*local),cl::NDRange(local));
            }
        }
        else{
            error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);
        }
        if(error){
            emit ConsoleLog("kernel error "+QString::number(error));
        }
        error=queue.finish();
        if(error){
            emit ConsoleLog("kernel error "+QString::number(error));
        }
        // read results
        error=queue.enqueueReadBuffer(bufB, CL_TRUE, 0, sizeof(T2)*global*n/2,Result.data());
        if(error){
            emit ConsoleLog("kernel error "+QString::number(error));
        }
        for(int i=0;i<global;i++){
            std::vector<T2>current;
            current.clear();
            for(int j=0;j<n/2;j++){
                current.push_back(Result[i*n/2+j]);
            }
            result2return->push_back(current);
        }
    }

    template<typename T,typename T2>
    std::vector<std::vector<T2>> Execute(std::vector<T> A,int framesize,int global,int local,int deltawidth ,std::string func){
        std::vector<T2> Result=std::vector<T2>(global*framesize);
        cl::Buffer bufA(MainContext,CL_MEM_READ_ONLY,sizeof(T)*A.size());
        cl::Buffer bufB(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*global*framesize);
        cl::LocalSpaceArg localbuf= cl::Local(sizeof(T2)*(2*deltawidth)*framesize);
        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*A.size(),A.data());
        // call function
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        kernel.setArg(0,bufA);
        kernel.setArg(1,bufB);
        kernel.setArg(2,(short)framesize);
        kernel.setArg(3,localbuf);
        kernel.setArg(4,(short)deltawidth);
        if(local!=0){
            if(local>maxworkitems[0]){
                 queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*maxworkitems[0]),cl::NDRange(maxworkitems[0]));
            }
            else{
                queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global*local),cl::NDRange(local));
            }
        }
        else{
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);
        }
        queue.finish();
        // read results
        queue.enqueueReadBuffer(bufB, CL_TRUE, 0, sizeof(T2)*global*framesize,Result.data());
        std::vector<std::vector<T2>> result2return;
        for(int i=0;i<global;i++){
            std::vector<T2>current;
            current.clear();
            for(int j=0;j<framesize;j++){
                current.push_back(Result[i*framesize+j]);
            }
            result2return.push_back(current);
        }

        return result2return;
    }

    template<typename T,typename T2>
    std::vector<T2> Execute(const T* A,int sizeA,int Ressize,int global,int local, std::string func){
        local=maxworkitems[0];
        std::vector<T2> Result=std::vector<T2>(Ressize,0);
        cl::Buffer bufA(MainContext,CL_MEM_READ_ONLY,sizeof(T)*sizeA);
        cl::Buffer bufB(MainContext,CL_MEM_WRITE_ONLY,sizeof(T2)*Ressize);
        // write to device
        queue.enqueueWriteBuffer(bufA,CL_FALSE,0,sizeof(T)*sizeA,A);
        // call function
        cl::Kernel kernel=cl::Kernel(program,func.c_str());
        kernel.setArg(0,bufA);
        kernel.setArg(1,bufB);
        kernel.setArg(2,global);
        global=ceil((float)global/local)*local;
        if(local!=0){
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NDRange(local));
        }
        else{
            queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(global),cl::NullRange);
        }
        queue.finish();
        // read results
        queue.enqueueReadBuffer(bufB,CL_TRUE,0,sizeof(T2)*Ressize,Result.data());
        return Result;

    }

private:
    void ReadKernel();
    void KernelsDeclaration();

    std::vector<cl::Platform> all_platforms;
    cl::Platform default_platform;
    std::vector<cl::Device> all_devices;
    cl::Device default_device;
    cl::Context MainContext;
    cl::Program::Sources sources;
    cl::Program program;
    cl::CommandQueue queue;

    int defaultplatind;
    int defaultdevind;
    std::vector<size_t> maxworkitems;
    size_t maxworkgroups;
    size_t maxcomputeunits;

signals:
    void ConsoleLog(QString text);
};

#endif // OPENCLINTERFACE_H
