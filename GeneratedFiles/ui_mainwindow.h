/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionFrame_Settings;
    QAction *actionOpen;
    QAction *actionHMM_Settings;
    QAction *actionAudio_Settings;
    QAction *actionOpenCL_Settings;
    QAction *actionOpen_File;
    QAction *actionSave_Model;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QLabel *FileLabel;
    QHBoxLayout *horizontalLayout;
    QCheckBox *openclbox;
    QLabel *TimingLabel;
    QPlainTextEdit *GUIConsole;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *ResetButton;
    QPushButton *StartButton;
    QProgressBar *progressBar;
    QProgressBar *progressBar_2;
    QMenuBar *menuBar;
    QMenu *menuSettings;
    QMenu *menuFile;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1229, 712);
        actionFrame_Settings = new QAction(MainWindow);
        actionFrame_Settings->setObjectName(QStringLiteral("actionFrame_Settings"));
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionHMM_Settings = new QAction(MainWindow);
        actionHMM_Settings->setObjectName(QStringLiteral("actionHMM_Settings"));
        actionAudio_Settings = new QAction(MainWindow);
        actionAudio_Settings->setObjectName(QStringLiteral("actionAudio_Settings"));
        actionOpenCL_Settings = new QAction(MainWindow);
        actionOpenCL_Settings->setObjectName(QStringLiteral("actionOpenCL_Settings"));
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        actionSave_Model = new QAction(MainWindow);
        actionSave_Model->setObjectName(QStringLiteral("actionSave_Model"));
        actionSave_Model->setEnabled(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setLayoutDirection(Qt::LeftToRight);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        FileLabel = new QLabel(centralWidget);
        FileLabel->setObjectName(QStringLiteral("FileLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(FileLabel->sizePolicy().hasHeightForWidth());
        FileLabel->setSizePolicy(sizePolicy1);
        FileLabel->setMaximumSize(QSize(16777215, 30));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        FileLabel->setFont(font);

        verticalLayout->addWidget(FileLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        openclbox = new QCheckBox(centralWidget);
        openclbox->setObjectName(QStringLiteral("openclbox"));

        horizontalLayout->addWidget(openclbox);

        TimingLabel = new QLabel(centralWidget);
        TimingLabel->setObjectName(QStringLiteral("TimingLabel"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(TimingLabel->sizePolicy().hasHeightForWidth());
        TimingLabel->setSizePolicy(sizePolicy2);
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(14);
        font1.setBold(true);
        font1.setWeight(75);
        TimingLabel->setFont(font1);

        horizontalLayout->addWidget(TimingLabel);


        verticalLayout->addLayout(horizontalLayout);

        GUIConsole = new QPlainTextEdit(centralWidget);
        GUIConsole->setObjectName(QStringLiteral("GUIConsole"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Ignored);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(GUIConsole->sizePolicy().hasHeightForWidth());
        GUIConsole->setSizePolicy(sizePolicy3);
        QFont font2;
        font2.setFamily(QStringLiteral("Arial Black"));
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        GUIConsole->setFont(font2);
        GUIConsole->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        GUIConsole->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        GUIConsole->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
        GUIConsole->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        verticalLayout->addWidget(GUIConsole);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        ResetButton = new QPushButton(centralWidget);
        ResetButton->setObjectName(QStringLiteral("ResetButton"));
        ResetButton->setEnabled(false);
        ResetButton->setMinimumSize(QSize(0, 50));

        horizontalLayout_2->addWidget(ResetButton);

        StartButton = new QPushButton(centralWidget);
        StartButton->setObjectName(QStringLiteral("StartButton"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(StartButton->sizePolicy().hasHeightForWidth());
        StartButton->setSizePolicy(sizePolicy4);
        StartButton->setMinimumSize(QSize(0, 50));

        horizontalLayout_2->addWidget(StartButton);


        verticalLayout->addLayout(horizontalLayout_2);

        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);

        verticalLayout->addWidget(progressBar);

        progressBar_2 = new QProgressBar(centralWidget);
        progressBar_2->setObjectName(QStringLiteral("progressBar_2"));
        progressBar_2->setValue(0);

        verticalLayout->addWidget(progressBar_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1229, 20));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QStringLiteral("menuSettings"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuSettings->addAction(actionAudio_Settings);
        menuSettings->addAction(actionFrame_Settings);
        menuSettings->addAction(actionHMM_Settings);
        menuSettings->addAction(actionOpenCL_Settings);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_File);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionFrame_Settings->setText(QApplication::translate("MainWindow", "Frame Settings", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("MainWindow", "Open", Q_NULLPTR));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", Q_NULLPTR));
#endif // QT_NO_SHORTCUT
        actionHMM_Settings->setText(QApplication::translate("MainWindow", "HMM Settings", Q_NULLPTR));
        actionAudio_Settings->setText(QApplication::translate("MainWindow", "Audio Settings", Q_NULLPTR));
        actionOpenCL_Settings->setText(QApplication::translate("MainWindow", "OpenCL Settings", Q_NULLPTR));
        actionOpen_File->setText(QApplication::translate("MainWindow", "Open File", Q_NULLPTR));
        actionSave_Model->setText(QApplication::translate("MainWindow", "Save Model", Q_NULLPTR));
        FileLabel->setText(QString());
        openclbox->setText(QApplication::translate("MainWindow", "Use OpenCL", Q_NULLPTR));
        TimingLabel->setText(QString());
        ResetButton->setText(QApplication::translate("MainWindow", "RESET", Q_NULLPTR));
        StartButton->setText(QApplication::translate("MainWindow", "INITIALIZE", Q_NULLPTR));
        menuSettings->setTitle(QApplication::translate("MainWindow", "Settings", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
