#include "threadhandler.h"

ThreadHandler::~ThreadHandler(){

}


void ThreadHandler::initialize(WavFilesHolder* wfh,bool opencl){
    int maxthreads=QThread::idealThreadCount();
    if(opencl){
        maxthreads=2;
    }
    maxthreads=2;
    if(maxthreads>0){
        MaxSimultaneousThreads=maxthreads;
    }

    wfholder=wfh;

}

void ThreadHandler::StartProcess(){
    start();
    emit TimingReport(-1);
}

void ThreadHandler::run(){
    while(true){
        WavFile* wf;
        while(wfholder->count()<=Concurrentthreads){
            {
                QEventLoop loop;
                loop.connect(wfholder, SIGNAL(HasElements()), SLOT(quit()));
                loop.connect(this, SIGNAL(ThreadsFinished()), SLOT(quit()));
                loop.exec();
            }
        }
        if(Concurrentthreads<MaxSimultaneousThreads&&wfholder->count()>0){
            int id;
            wf=wfholder->GetWfile(Concurrentthreads,&id);
            connect(wf,SIGNAL(Threadfinished(int,int)),this,SLOT(ThreadFinished(int,int)));
            wf->StartProcess();
            QMutexLocker ml(&mtx);
            Concurrentthreads++;
            ml.unlock();
        }
        QMutexLocker ml(&mtx);
        if(TotalThreads2beCreated<=ThreadsDone){
            break;
        }
        ml.unlock();

    }

}


void ThreadHandler::updateTotalThreads(int totalthreads){
    TotalThreads2beCreated+=totalthreads;
}


void ThreadHandler::ThreadFinished(int id,int time){
    QMutexLocker ml(&mtx);
    if(wfholder->DeleteEntry(id)){
        ThreadsDone++;
        Concurrentthreads--;
        if(time<1000){
            emit ConsoleLog("Time Elapsed: "+QString::number(time)+" ms");
        }
        else if (time<60000){
            emit ConsoleLog("Time Elapsed: "+QString::number(time/1000)+" s + "+QString::number(time%1000)+" ms");
        }
        else{
            emit ConsoleLog("Time Elapsed: "+QString::number(time/60000)+" m + "+QString::number((time/1000)%60)+" s + "+QString::number(time%1000)+" ms");
        }
        emit TimingReport(time);
        if(TotalThreads2beCreated>0){
            emit Progress((int)(100*(float) ThreadsDone/TotalThreads2beCreated));
        }
        if(TotalThreads2beCreated<=ThreadsDone){
            emit ThreadsFinished();
        }
    }
    ml.unlock();

}
