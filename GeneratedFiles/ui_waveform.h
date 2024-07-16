/********************************************************************************
** Form generated from reading UI file 'waveform.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAVEFORM_H
#define UI_WAVEFORM_H

#include <../../qcustomplot.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WaveForm
{
public:
    QVBoxLayout *verticalLayout;
    QCustomPlot *TimeDomain;

    void setupUi(QDialog *WaveForm)
    {
        if (WaveForm->objectName().isEmpty())
            WaveForm->setObjectName(QStringLiteral("WaveForm"));
        WaveForm->resize(894, 492);
        WaveForm->setSizeGripEnabled(true);
        verticalLayout = new QVBoxLayout(WaveForm);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        TimeDomain = new QCustomPlot(WaveForm);
        TimeDomain->setObjectName(QStringLiteral("TimeDomain"));

        verticalLayout->addWidget(TimeDomain);


        retranslateUi(WaveForm);

        QMetaObject::connectSlotsByName(WaveForm);
    } // setupUi

    void retranslateUi(QDialog *WaveForm)
    {
        WaveForm->setWindowTitle(QApplication::translate("WaveForm", "WaveForm", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WaveForm: public Ui_WaveForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAVEFORM_H
