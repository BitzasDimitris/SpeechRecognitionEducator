#ifndef THREADHANDLER_H
#define THREADHANDLER_H
#include "wavfilesholder.h"
#include "WavFile.h"
#include <Qthread>
#include <QMutex>
#include <QMutexLocker>
#include <QEventLoop>


class ThreadHandler: public QThread
{
    Q_OBJECT
public:

    explicit ThreadHandler(QWidget *parent = 0):mtx(),Concurrentthreads(0){};

    void initialize(WavFilesHolder* wfh,bool opencl);
    void updateTotalThreads(int totalthreads);
    void StartProcess();
    ~ThreadHandler();


private:
    int Concurrentthreads=0;
    int MaxSimultaneousThreads=8;
    int ThreadsDone=0;
    int TotalThreads2beCreated=0;
    WavFilesHolder* wfholder; 
    QMutex mtx;

protected:
    void run() Q_DECL_OVERRIDE;

signals:
    void ConsoleLog(QString text);
    void Progress(int val);
    void ThreadsFinished(); 
    void TimingReport(int);

public slots:
    void ThreadFinished(int id,int time);
};

#endif // THREADHANDLER_H
