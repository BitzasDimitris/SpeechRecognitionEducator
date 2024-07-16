#ifndef DIRECTORYRUNNER_H
#define DIRECTORYRUNNER_H

#include "WavFile.h"
#include "phonemeparser.h"
#include "phonemesLibrary.h"
#include "threadhandler.h"
#include "wavfilesholder.h"
#include "openclinterface.h"
#include <QDir>
#include <QStringList>
#include <Qthread>
#include <QWaitCondition>
#include <QMutex>


class DirectoryRunner: public QThread
{
    Q_OBJECT
public:
    explicit DirectoryRunner(QWidget *parent = 0);
    ~DirectoryRunner();
    void Initialize(int,int,int,QString,WavFilesHolder* wfh,ThreadHandler* thh,PhonemesLibrary* pl,bool ocl,OpenCLInterface* oclint);
    void StartProcess();


private:
    QString directory;
    QDir rootdir;
    QStringList dirlist;

    int framesize;
    int framestep;
    int SampleRate;

    bool opencl;
    bool ended=false;
    QWaitCondition Threadsdone;
    QMutex mutex;

    WavFilesHolder* wfholder;
    ThreadHandler* threadh;
    PhonemesLibrary* phnlib;
    OpenCLInterface* opclint;


    void ProcessDir(QString dir);

protected:
    void run() Q_DECL_OVERRIDE;

private slots:
    void on_threads_completed();
signals:
    void WF2show();
    void ConsoleLog(QString text);
    void Progress(int val);


};

#endif // DIRECTORYRUNNER_H
