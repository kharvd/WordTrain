#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QSettings settings;

    cmbLanguages = new QComboBox();
    cmbLanguages->addItem(QIcon(), tr("<Auto>"));
    cmbLanguages->setItemData(0, "auto");
    cmbLanguages->addItem(QIcon(), tr("English"));
    cmbLanguages->setItemData(1, "en");

    fillLanguages();

    for (int i = 0; i < cmbLanguages->count(); i++) {
        if (cmbLanguages->itemData(i).toString()
            == settings.value("lang", "auto").toString())
        {
            cmbLanguages->setCurrentIndex(i);
        }
    }

    txtCorrAnswers = new QLineEdit(QString("%1")
                                   .arg(settings.value("corr_answers", 10).toInt()));
    QIntValidator *validator = new QIntValidator(1, 10000, this);
    txtCorrAnswers->setValidator(validator);

    chckSaveWinPosition = new QCheckBox(tr("Save window position and size"));
    chckSaveWinPosition->setChecked(settings.value("save_pos", 1).toBool());

    btnOk = new QPushButton(tr("OK"));
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));

    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *ltButtons = new QHBoxLayout();
    ltButtons->addStretch(1);
    ltButtons->addWidget(btnOk);
    ltButtons->addWidget(btnCancel);

    QFormLayout *ltForm = new QFormLayout();
    ltForm->addRow(tr("Language (will be changed on the next run):"),
                   cmbLanguages);
    ltForm->addRow(tr("Word is learned after ... correct answers:"), txtCorrAnswers);

    QVBoxLayout *ltMain = new QVBoxLayout();
    ltMain->addLayout(ltForm);
    ltMain->addWidget(chckSaveWinPosition);
    ltMain->addStretch(1);
    ltMain->addLayout(ltButtons);

    connect(this, SIGNAL(accepted()), SLOT(writeSettings()));

    setWindowTitle(tr("Settings"));
    resize(300, 200);
    setLayout(ltMain);
}

void SettingsDialog::writeSettings()
{
    QSettings settings;
    settings.setValue("lang", cmbLanguages->itemData(
            cmbLanguages->currentIndex()).toString());
    settings.setValue("corr_answers", txtCorrAnswers->text().toInt());
    settings.setValue("save_pos", chckSaveWinPosition->isChecked());
}

void SettingsDialog::fillLanguages()
{
    QDir qmDir(":/translations");
    QStringList fileNames =
            qmDir.entryList(QStringList("wordtrain_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.chop(3);

        QTranslator translator;
        translator.load(fileNames[i], qmDir.absolutePath());
        QString language = translator.translate("Languages",
                                                "English");

        cmbLanguages->addItem(QIcon(), language);
        cmbLanguages->setItemData(cmbLanguages->count() + i - 1, locale);
    }
}




