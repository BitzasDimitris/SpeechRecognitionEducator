/********************************************************************************
** Form generated from reading UI file 'mfccdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MFCCDIALOG_H
#define UI_MFCCDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_MFCCDialog
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QLabel *label;
    QPushButton *WaveFormButton;
    QLabel *label_5;
    QPushButton *FFTFrames;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_2;
    QPushButton *Waveformafterlowpass;
    QLabel *label_6;
    QPushButton *MelFrames;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QPushButton *Frames;
    QLabel *label_7;
    QPushButton *LogMelFrames;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_4;
    QPushButton *HFrames;
    QLabel *label_8;
    QPushButton *DCTFrames;

    void setupUi(QDialog *MFCCDialog)
    {
        if (MFCCDialog->objectName().isEmpty())
            MFCCDialog->setObjectName(QStringLiteral("MFCCDialog"));
        MFCCDialog->resize(914, 351);
        MFCCDialog->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout = new QHBoxLayout(MFCCDialog);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label = new QLabel(MFCCDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(16777215, 40));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setLayoutDirection(Qt::RightToLeft);
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label);

        WaveFormButton = new QPushButton(MFCCDialog);
        WaveFormButton->setObjectName(QStringLiteral("WaveFormButton"));
        WaveFormButton->setEnabled(true);
        WaveFormButton->setFont(font);

        verticalLayout_2->addWidget(WaveFormButton);

        label_5 = new QLabel(MFCCDialog);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setMaximumSize(QSize(16777215, 40));
        label_5->setFont(font);
        label_5->setLayoutDirection(Qt::RightToLeft);
        label_5->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(label_5);

        FFTFrames = new QPushButton(MFCCDialog);
        FFTFrames->setObjectName(QStringLiteral("FFTFrames"));
        FFTFrames->setEnabled(true);
        FFTFrames->setFont(font);

        verticalLayout_2->addWidget(FFTFrames);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label_2 = new QLabel(MFCCDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(16777215, 40));
        label_2->setFont(font);
        label_2->setLayoutDirection(Qt::RightToLeft);
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_2);

        Waveformafterlowpass = new QPushButton(MFCCDialog);
        Waveformafterlowpass->setObjectName(QStringLiteral("Waveformafterlowpass"));
        Waveformafterlowpass->setEnabled(true);
        Waveformafterlowpass->setFont(font);

        verticalLayout_3->addWidget(Waveformafterlowpass);

        label_6 = new QLabel(MFCCDialog);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(16777215, 40));
        label_6->setFont(font);
        label_6->setLayoutDirection(Qt::RightToLeft);
        label_6->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_6);

        MelFrames = new QPushButton(MFCCDialog);
        MelFrames->setObjectName(QStringLiteral("MelFrames"));
        MelFrames->setEnabled(true);
        MelFrames->setFont(font);

        verticalLayout_3->addWidget(MelFrames);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_3 = new QLabel(MFCCDialog);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(16777215, 40));
        label_3->setFont(font);
        label_3->setLayoutDirection(Qt::RightToLeft);
        label_3->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_3);

        Frames = new QPushButton(MFCCDialog);
        Frames->setObjectName(QStringLiteral("Frames"));
        Frames->setEnabled(true);
        Frames->setFont(font);

        verticalLayout_4->addWidget(Frames);

        label_7 = new QLabel(MFCCDialog);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setMaximumSize(QSize(16777215, 40));
        label_7->setFont(font);
        label_7->setLayoutDirection(Qt::RightToLeft);
        label_7->setAlignment(Qt::AlignCenter);

        verticalLayout_4->addWidget(label_7);

        LogMelFrames = new QPushButton(MFCCDialog);
        LogMelFrames->setObjectName(QStringLiteral("LogMelFrames"));
        LogMelFrames->setEnabled(true);
        LogMelFrames->setFont(font);

        verticalLayout_4->addWidget(LogMelFrames);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_4 = new QLabel(MFCCDialog);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(16777215, 40));
        label_4->setFont(font);
        label_4->setLayoutDirection(Qt::RightToLeft);
        label_4->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_4);

        HFrames = new QPushButton(MFCCDialog);
        HFrames->setObjectName(QStringLiteral("HFrames"));
        HFrames->setEnabled(true);
        HFrames->setFont(font);

        verticalLayout_5->addWidget(HFrames);

        label_8 = new QLabel(MFCCDialog);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMaximumSize(QSize(16777215, 40));
        label_8->setFont(font);
        label_8->setLayoutDirection(Qt::RightToLeft);
        label_8->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_8);

        DCTFrames = new QPushButton(MFCCDialog);
        DCTFrames->setObjectName(QStringLiteral("DCTFrames"));
        DCTFrames->setEnabled(true);
        DCTFrames->setFont(font);

        verticalLayout_5->addWidget(DCTFrames);


        horizontalLayout->addLayout(verticalLayout_5);


        retranslateUi(MFCCDialog);

        QMetaObject::connectSlotsByName(MFCCDialog);
    } // setupUi

    void retranslateUi(QDialog *MFCCDialog)
    {
        MFCCDialog->setWindowTitle(QApplication::translate("MFCCDialog", "Dialog", 0));
        label->setText(QApplication::translate("MFCCDialog", "Digital Read", 0));
        WaveFormButton->setText(QApplication::translate("MFCCDialog", "WaveForm", 0));
        label_5->setText(QApplication::translate("MFCCDialog", "FFT", 0));
        FFTFrames->setText(QApplication::translate("MFCCDialog", "Show FFT of  Frames", 0));
        label_2->setText(QApplication::translate("MFCCDialog", "Lowpass filter", 0));
        Waveformafterlowpass->setText(QApplication::translate("MFCCDialog", "Waveform", 0));
        label_6->setText(QApplication::translate("MFCCDialog", "  Log of Mel Filterbank Energies", 0));
        MelFrames->setText(QApplication::translate("MFCCDialog", "Vectors of MF Energies", 0));
        label_3->setText(QApplication::translate("MFCCDialog", "Framming", 0));
        Frames->setText(QApplication::translate("MFCCDialog", "Show Frames", 0));
        label_7->setText(QApplication::translate("MFCCDialog", "DCT", 0));
        LogMelFrames->setText(QApplication::translate("MFCCDialog", "DCT of MF Energy Vectors", 0));
        label_4->setText(QApplication::translate("MFCCDialog", "Hamming", 0));
        HFrames->setText(QApplication::translate("MFCCDialog", "Show Frames after hamming", 0));
        label_8->setText(QApplication::translate("MFCCDialog", "MFCC", 0));
        DCTFrames->setText(QApplication::translate("MFCCDialog", "MFCC Vectors", 0));
    } // retranslateUi

};

namespace Ui {
    class MFCCDialog: public Ui_MFCCDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MFCCDIALOG_H
