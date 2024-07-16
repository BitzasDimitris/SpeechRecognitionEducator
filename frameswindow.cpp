#include "frameswindow.h"
#include "ui_frameswindow.h"

FramesWindow::FramesWindow(WavFile* wf,int c,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FramesWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    wfile=wf;
    Framesnum=wfile->GetFrameNumber();
    ui->Frame2Show->setValue(1);
    ui->Frame2Show->setMinimum(1);
    ui->Frame2Show->setMaximum(Framesnum);
    Choice=c;
    PlotFile(ui->Frame2Show->value()-1);
}

FramesWindow::~FramesWindow()
{
    delete ui;
}


void FramesWindow::PlotFile(int i){
    QVector<double> t,amp;
    float size;
    switch(Choice){
    case 0:
        wfile->GetFrame(i,&t,&amp,1.0f,&size);
        break;
    case 1:
        wfile->GetPFrame(i,&t,&amp,1.0f,&size);
        break;
    case 2:
        wfile->GetHFrame(i,&t,&amp,1.0f,&size);
        break;
    case 3:
        wfile->GetFFrame(i,&t,&amp,&size);
        break;
    case 4:
        wfile->GetMLFrame(i,&t,&amp,&size);
        break;
    case 5:
        wfile->GetMLDFrame(i,&t,&amp,&size);
        break;
    case 6:
        wfile->GetMDFrame(i,&t,&amp,&size);
        break;
    }
    if(ui->FramePlot->graphCount()>0){
        ui->FramePlot->removeGraph(1);
    }
    ui->FramePlot->addGraph();
    ui->FramePlot->graph(0)->setData(t,amp);
    ui->FramePlot->xAxis->setRange(0,size);
    ui->FramePlot->yAxis->setRange(-1.2,1.2);
    if(Choice==3||Choice==4){
        ui->FramePlot->yAxis->setRange(0,1.5);
    }
    if(Choice>=5){
        ui->FramePlot->yAxis->setRange(-20,100);
    }
    ui->FramePlot->replot();

    ui->FramePlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->FramePlot->axisRect()->setRangeDragAxes(ui->FramePlot->xAxis,ui->FramePlot->yAxis);
    ui->FramePlot->axisRect()->setRangeZoomAxes(ui->FramePlot->xAxis,NULL);
    ui->sizelabel->setText("Size: "+QString::number(amp.size()));

}

void FramesWindow::on_Frame2Show_valueChanged(int arg1)
{
    ui->CurrentFrame->setText(QString::number(ui->Frame2Show->value()));
    PlotFile(ui->Frame2Show->value()-1);
}
