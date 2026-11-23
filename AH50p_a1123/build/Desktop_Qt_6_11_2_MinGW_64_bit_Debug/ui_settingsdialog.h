/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QVBoxLayout *mainLayout;
    QFormLayout *formLayout;
    QLabel *languageLabel;
    QComboBox *languageComboBox;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *buttonSpacer;
    QPushButton *applyButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName("SettingsDialog");
        SettingsDialog->resize(360, 150);
        SettingsDialog->setMinimumSize(QSize(320, 140));
        SettingsDialog->setModal(true);
        mainLayout = new QVBoxLayout(SettingsDialog);
        mainLayout->setSpacing(12);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(16, 16, 16, 16);
        formLayout = new QFormLayout();
        formLayout->setObjectName("formLayout");
        formLayout->setHorizontalSpacing(12);
        formLayout->setVerticalSpacing(10);
        languageLabel = new QLabel(SettingsDialog);
        languageLabel->setObjectName("languageLabel");

        formLayout->setWidget(0, QFormLayout::ItemRole::LabelRole, languageLabel);

        languageComboBox = new QComboBox(SettingsDialog);
        languageComboBox->addItem(QString());
        languageComboBox->addItem(QString());
        languageComboBox->setObjectName("languageComboBox");

        formLayout->setWidget(0, QFormLayout::ItemRole::FieldRole, languageComboBox);


        mainLayout->addLayout(formLayout);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(verticalSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(8);
        buttonLayout->setObjectName("buttonLayout");
        buttonSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(buttonSpacer);

        applyButton = new QPushButton(SettingsDialog);
        applyButton->setObjectName("applyButton");

        buttonLayout->addWidget(applyButton);

        cancelButton = new QPushButton(SettingsDialog);
        cancelButton->setObjectName("cancelButton");

        buttonLayout->addWidget(cancelButton);


        mainLayout->addLayout(buttonLayout);


        retranslateUi(SettingsDialog);
        QObject::connect(applyButton, &QPushButton::clicked, SettingsDialog, qOverload<>(&QDialog::accept));
        QObject::connect(cancelButton, &QPushButton::clicked, SettingsDialog, qOverload<>(&QDialog::reject));

        applyButton->setDefault(true);


        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QCoreApplication::translate("SettingsDialog", "\350\250\255\345\256\232", nullptr));
        languageLabel->setText(QCoreApplication::translate("SettingsDialog", "\350\252\236\350\250\200\357\274\232", nullptr));
        languageComboBox->setItemText(0, QCoreApplication::translate("SettingsDialog", "\347\271\201\351\253\224\344\270\255\346\226\207", nullptr));
        languageComboBox->setItemText(1, QCoreApplication::translate("SettingsDialog", "English", nullptr));

        applyButton->setText(QCoreApplication::translate("SettingsDialog", "\345\245\227\347\224\250", nullptr));
        cancelButton->setText(QCoreApplication::translate("SettingsDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
