#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(Language currentLanguage, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    ui->languageComboBox->setCurrentIndex(static_cast<int>(currentLanguage));
    applyDialogLanguage(currentLanguage);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Language SettingsDialog::selectedLanguage() const
{
    return (ui->languageComboBox->currentIndex() == 1)
    ? Language::English
    : Language::TraditionalChinese;
}

void SettingsDialog::applyDialogLanguage(Language language)
{
    const bool isZh = (language == Language::TraditionalChinese);
    setWindowTitle(isZh ? QStringLiteral("設定") : QStringLiteral("Settings"));
    ui->languageLabel->setText(isZh ? QStringLiteral("語言：") : QStringLiteral("Language:"));
    ui->applyButton->setText(isZh ? QStringLiteral("套用") : QStringLiteral("Apply"));
    ui->cancelButton->setText(isZh ? QStringLiteral("取消") : QStringLiteral("Cancel"));
}