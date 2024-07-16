#ifndef WAVFILE_H
#define WAVFILE_H
#include "qcustomplot.h"
#include <stdlib.h>
#include <stdio.h>
#include <cstdio>
#include <string>
#include <vector>
#include <math.h>
#include <QThread>
#include "SoundDataOperators.h"
#include <QString>
#include "openclinterface.h"
#ifndef WAVESTRUCTS_H
#define WAVESTRUCTS_H
#include "wavestructs.h"
#endif // WAVESTRUCTS_H


class WavFile : public QThread
{
    Q_OBJECT

public:

    RIFF_Header Rheader;
    WAVE_Format Wformat;
    WAVE_Data Wdata;
    uchar * data;

    explicit WavFile(QWidget *parent = 0,bool destroy=true);
    ~WavFile();
    void GetWaveForm(QVector<double>* x,QVector<double>* y,float xstep,float *size);
    void GetWaveForm(int SampleRate,QVector<double>* x,QVector<double>* y,float xstep,float *size);
    void GetFrame(int i,QVector<double>* x,QVector<double>* y,float xstep,float *size);
    void GetPFrame(int i,QVector<double>* x,QVector<double>* y,float xstep,float *size);
    void GetHFrame(int i,QVector<double>* x,QVector<double>* y,float xstep,float *size);
    void GetFFrame(int i,QVector<double>* x,QVector<double>* y,float *size);
    void GetMFrame(int i,QVector<double>* x,QVector<double>* y,float *size);
    void GetMLFrame(int i,QVector<double>* x,QVector<double>* y,float *size);
    void GetMLDFrame(int i,QVector<double>* x,QVector<double>* y,float *size);
    void GetMDFrame(int i,QVector<double>* x,QVector<double>* y,float *size);
    void GetMFCC(std::vector<std::vector<double>> *MFCCFrames);
    int GetFrameNumber();
    bool Initiallize(int,int,int,QString,bool,OpenCLInterface*);
    void StartProcess();
    void freememory();
    int GetId();
    void SetId(int);
protected:
    void run() Q_DECL_OVERRIDE;
private:
    void CreateFramesCL();
    void FFTCL();
    void MELCL();
    void FCTCL();
    void DeltasCL();

    void CreateWaveForm();
    void LowpassFilter();// to do
    void CreateFrames();
    void Preemphasis();
    void Hamming();
    void FFT();
    void FCT();
    void MEL();
    void Logs();
    void Deltas();
    bool LoadWavFile();
    std::vector<double> Waveform;
    std::vector<std::vector<double> > Frames;
    std::vector<std::vector<double> > PFrames;
    std::vector<std::vector<double> > HFrames;
    std::vector<std::vector<double> > FFrames;
    std::vector<std::vector<double> > MFrames;
    std::vector<std::vector<double> > MLFrames;
    std::vector<std::vector<double> > MLDFrames;
    std::vector<std::vector<double> > MDFrames;
    std::vector<int> inits;
    std::vector<double> mids;
    int framesize;
    int framestep;
    float SampleRate;
    int framesnum;
    int padding;
    int id;
    std::string filename;
    bool opencl;
    OpenCLInterface *opclint;

signals:
    void Threadfinished(int id,int time);

};

#endif  //WAVFILE_H
