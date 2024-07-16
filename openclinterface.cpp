#include "openclinterface.h"

OpenCLInterface::OpenCLInterface()
{

    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        emit ConsoleLog("No platforms found. Check OpenCL installation!\n");
        return;
    }
    defaultplatind=1;
    defaultdevind=0;
    default_platform=all_platforms[1];
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        emit ConsoleLog("Not availiable device for this platform!\n");
        return;
    }
    default_device=all_devices[0];
}

void OpenCLInterface::ReadKernel(){
    char *source_str;
    size_t source_size;
    QFile file(":/OpenCL/kernels.cl");
    if (!file.open(QFile::ReadOnly)) {
        emit ConsoleLog("Failed to load Kernel");
        return;
    }


    source_size = file.size();
    source_str = (char*)malloc(source_size + 1);
    source_str[source_size] = '\0';
    file.read(source_str,source_size);
    file.close();
    sources.push_back({source_str,source_size+1});
}


bool OpenCLInterface::InitiallizeOpenCL(){
    MainContext=cl::Context(default_device);
    sources=cl::Program::Sources();
    ReadKernel();


    program=cl::Program(MainContext,sources);
    if(program.build({default_device})!=CL_SUCCESS){
        emit ConsoleLog("Error building: "+QString::fromStdString((std::string)program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)));
        return false;
    }
    queue= cl::CommandQueue(MainContext,default_device);
    default_device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS,&maxcomputeunits);
    default_device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS,&maxworkgroups);
    default_device.getInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES,&maxworkitems);
    return true;
}


std::vector<QString> OpenCLInterface::getplatforms(){
    std::vector<QString> plats;
    for (int i=0;i<all_platforms.size();i++){
        plats.push_back(QString::fromStdString(all_platforms[i].getInfo<CL_PLATFORM_NAME>()));
    }
    return plats;
}

std::vector<QString> OpenCLInterface::getdevices(){
    std::vector<QString> devs;
    for (int i=0;i<all_devices.size();i++){
        devs.push_back(QString::fromStdString(all_devices[i].getInfo<CL_DEVICE_NAME>()));
    }
    return devs;
}

QString OpenCLInterface::getDeviceName(){
    return QString::fromStdString(all_devices[defaultdevind].getInfo<CL_DEVICE_NAME>());
}

int OpenCLInterface::getdefaultplatform(){
    return defaultplatind;
}

int OpenCLInterface::getdefaultdevice(){
    return defaultdevind;
}

void OpenCLInterface::setdefaultplatform(int plat){
    default_platform=all_platforms[plat];
    defaultdevind=0;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        emit ConsoleLog("No t availiable device for this platform!\n");
        return;
    }
    default_device=all_devices[0];
}

void OpenCLInterface::setdefaultdevice(int dev){
    default_device=all_devices[dev];
}


std::vector<std::vector<double>> OpenCLInterface::ViterbiStep1(std::vector<std::vector<double>>obvec,int states,std::vector<std::vector<double>> means,std::vector<std::vector<double>>sigmas,std::vector<double> priors,std::vector<double> Dets,std::vector<int>sumindex,double avgenergy){
    int M= obvec.size();
    int N=states;
    int vec=obvec[0].size();
    std::vector<double> m;
    std::vector<double> s;
    for(int i=0;i<means.size();i++){
        for(int j=0;j<vec;j++){
            m.push_back(means[i][j]);
            s.push_back(sigmas[i][j]);
        }
    }
    std::vector<double> ovec;
    for(int i=0;i<M;i++){
        for(int j=0;j<vec;j++){
            ovec.push_back(obvec[i][j]);
        }
    }
    std::vector<double> Result= std::vector<double>(N*M);
    cl::Buffer bufO(MainContext,CL_MEM_READ_ONLY,sizeof(double)*ovec.size());
    cl::Buffer bufM(MainContext,CL_MEM_READ_ONLY,sizeof(double)*m.size());
    cl::Buffer bufS(MainContext,CL_MEM_READ_ONLY,sizeof(double)*s.size());
    cl::Buffer bufP(MainContext,CL_MEM_READ_ONLY,sizeof(double)*priors.size());
    cl::Buffer bufD(MainContext,CL_MEM_READ_ONLY,sizeof(double)*Dets.size());
    cl::Buffer bufSI(MainContext,CL_MEM_READ_ONLY,sizeof(int)*sumindex.size());
    cl::Buffer bufR(MainContext,CL_MEM_WRITE_ONLY,sizeof(double)*Result.size());

    queue.enqueueWriteBuffer(bufO,CL_FALSE,0,sizeof(double)*ovec.size(),ovec.data());
    queue.enqueueWriteBuffer(bufM,CL_FALSE,0,sizeof(double)*m.size(),m.data());
    queue.enqueueWriteBuffer(bufS,CL_FALSE,0,sizeof(double)*s.size(),s.data());
    queue.enqueueWriteBuffer(bufP,CL_FALSE,0,sizeof(double)*priors.size(),priors.data());
    queue.enqueueWriteBuffer(bufD,CL_FALSE,0,sizeof(double)*Dets.size(),Dets.data());
    queue.enqueueWriteBuffer(bufSI,CL_FALSE,0,sizeof(int)*sumindex.size(),sumindex.data());

    cl::Kernel kernel=cl::Kernel(program,"ViterbiStep1");
    kernel.setArg(0,bufO);
    kernel.setArg(1,bufM);
    kernel.setArg(2,bufS);
    kernel.setArg(3,bufP);
    kernel.setArg(4,bufD);
    kernel.setArg(5,bufSI);
    kernel.setArg(6,M);
    kernel.setArg(7,vec);
    kernel.setArg(8,bufR);
    kernel.setArg(9,avgenergy);
    cl_int error;

    if(M>maxworkitems[0]){
        error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(N*maxworkitems[0]),cl::NDRange(maxworkitems[0]));
    }
    else{
        error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(N*M),cl::NDRange(M));
    }
    error=queue.finish();
    // read results
    error=queue.enqueueReadBuffer(bufR, CL_TRUE, 0, sizeof(double)*Result.size(),Result.data());
    std::vector<std::vector<double>> result2return;
    for(int i=0;i<M;i++){
        std::vector<double>current;
        current.clear();
        for(int j=0;j<N;j++){
            current.push_back(Result[j*M+i]);
        }
        result2return.push_back(current);
    }

    return result2return;
}


std::vector<int> OpenCLInterface::ViterbiStep2(std::vector<std::vector<double>>Matrix,std::vector<double> Initials,std::vector<std::vector<double>>Transitions,int states,int seqsize){
    int N=states;
    int M=seqsize;
    std::vector<double> Mat;
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            Mat.push_back(Matrix[i][j]);
        }
    }
    std::vector<double> Tr;
    for(int i=0;i<N;i++){
        for(int j=0;j<N;j++){
            Tr.push_back(Transitions[i][j]);
        }
    }
    std::vector<int> Result= std::vector<int>(M);

    cl::Buffer bufM(MainContext,CL_MEM_READ_WRITE,sizeof(double)*Mat.size());
    cl::Buffer bufI(MainContext,CL_MEM_READ_ONLY,sizeof(double)*Initials.size());
    cl::Buffer bufT(MainContext,CL_MEM_READ_ONLY,sizeof(double)*Tr.size());
    cl::Buffer bufR(MainContext,CL_MEM_WRITE_ONLY,sizeof(int)*Result.size());
    cl::Buffer Indexes(MainContext,CL_MEM_READ_WRITE,sizeof(ushort)*N*M);
    cl::LocalSpaceArg Colbuf=cl::Local(sizeof(double)*N);

    queue.enqueueWriteBuffer(bufM,CL_FALSE,0,sizeof(double)*Mat.size(),Mat.data());
    queue.enqueueWriteBuffer(bufI,CL_FALSE,0,sizeof(double)*Initials.size(),Initials.data());
    queue.enqueueWriteBuffer(bufT,CL_FALSE,0,sizeof(double)*Tr.size(),Tr.data());

    cl::Kernel kernel=cl::Kernel(program,"ViterbiStep2");
    kernel.setArg(0,bufM);
    kernel.setArg(1,bufI);
    kernel.setArg(2,bufT);
    kernel.setArg(3,bufR);
    kernel.setArg(4,N);
    kernel.setArg(5,M);
    kernel .setArg(6,Indexes);
    kernel .setArg(7,Colbuf);
    cl_int error;

    error=queue.enqueueNDRangeKernel(kernel,cl::NullRange,cl::NDRange(N),cl::NDRange(N));
    if(error!=0){
        emit ConsoleLog("Step2 kernel "+QString::number(error));
    }
    error=queue.finish();
    error=queue.enqueueReadBuffer(bufR, CL_TRUE, 0, sizeof(int)*Result.size(),Result.data());
    error=queue.enqueueReadBuffer(bufM, CL_TRUE, 0, sizeof(int)*Mat.size(),Mat.data());
    std::vector<std::vector<double>> result2return;
    for(int i=0;i<M;i++){
        std::vector<double>current;
        current.clear();
        for(int j=0;j<N;j++){
            current.push_back(Mat[j*M+i]);
        }
        result2return.push_back(current);
    }
    return Result;
}
