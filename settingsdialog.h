#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);

private slots:
    void writeSettings();
    void fillLanguages();

private:
    QComboBox *cmbLanguages;
    QLineEdit *txtCorrAnswers;
    QCheckBox *chckSaveWinPosition;
    QPushButton *btnOk;
    QPushButton *btnCancel;
};

#endif // SETTINGSDIALOG_H
