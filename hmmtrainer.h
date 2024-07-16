#ifndef HMMTRAINER_H
#define HMMTRAINER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QWaitCondition>
#include <QMutex>
#include <vector>
#include "mfccholder.h"
#include "phonemesLibrary.h"
#include "openclinterface.h"
#include "mog.h"

class HMMTrainer : public QThread
{
    Q_OBJECT
public:
    explicit HMMTrainer(QWidget *parent = 0);
    ~HMMTrainer();
    void Initialize(MFCCHolder* ,PhonemesLibrary*,int fsize,int fstep,int SR,double pc,int maxvec,double Crosspc,bool silence,OpenCLInterface * opc,bool cl);
    void StartProcess();
    void PauseProcess();
    void CancelProcess();
    void ResumeProcess();
    void FillMatrices();
    std::vector<int> Viterbi(std::vector<std::vector<double >> ObVec);
    std::vector<int> ViterbiCL(std::vector<std::vector<double >> ObVec);
    std::vector<int> InitialAssignment(std::vector<std::vector<double>> ObVec,std::vector<int* > States,std::vector<int > StateAssignments);
    void SegmentalK_means(std::vector<std::vector<double>> ObVec,std::vector<int* > States,std::vector<int > *StateAssignments,float percentage);
    bool CheckClusters(std::vector<std::vector<double>> ObVec,std::vector<int* > States,std::vector<int > *StateAssignments,int * wrongs);
    void SaveFile(QString filename);

protected:
    void run() Q_DECL_OVERRIDE;
private:
    bool converged=false;
    int converge=0;
    std::vector<int> last5;
    int States;
    int ObservationSize;
    int framesize;
    int framestep;
    int SampleRate;
    double percent;
    double Crosspercent;
    double avgenergy=-1;
    int MaxVecs;
    int mintime=-1;
    int maxtime=-1;
    int AVGtime;
    bool mogsInitiated=false;
    bool Modelsilence;
    std::vector<double> Initials;
    std::vector<std::vector<double> > Transitions;
    std::vector<MoG> Observations;
    MFCCHolder* mfccholder;
    PhonemesLibrary* phnlib;
    QWaitCondition Paused;
    QMutex mutex;
    bool paused=false;
    bool canceled=false;
    bool opencl=false;
    OpenCLInterface *opclint;
    std::vector<std::vector<int>> ConfusionMatrix;

signals:
    void TrainingDone();
    void ConsoleLog(QString text);
    void Progress(int val);
    void Progress2(int val);
    void TimingReport(int time);
};

#endif // HMMTRAINER_H
