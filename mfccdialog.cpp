#include "mfccdialog.h"
#include "ui_mfccdialog.h"

MFCCDialog::MFCCDialog(WavFile* wf,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MFCCDialog)
{
    ui->setupUi(this);
    wfile=wf;
}

MFCCDialog::~MFCCDialog()
{
    delete ui;
}



void MFCCDialog::on_WaveFormButton_clicked()
{

    WaveForm* wfw= new WaveForm(wfile);
    wfw->show();
}


void MFCCDialog::on_Waveformafterlowpass_clicked()
{

    WaveForm* wfw= new WaveForm(wfile,10000);
    wfw->show();

}


void MFCCDialog::on_FFTFrames_clicked()
{

    FramesWindow* fw=new FramesWindow(wfile,3);
    fw->show();

}

void MFCCDialog::on_MelFrames_clicked()
{

    FramesWindow* fw=new FramesWindow(wfile,4);
    fw->show();

}


void MFCCDialog::on_HFrames_clicked()
{
    FramesWindow* fw=new FramesWindow(wfile,2);
    fw->show();
}

void MFCCDialog::on_Frames_clicked()
{
    FramesWindow* fw=new FramesWindow(wfile,0);
    fw->show();
}

void MFCCDialog::on_LogMelFrames_clicked()
{
    FramesWindow* fw=new FramesWindow(wfile,5);
    fw->show();
}

void MFCCDialog::on_DCTFrames_clicked()
{
    FramesWindow* fw=new FramesWindow(wfile,6);
    fw->show();
}
