#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "framesizedialog.h"
#include "audiosettingsdialog.h"
#include "qfiledialog.h"
#include "QMessageBox"
#include "wavfilesholder.h"
#include "threadhandler.h"
#include "directoryrunner.h"
#include "phonemeslibrary.h"
#include "hmmtrainer.h"
#include "mfccdialog.h"
#include "openclinterface.h"
#include "openclsettings.h"
#include "hmmsettings.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void on_Frame_Settings_Changed(int fsize,int osize);
    void on_Audio_Settings_Changed(int SampleRate);
    void on_HMM_Settings_Changes(double percent,int maxvec,double Crosspc,bool silence);
    void reset();
    void Defines();

private slots:


    void on_actionOpen_triggered();

    void on_actionFrame_Settings_triggered();

    void on_StartButton_clicked();

    void on_actionAudio_Settings_triggered();

    void on_wfiles_ready();

    void on_actionOpenCL_Settings_triggered();

    void on_openclbox_stateChanged(int arg1);

    void on_ResetButton_clicked();

    void on_actionOpen_File_triggered();

    void TimingReport(int time);

    void on_actionHMM_Settings_triggered();

    void TrainingDone();

    //void on_actionSave_Model_triggered();

private:
    Ui::MainWindow *ui;
    QString directory;
    int Framesize =25;
    int Overlapping=10;
    int SampleRate=16000;
    DirectoryRunner* dirrunner;
    WavFile* wfile;
    WavFile* test;
    WavFilesHolder* wfholder;
    MFCCHolder* mfccholder;
    ThreadHandler* threadh;
    PhonemesLibrary* phnlib;
    HMMTrainer* HTrainer;
    OpenCLInterface* opclint;
    bool opencl=false;
    bool Modelsilence=false;
    bool loaded=false;
    bool Initialized=false;
    bool trained=false;
    bool Started=false;
    bool Paused=false;
    int avgtime;
    int times;
    double KCriticalPercent=0.75;
    int MaxVecs=0;
    double Crosspercent=0.0;


};

#endif // MAINWINDOW_H
