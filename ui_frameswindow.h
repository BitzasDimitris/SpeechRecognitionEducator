/********************************************************************************
** Form generated from reading UI file 'frameswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMESWINDOW_H
#define UI_FRAMESWINDOW_H

#include <qcustomplot.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_FramesWindow
{
public:
    QVBoxLayout *verticalLayout_2;
    QLabel *sizelabel;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *CurrentFrame;
    QSlider *Frame2Show;
    QCustomPlot *FramePlot;

    void setupUi(QDialog *FramesWindow)
    {
        if (FramesWindow->objectName().isEmpty())
            FramesWindow->setObjectName(QStringLiteral("FramesWindow"));
        FramesWindow->resize(926, 582);
        verticalLayout_2 = new QVBoxLayout(FramesWindow);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        sizelabel = new QLabel(FramesWindow);
        sizelabel->setObjectName(QStringLiteral("sizelabel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sizelabel->sizePolicy().hasHeightForWidth());
        sizelabel->setSizePolicy(sizePolicy);
        sizelabel->setMaximumSize(QSize(16777215, 10));

        verticalLayout_2->addWidget(sizelabel);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label = new QLabel(FramesWindow);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(1);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(60, 16777215));

        horizontalLayout->addWidget(label);

        CurrentFrame = new QLabel(FramesWindow);
        CurrentFrame->setObjectName(QStringLiteral("CurrentFrame"));
        CurrentFrame->setMinimumSize(QSize(30, 0));

        horizontalLayout->addWidget(CurrentFrame);

        Frame2Show = new QSlider(FramesWindow);
        Frame2Show->setObjectName(QStringLiteral("Frame2Show"));
        Frame2Show->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(Frame2Show);


        verticalLayout->addLayout(horizontalLayout);

        FramePlot = new QCustomPlot(FramesWindow);
        FramePlot->setObjectName(QStringLiteral("FramePlot"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(FramePlot->sizePolicy().hasHeightForWidth());
        FramePlot->setSizePolicy(sizePolicy2);

        verticalLayout->addWidget(FramePlot);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(FramesWindow);

        QMetaObject::connectSlotsByName(FramesWindow);
    } // setupUi

    void retranslateUi(QDialog *FramesWindow)
    {
        FramesWindow->setWindowTitle(QApplication::translate("FramesWindow", "Dialog", 0));
        sizelabel->setText(QApplication::translate("FramesWindow", "TextLabel", 0));
        label->setText(QApplication::translate("FramesWindow", "Frame:", 0));
        CurrentFrame->setText(QApplication::translate("FramesWindow", "1", 0));
    } // retranslateUi

};

namespace Ui {
    class FramesWindow: public Ui_FramesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMESWINDOW_H
