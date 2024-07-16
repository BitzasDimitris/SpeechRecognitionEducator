/********************************************************************************
** Form generated from reading UI file 'openclsettings.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPENCLSETTINGS_H
#define UI_OPENCLSETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OpenClSettings
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *PlatformDropdown;
    QComboBox *DeviceDropdown;

    void setupUi(QDialog *OpenClSettings)
    {
        if (OpenClSettings->objectName().isEmpty())
            OpenClSettings->setObjectName(QStringLiteral("OpenClSettings"));
        OpenClSettings->resize(727, 275);
        layoutWidget = new QWidget(OpenClSettings);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 0, 721, 271));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 30));
        QFont font;
        font.setPointSize(12);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label);

        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        label_2->setMinimumSize(QSize(0, 30));
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_2);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        PlatformDropdown = new QComboBox(layoutWidget);
        PlatformDropdown->setObjectName(QStringLiteral("PlatformDropdown"));
        PlatformDropdown->setMinimumSize(QSize(0, 30));
        PlatformDropdown->setFont(font);

        horizontalLayout_3->addWidget(PlatformDropdown);

        DeviceDropdown = new QComboBox(layoutWidget);
        DeviceDropdown->setObjectName(QStringLiteral("DeviceDropdown"));
        DeviceDropdown->setMinimumSize(QSize(0, 30));
        DeviceDropdown->setFont(font);

        horizontalLayout_3->addWidget(DeviceDropdown);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(OpenClSettings);

        QMetaObject::connectSlotsByName(OpenClSettings);
    } // setupUi

    void retranslateUi(QDialog *OpenClSettings)
    {
        OpenClSettings->setWindowTitle(QApplication::translate("OpenClSettings", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("OpenClSettings", "Availiable Platforms:", Q_NULLPTR));
        label_2->setText(QApplication::translate("OpenClSettings", "Availiable Devices", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class OpenClSettings: public Ui_OpenClSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPENCLSETTINGS_H
