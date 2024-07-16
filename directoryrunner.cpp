#include "directoryrunner.h"

DirectoryRunner::DirectoryRunner(QWidget *parent)
{

}

DirectoryRunner::~DirectoryRunner()
{

}

void DirectoryRunner::Initialize(int fsize,int fstep,int SR,QString dir,WavFilesHolder* wfh,ThreadHandler* thh,PhonemesLibrary* pl,bool ocl,OpenCLInterface* oclint){
    framesize=fsize*SR/1000;
    framestep=fstep*SR/1000;
    SampleRate=SR;
    directory=dir;
    rootdir=QDir(directory);
    rootdir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    dirlist=QStringList(rootdir.entryList());
    wfholder=wfh;
    threadh=thh;
    phnlib=pl;
    opencl=ocl;
    opclint=oclint;
    ended=false;
    emit ConsoleLog("Root Folder: "+directory);
    emit ConsoleLog("Total " + QString::number(dirlist.count()) +" directories");
    emit ConsoleLog("Ready for Initialization.");
    emit Progress(0);
    connect(threadh,SIGNAL(ThreadsFinished()),this,SLOT(on_threads_completed()));
}

void DirectoryRunner::StartProcess(){
    start();
    threadh->StartProcess();
}


void DirectoryRunner::run(){
    QTime time;
    time.start();
    threadh->updateTotalThreads(dirlist.count());
    for(int i=0;i<dirlist.count();i++){
        if(dirlist[i]!="."&&dirlist[i]!="..")
        ProcessDir(dirlist[i]);
    }

    mutex.lock();
    while(!ended){
        Threadsdone.wait(&mutex);
    }
    int timing=time.elapsed();
    if(timing<1000){
        emit ConsoleLog("Total Time: "+QString::number(timing)+" ms");
    }
    else if (timing<60000){
       emit ConsoleLog("Total Time: "+QString::number(timing/1000)+" s + "+QString::number(timing%1000)+" ms");
    }
    else{
        emit ConsoleLog("Total Time: "+QString::number(timing/60000)+" m + "+QString::number((timing/1000)%60)+" s + "+QString::number(timing%1000)+" ms");
    }
    emit WF2show();
    mutex.unlock();

}


void DirectoryRunner::ProcessDir(QString dir){
    QDir curdir=rootdir;
    curdir.cd(dir);
    curdir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    curdir.setNameFilters(QStringList()<<"*.wav");
    QString curwavfile,curphnfile,curwrdfile;
    if(curdir.entryList().count()>=1){
        curwavfile=curdir.path()+"/"+curdir.entryList()[0];
    }
    else{
        emit ConsoleLog(".wav file not found in folder :");
        emit ConsoleLog(dir);
        threadh->updateTotalThreads(-1);
        return;
    }
    curdir.setNameFilters(QStringList()<<"*.phn");
    if(curdir.entryList().count()>=1){
        curphnfile=curdir.path()+"/"+curdir.entryList()[0];
    }
    else{
        emit ConsoleLog(".phn file not found in folder :");
        emit ConsoleLog(dir);
        threadh->updateTotalThreads(-1);
        return;
    }

    curdir.setNameFilters(QStringList()<<"*.wrd");
    if(curdir.entryList().count()>=1){
        curwrdfile=curdir.path()+"/"+curdir.entryList()[0];
    }
    else{
        //emit ConsoleLog(".wrd file not found in folder :");
        //emit ConsoleLog(dir);
        curwrdfile="";
        //threadh->updateTotalThreads(-1);
        //return;
    }

    WavFile* curwfile=new WavFile();
    if(!curwfile->Initiallize(framesize,framestep,SampleRate,curwavfile,opencl,opclint)){
        emit ConsoleLog("Error while trying to load "+curwavfile+" in folder :");
        emit ConsoleLog(dir);
        threadh->updateTotalThreads(-1);
    }
    else{
        PhonemeParser* curpfile=new PhonemeParser(phnlib,framesize);
        if(!curpfile->StartProcess(curphnfile,curwrdfile)){
            emit ConsoleLog("Error while trying to load "+curphnfile+" in folder :");
            emit ConsoleLog(dir);
            threadh->updateTotalThreads(-1);
        }
        else{
            wfholder->InsertEntry(curwfile,curpfile);
        }
    }
}

void DirectoryRunner::on_threads_completed(){
    ended=true;
    Threadsdone.wakeAll();
}

