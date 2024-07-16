#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Defines();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::reset(){
    if(Started){
        Started=false;
        ui->StartButton->setText("START TRAINING");
    }
    else if(Initialized){
        Initialized=false;
        ui->StartButton->setText("INITIALIZE");
        ui->ResetButton->setEnabled(false);
    }
    Paused=false;

    ui->GUIConsole->clear();
    ui->TimingLabel->setText("");
}

void MainWindow::Defines(){
    mfccholder= new MFCCHolder();
    wfholder=new WavFilesHolder();
    wfholder->initialize(mfccholder);
    threadh=new ThreadHandler();

    phnlib= new PhonemesLibrary();
    dirrunner=new DirectoryRunner();
    HTrainer= new HMMTrainer();
    opclint = new OpenCLInterface();

    connect(opclint,SIGNAL(ConsoleLog(QString)),ui->GUIConsole,SLOT(appendPlainText(QString)));

    connect(dirrunner,SIGNAL(ConsoleLog(QString)),ui->GUIConsole,SLOT(appendPlainText(QString)));
    connect(dirrunner,SIGNAL(Progress(int)),ui->progressBar,SLOT(setValue(int)));
    connect(dirrunner,SIGNAL(WF2show()),this,SLOT(on_wfiles_ready()));

    connect(threadh,SIGNAL(Progress(int)),ui->progressBar_2,SLOT(setValue(int)));
    connect(threadh,SIGNAL(TimingReport(int)),this,SLOT(TimingReport(int)));
    connect(threadh,SIGNAL(ConsoleLog(QString)),ui->GUIConsole,SLOT(appendPlainText(QString)));

    connect(HTrainer,SIGNAL(ConsoleLog(QString)),ui->GUIConsole,SLOT(appendPlainText(QString)));
    connect(HTrainer,SIGNAL(Progress(int)),ui->progressBar,SLOT(setValue(int)));
    connect(HTrainer,SIGNAL(Progress2(int)),ui->progressBar_2,SLOT(setValue(int)));
    connect(HTrainer,SIGNAL(TimingReport(int)),this,SLOT(TimingReport(int)));
    connect(HTrainer,SIGNAL(TrainingDone()),this,SLOT(TrainingDone()));
}

void MainWindow::on_actionOpen_triggered()
{
    bool flag=false;
    if(Started||Initialized||trained){
        QMessageBox msgBox;
        msgBox.setText("Warning!");
        msgBox.setInformativeText("Are you sure you want to open another folder?\n You will lose all the Data builded so far.");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret=msgBox.exec();
        if(ret==QMessageBox::Cancel){
            flag=true;
        }
    }
    if(!flag){
        if(trained){
            ui->StartButton->setEnabled(true);
            ui->ResetButton->setEnabled(true);
        }
        Started=false;
        Initialized=false;
        Paused=false;
        trained=false;
        mfccholder->clear();
        wfholder->clear();
        phnlib->clear();
        ui->StartButton->setText("INITIALIZE");
        ui->ResetButton->setEnabled(false);
        ui->GUIConsole->clear();
        ui->FileLabel->setText("");
        directory=QFileDialog::getExistingDirectory(this,tr("Select Root Directory of Dataset"),"/home",QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);
        ui->FileLabel->setText("Directory Selected: "+directory);
        loaded=true;
    }


}



void MainWindow::on_actionFrame_Settings_triggered()
{
    FrameSizeDialog* frmsd=new FrameSizeDialog(Framesize,Overlapping,this);
    frmsd->show();
}

void MainWindow::on_Frame_Settings_Changed(int fsize,int osize){
    Framesize=fsize;
    Overlapping=osize;
    ui->GUIConsole->clear();
    ui->progressBar->setValue(0);
    if(Started||loaded){
        reset();
        Started=false;
        Initialized=false;
        Paused=false;
        mfccholder->clear();
        wfholder->clear();
        phnlib->clear();
        ui->StartButton->setText("INITIALIZE");
        ui->ResetButton->setEnabled(false);

    }
}

void MainWindow::on_Audio_Settings_Changed(int SR){
    SampleRate=SR;
    ui->GUIConsole->clear();
    ui->progressBar->setValue(0);
    if(Started||loaded){
        reset();
        Started=false;
        Initialized=false;
        Paused=false;
        mfccholder->clear();
        wfholder->clear();
        phnlib->clear();
        ui->StartButton->setText("INITIALIZE");
        ui->ResetButton->setEnabled(false);
    }
}


void MainWindow::on_StartButton_clicked()
{   if(Paused){
        HTrainer->ResumeProcess();
        ui->StartButton->setText("PAUSE TRAINING");
        Paused=false;
    }
    else if(Started){
        HTrainer->PauseProcess();
        ui->StartButton->setText("RESUME TRAINING");
        Paused=true;
    }
    else if(Initialized){
        ui->GUIConsole->clear();
        HTrainer->Initialize(mfccholder,phnlib,Framesize,Framesize-Overlapping,SampleRate,KCriticalPercent,MaxVecs,Crosspercent,Modelsilence,opclint,opencl);
        HTrainer->StartProcess();
        Started=true;
        ui->StartButton->setText("PAUSE TRAINING");
        ui->ResetButton->setText("CANCEL TRAINING");
    }
    else if(loaded){
        threadh->initialize(wfholder,opencl);
        dirrunner->Initialize(Framesize,Framesize-Overlapping,SampleRate,directory,wfholder,threadh,phnlib,opencl,opclint);
        ui->GUIConsole->clear();
        ui->progressBar->setValue(0);
        opclint->InitiallizeOpenCL();
        dirrunner->StartProcess();
        ui->ResetButton->setEnabled(true);
    }

}

void MainWindow::on_ResetButton_clicked()
{
    if(Started){
        HTrainer->CancelProcess();
        Paused=false;
        Started=false;
        ui->StartButton->setText("START TRAINING");
    }
    else if(Initialized){
        Paused=false;
        Started=false;
        Initialized=false;
        reset();
    }


}

void MainWindow::on_HMM_Settings_Changes(double percent,int maxvec,double Crosspc,bool silence){
    KCriticalPercent=percent;
    MaxVecs=maxvec;
    Crosspercent=Crosspc;
    Modelsilence=silence;
}

void MainWindow::on_actionAudio_Settings_triggered()
{
    AudioSettingsDialog* as=new AudioSettingsDialog(SampleRate,this);
    as->show();

}

void MainWindow::on_wfiles_ready(){

    Initialized=true;
    ui->StartButton->setText("START TRAINING");
}


void MainWindow::on_actionOpenCL_Settings_triggered()
{
    OpenClSettings* opc= new OpenClSettings(opclint,this);
    opc->show();
}


void MainWindow::on_openclbox_stateChanged(int arg1)
{
 (arg1>0)?opencl=true:opencl=false;
}



void MainWindow::on_actionOpen_File_triggered()
{
    ui->FileLabel->setText("");
    QString File=QFileDialog::getOpenFileName(this,tr("Open File"),"/home",tr("*.wav"));

    if(File!=NULL){
        ui->FileLabel->setText("File Opened: "+File);
        if(opencl){
            if(opclint->InitiallizeOpenCL()){
                test= new WavFile(this,false);
                test->Initiallize(Framesize*SampleRate/1000,(Framesize-Overlapping)*SampleRate/1000,SampleRate,File,opencl,opclint);
                test->StartProcess();
                test->wait();
                MFCCDialog* mfccdiag= new MFCCDialog(test);
                mfccdiag->show();
            }
        }
        else{
            test= new WavFile(this,false);
            test->Initiallize(Framesize*SampleRate/1000,(Framesize-Overlapping)*SampleRate/1000,SampleRate,File,opencl,opclint);
            test->StartProcess();
            test->wait();
            MFCCDialog* mfccdiag= new MFCCDialog(test);
            mfccdiag->show();
        }
    }

}

void MainWindow::TimingReport(int time){
    if(time<0){
        if(ui->TimingLabel->text()!=""){
            ui->GUIConsole->appendPlainText(ui->TimingLabel->text());
        }
        times=0;
        avgtime=0;
        ui->TimingLabel->setText("");
    }
    else{
        avgtime=round((times*avgtime+time)/(times+1.0));
        times++;
        if(avgtime<1000){
            ui->TimingLabel->setText("Average Time: "+QString::number(avgtime)+" ms");
        }
        else if (avgtime<60000){
           ui->TimingLabel->setText("Average Time: "+QString::number(avgtime/1000)+" s + "+QString::number(avgtime%1000)+" ms");
        }
        else{
            ui->TimingLabel->setText("Average Time: "+QString::number(avgtime/60000)+" m + "+QString::number((avgtime/1000)%60)+" s + "+QString::number(avgtime%1000)+" ms");
        }
    }
}

void MainWindow::on_actionHMM_Settings_triggered()
{
    HMMSettings* hmmsetdialog= new HMMSettings(KCriticalPercent,MaxVecs,Crosspercent,Modelsilence,this);
    hmmsetdialog->show();
}


void MainWindow::TrainingDone(){
    trained=true;
    ui->StartButton->setText("Done");
    ui->StartButton->setEnabled(false);
    ui->ResetButton->setEnabled(false);
    ui->actionSave_Model->setEnabled(true);

}


/*
void MainWindow::on_actionSave_Model_triggered()
{
    QString File=QFileDialog::getSaveFileName(this,tr("Save Model File"),"/home",tr("*.hmm"));
    if(File!=NULL){
        HTrainer->SaveFile(File);
    }
}
//*/
