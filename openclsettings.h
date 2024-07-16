#ifndef OPENCLSETTINGS_H
#define OPENCLSETTINGS_H

#include <QDialog>
#include "openclinterface.h"
#include <vector>
#include <QString>

namespace Ui {
class OpenClSettings;
}

class OpenClSettings : public QDialog
{
    Q_OBJECT

public:
    explicit OpenClSettings(OpenCLInterface* opci,QWidget *parent = 0);
    ~OpenClSettings();

private slots:
    void on_PlatformDropdown_currentIndexChanged(int index);

    void on_DeviceDropdown_currentIndexChanged(int index);

private:
    Ui::OpenClSettings *ui;
    OpenCLInterface* opclint;
    std::vector<QString> platforms;
    std::vector<QString> devices;
    int platselected;
    int deviceselected;
    bool initiated=false;
};

#endif // OPENCLSETTINGS_H
