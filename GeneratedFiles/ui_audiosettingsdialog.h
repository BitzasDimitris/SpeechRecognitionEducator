/********************************************************************************
** Form generated from reading UI file 'audiosettingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AUDIOSETTINGSDIALOG_H
#define UI_AUDIOSETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_AudioSettingsDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QSpinBox *SampleRateBox;

    void setupUi(QDialog *AudioSettingsDialog)
    {
        if (AudioSettingsDialog->objectName().isEmpty())
            AudioSettingsDialog->setObjectName(QStringLiteral("AudioSettingsDialog"));
        AudioSettingsDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(AudioSettingsDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(AudioSettingsDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 90, 191, 41));
        QFont font;
        font.setFamily(QStringLiteral("Garamond"));
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        SampleRateBox = new QSpinBox(AudioSettingsDialog);
        SampleRateBox->setObjectName(QStringLiteral("SampleRateBox"));
        SampleRateBox->setGeometry(QRect(240, 90, 71, 41));
        QFont font1;
        font1.setFamily(QStringLiteral("Garamond"));
        font1.setPointSize(12);
        SampleRateBox->setFont(font1);
        SampleRateBox->setMinimum(1500);
        SampleRateBox->setMaximum(16000);
        SampleRateBox->setSingleStep(500);
        SampleRateBox->setValue(10000);

        retranslateUi(AudioSettingsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), AudioSettingsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), AudioSettingsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(AudioSettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *AudioSettingsDialog)
    {
        AudioSettingsDialog->setWindowTitle(QApplication::translate("AudioSettingsDialog", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("AudioSettingsDialog", "Recognition Sample Rate:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AudioSettingsDialog: public Ui_AudioSettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AUDIOSETTINGSDIALOG_H
