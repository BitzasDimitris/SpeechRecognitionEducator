#ifndef FRAMESIZEDIALOG_H
#define FRAMESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class FrameSizeDialog;
}

class FrameSizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FrameSizeDialog(int fsize, int osize,QWidget *parent = 0);
    ~FrameSizeDialog();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::FrameSizeDialog *ui;

};

#endif // FRAMESIZEDIALOG_H
