#include "framesizedialog.h"
#include "ui_framesizedialog.h"
#include <mainwindow.h>


FrameSizeDialog::FrameSizeDialog(int fsize,int osize,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FrameSizeDialog)
{
    ui->setupUi(this);
    ui->Framesizetxt->setText(QString::number(fsize));
    ui->Overlappingtxt->setText(QString::number(osize));
}

FrameSizeDialog::~FrameSizeDialog()
{
    delete ui;
}




void FrameSizeDialog::on_buttonBox_accepted()
{
    MainWindow* parent = qobject_cast<MainWindow*>(this->parent());
    if(parent==0)return;
    parent->on_Frame_Settings_Changed(ui->Framesizetxt->text().toInt(),ui->Overlappingtxt->text().toInt());
}
