#ifndef HMMSETTINGS_H
#define HMMSETTINGS_H

#include <QDialog>

namespace Ui {
class HMMSettings;
}

class HMMSettings : public QDialog
{
    Q_OBJECT

public:
    explicit HMMSettings(double percent,int maxvec,double Crosspc,bool silence,QWidget *parent = 0);
    ~HMMSettings();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::HMMSettings *ui;
};

#endif // HMMSETTINGS_H
