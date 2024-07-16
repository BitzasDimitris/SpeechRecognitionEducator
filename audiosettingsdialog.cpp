#include "audiosettingsdialog.h"
#include "ui_audiosettingsdialog.h"
#include "mainwindow.h"

AudioSettingsDialog::AudioSettingsDialog(int sr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AudioSettingsDialog)
{
    ui->setupUi(this);

    ui->SampleRateBox->setValue(sr);
}

AudioSettingsDialog::~AudioSettingsDialog()
{
    delete ui;
}

void AudioSettingsDialog::on_buttonBox_accepted()
{
    MainWindow* parent = qobject_cast<MainWindow*>(this->parent());
    if(parent==0)return;
    parent->on_Audio_Settings_Changed(ui->SampleRateBox->value());
}
