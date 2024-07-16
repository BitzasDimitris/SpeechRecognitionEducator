#ifndef MFCCDIALOG_H
#define MFCCDIALOG_H

#include <QDialog>
#include "waveform.h"
#include "WavFile.h"
#include "frameswindow.h"
#include <qcustomplot.h>

namespace Ui {
class MFCCDialog;
}

class MFCCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MFCCDialog(WavFile* wf,QWidget *parent = 0);

    ~MFCCDialog();

private slots:
    void on_WaveFormButton_clicked();

    void on_Waveformafterlowpass_clicked();

    void on_FFTFrames_clicked();

    void on_MelFrames_clicked();

    void on_HFrames_clicked();

    void on_Frames_clicked();

    void on_LogMelFrames_clicked();

    void on_DCTFrames_clicked();

private:
    Ui::MFCCDialog *ui;
    WavFile* wfile;
};

#endif // MFCCDIALOG_H
