#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    enum class Language {
        TraditionalChinese = 0,
        English = 1
    };

    explicit SettingsDialog(Language currentLanguage, QWidget *parent = nullptr);
    ~SettingsDialog();

    Language selectedLanguage() const;

private:
    void applyDialogLanguage(Language language);

    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H