#include "hmmsettings.h"
#include "ui_hmmsettings.h"
#include "mainwindow.h"

HMMSettings::HMMSettings(double percent,int maxvec,double Crosspc,bool silence,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HMMSettings)
{
    ui->setupUi(this);
    ui->QualitySlider->setValue((int)(percent*100));
    ui->QualitySpinner->setValue((int)(percent*100));
    ui->MaxVecSpinner->setValue(maxvec);
    ui->CrossSpinner->setValue((int)(Crosspc*100));
    ui->SilenceBox->setChecked(silence);
    connect(ui->QualitySlider,SIGNAL(valueChanged(int)),ui->QualitySpinner,SLOT(setValue(int)));
    connect(ui->QualitySpinner,SIGNAL(valueChanged(int)),ui->QualitySlider,SLOT(setValue(int)));
}

HMMSettings::~HMMSettings()
{
    delete ui;


}


void HMMSettings::on_buttonBox_accepted()
{
    MainWindow* parent = qobject_cast<MainWindow*>(this->parent());
    if(parent==0)return;
    parent->on_HMM_Settings_Changes((double)ui->QualitySpinner->value()/100.0,ui->MaxVecSpinner->value(),(double)ui->CrossSpinner->value()/100.0,ui->SilenceBox->isChecked());
}
