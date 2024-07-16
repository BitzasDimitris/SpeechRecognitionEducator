#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QDialog>
#include "WavFile.h"

namespace Ui {
class WaveForm;
}

class WaveForm : public QDialog
{
    Q_OBJECT

public:
    explicit WaveForm(WavFile* w, QWidget *parent = 0);
    explicit WaveForm(WavFile* w,int SampleRate, QWidget *parent = 0);
    ~WaveForm();

private:
    WavFile* wfile;
    Ui::WaveForm *ui;
    void PlotFile();
    void PlotFile(int);
};

#endif // WAVEFORM_H
