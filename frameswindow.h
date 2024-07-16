#ifndef FRAMESWINDOW_H
#define FRAMESWINDOW_H

#include <QDialog>
#include "WavFile.h"

namespace Ui {
class FramesWindow;
}

class FramesWindow : public QDialog
{
    Q_OBJECT

public:
    explicit FramesWindow(WavFile* wf,int,QWidget *parent = 0);
    ~FramesWindow();
    void PlotFile(int i);
private slots:
    void on_Frame2Show_valueChanged(int arg1);

private:
    WavFile* wfile;
    int Framesnum;
    int Choice;
    Ui::FramesWindow *ui;

};

#endif // FRAMESWINDOW_H
