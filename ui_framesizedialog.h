/********************************************************************************
** Form generated from reading UI file 'framesizedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRAMESIZEDIALOG_H
#define UI_FRAMESIZEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_FrameSizeDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *Framesizetxt;
    QLabel *label_2;
    QLineEdit *Overlappingtxt;

    void setupUi(QDialog *FrameSizeDialog)
    {
        if (FrameSizeDialog->objectName().isEmpty())
            FrameSizeDialog->setObjectName(QStringLiteral("FrameSizeDialog"));
        FrameSizeDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(FrameSizeDialog);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(FrameSizeDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 50, 131, 21));
        QFont font;
        font.setFamily(QStringLiteral("Garamond"));
        font.setPointSize(14);
        label->setFont(font);
        Framesizetxt = new QLineEdit(FrameSizeDialog);
        Framesizetxt->setObjectName(QStringLiteral("Framesizetxt"));
        Framesizetxt->setGeometry(QRect(200, 50, 113, 20));
        label_2 = new QLabel(FrameSizeDialog);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 110, 141, 21));
        label_2->setFont(font);
        Overlappingtxt = new QLineEdit(FrameSizeDialog);
        Overlappingtxt->setObjectName(QStringLiteral("Overlappingtxt"));
        Overlappingtxt->setGeometry(QRect(200, 110, 113, 20));

        retranslateUi(FrameSizeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), FrameSizeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), FrameSizeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(FrameSizeDialog);
    } // setupUi

    void retranslateUi(QDialog *FrameSizeDialog)
    {
        FrameSizeDialog->setWindowTitle(QApplication::translate("FrameSizeDialog", "Dialog", 0));
        label->setText(QApplication::translate("FrameSizeDialog", "Frame Size in ms:", 0));
        label_2->setText(QApplication::translate("FrameSizeDialog", "Overlapping in ms:", 0));
    } // retranslateUi

};

namespace Ui {
    class FrameSizeDialog: public Ui_FrameSizeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRAMESIZEDIALOG_H
