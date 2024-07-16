#include "waveform.h"
#include "ui_waveform.h"

WaveForm::WaveForm(WavFile* w,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaveForm)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    wfile=w;
    PlotFile();
}

WaveForm::WaveForm(WavFile* w,int SapmleRate,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaveForm)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Window | Qt::WindowSystemMenuHint
                                | Qt::WindowMinimizeButtonHint
                                | Qt::WindowMaximizeButtonHint
                                | Qt::WindowCloseButtonHint;
    this->setWindowFlags(flags);
    wfile=w;
    PlotFile(SapmleRate);
}

WaveForm::~WaveForm()
{
    delete ui;
}
void WaveForm::PlotFile(int SampleRate){
    QVector<double> t,amp;
    float size;
    wfile->GetWaveForm(SampleRate,&t,&amp,1.0f,&size);
    ui->TimeDomain->addGraph();
    ui->TimeDomain->graph(0)->setData(t,amp);
    ui->TimeDomain->xAxis->setRange(0,size);
    ui->TimeDomain->yAxis->setRange(-1.2,1.2);
    ui->TimeDomain->replot();

    ui->TimeDomain->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->TimeDomain->axisRect()->setRangeDragAxes(ui->TimeDomain->xAxis,ui->TimeDomain->yAxis);
    ui->TimeDomain->axisRect()->setRangeZoomAxes(ui->TimeDomain->xAxis,NULL);
}


void WaveForm::PlotFile(){
    QVector<double> t,amp;
    float size;
    wfile->GetWaveForm(&t,&amp,1.0f,&size);
    ui->TimeDomain->addGraph();
    ui->TimeDomain->graph(0)->setData(t,amp);
    ui->TimeDomain->xAxis->setRange(0,size);
    ui->TimeDomain->yAxis->setRange(-1.2,1.2);
    ui->TimeDomain->replot();

    ui->TimeDomain->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->TimeDomain->axisRect()->setRangeDragAxes(ui->TimeDomain->xAxis,NULL);
    ui->TimeDomain->axisRect()->setRangeZoomAxes(ui->TimeDomain->xAxis,NULL);
}
