/********************************************************************************
** Form generated from reading UI file 'hmmsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HMMSETTINGS_H
#define UI_HMMSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_HMMSettings
{
public:
    QDialogButtonBox *buttonBox;
    QSlider *QualitySlider;
    QLabel *label;
    QSpinBox *QualitySpinner;
    QLabel *label_3;
    QSpinBox *MaxVecSpinner;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *CrossSpinner;
    QCheckBox *SilenceBox;

    void setupUi(QDialog *HMMSettings)
    {
        if (HMMSettings->objectName().isEmpty())
            HMMSettings->setObjectName(QStringLiteral("HMMSettings"));
        HMMSettings->resize(692, 474);
        buttonBox = new QDialogButtonBox(HMMSettings);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(320, 410, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        QualitySlider = new QSlider(HMMSettings);
        QualitySlider->setObjectName(QStringLiteral("QualitySlider"));
        QualitySlider->setGeometry(QRect(20, 60, 160, 41));
        QualitySlider->setMinimum(50);
        QualitySlider->setMaximum(100);
        QualitySlider->setValue(75);
        QualitySlider->setOrientation(Qt::Horizontal);
        label = new QLabel(HMMSettings);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 30, 151, 31));
        QFont font;
        font.setFamily(QStringLiteral("Arial"));
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        QualitySpinner = new QSpinBox(HMMSettings);
        QualitySpinner->setObjectName(QStringLiteral("QualitySpinner"));
        QualitySpinner->setGeometry(QRect(190, 50, 51, 51));
        QualitySpinner->setMinimum(50);
        QualitySpinner->setMaximum(100);
        QualitySpinner->setValue(75);
        label_3 = new QLabel(HMMSettings);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(400, 30, 261, 31));
        label_3->setFont(font);
        MaxVecSpinner = new QSpinBox(HMMSettings);
        MaxVecSpinner->setObjectName(QStringLiteral("MaxVecSpinner"));
        MaxVecSpinner->setGeometry(QRect(400, 70, 91, 41));
        MaxVecSpinner->setMinimum(0);
        MaxVecSpinner->setMaximum(10000);
        MaxVecSpinner->setValue(0);
        label_4 = new QLabel(HMMSettings);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(540, 70, 171, 31));
        QFont font1;
        font1.setFamily(QStringLiteral("Arial"));
        font1.setPointSize(9);
        font1.setBold(true);
        font1.setItalic(true);
        font1.setWeight(75);
        label_4->setFont(font1);
        label_5 = new QLabel(HMMSettings);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(400, 130, 261, 31));
        label_5->setFont(font);
        label_6 = new QLabel(HMMSettings);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(540, 170, 131, 51));
        label_6->setFont(font1);
        CrossSpinner = new QSpinBox(HMMSettings);
        CrossSpinner->setObjectName(QStringLiteral("CrossSpinner"));
        CrossSpinner->setGeometry(QRect(400, 170, 91, 41));
        CrossSpinner->setMinimum(0);
        CrossSpinner->setMaximum(10000);
        CrossSpinner->setValue(0);
        SilenceBox = new QCheckBox(HMMSettings);
        SilenceBox->setObjectName(QStringLiteral("SilenceBox"));
        SilenceBox->setGeometry(QRect(60, 160, 151, 41));
        SilenceBox->setFont(font);

        retranslateUi(HMMSettings);
        QObject::connect(buttonBox, SIGNAL(accepted()), HMMSettings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), HMMSettings, SLOT(reject()));

        QMetaObject::connectSlotsByName(HMMSettings);
    } // setupUi

    void retranslateUi(QDialog *HMMSettings)
    {
        HMMSettings->setWindowTitle(QApplication::translate("HMMSettings", "Dialog", 0));
        label->setText(QApplication::translate("HMMSettings", "GMM Quality", 0));
        QualitySpinner->setSuffix(QApplication::translate("HMMSettings", "%", 0));
        label_3->setText(QApplication::translate("HMMSettings", "Initial Samples Per Phoneme", 0));
        MaxVecSpinner->setSuffix(QString());
        label_4->setText(QApplication::translate("HMMSettings", " 0 for All ", 0));
        label_5->setText(QApplication::translate("HMMSettings", "Cross Validation Percentage", 0));
        label_6->setText(QApplication::translate("HMMSettings", "<html><head/><body><p>0 for evaluation of </p><p>complete Training Set</p></body></html>", 0));
        CrossSpinner->setSuffix(QApplication::translate("HMMSettings", "%", 0));
        SilenceBox->setText(QApplication::translate("HMMSettings", "Model Silence", 0));
    } // retranslateUi

};

namespace Ui {
    class HMMSettings: public Ui_HMMSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HMMSETTINGS_H
