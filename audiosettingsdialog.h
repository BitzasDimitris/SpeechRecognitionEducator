#ifndef AUDIOSETTINGSDIALOG_H
#define AUDIOSETTINGSDIALOG_H

#include <QDialog>



namespace Ui {
class AudioSettingsDialog;
}

class AudioSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AudioSettingsDialog(int sr,QWidget *parent = 0);
    ~AudioSettingsDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::AudioSettingsDialog *ui;
};

#endif // AUDIOSETTINGSDIALOG_H
