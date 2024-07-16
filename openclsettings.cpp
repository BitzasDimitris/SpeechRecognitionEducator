#include "openclsettings.h"
#include "ui_openclsettings.h"

OpenClSettings::OpenClSettings(OpenCLInterface* opci,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenClSettings)
{
    ui->setupUi(this);
    opclint=opci;
    platforms=opclint->getplatforms();
    platselected=opclint->getdefaultplatform();

    devices=opclint->getdevices();
    for(int i=0;i<platforms.size();i++){
        ui->PlatformDropdown->addItem(platforms[i]);
    }
    for(int i=0;i<devices.size();i++){
        ui->DeviceDropdown->addItem(devices[i]);
    }
    ui->PlatformDropdown->setCurrentIndex(platselected);
    deviceselected=opclint->getdefaultdevice();
    ui->DeviceDropdown->setCurrentIndex(deviceselected);
    initiated=true;
}

OpenClSettings::~OpenClSettings()
{
    delete ui;
}

void OpenClSettings::on_PlatformDropdown_currentIndexChanged(int index)
{
    if(!initiated){
        return;
    }
    if(index<0){
        return;
    }
    if(index!=platselected){
        platselected=index;
        opclint->setdefaultplatform(platselected);
         devices=opclint->getdevices();
         ui->DeviceDropdown->clear();
         for(int i=0;i<devices.size();i++){
             ui->DeviceDropdown->addItem(devices[i]);
         }
         deviceselected=opclint->getdefaultdevice();
         ui->DeviceDropdown->setCurrentIndex(deviceselected);
    }
}

void OpenClSettings::on_DeviceDropdown_currentIndexChanged(int index)
{
    if(!initiated){
        return;
    }
    if(index<0){
        return;
    }
    if(index!=deviceselected){
        deviceselected=index;
        opclint->setdefaultdevice(deviceselected);
    }
}
