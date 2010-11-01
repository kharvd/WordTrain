/******************************************************************************
** WordTrain 0.8.5 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent)
{
    QSettings settings;

    cmbLanguages = new QComboBox();
    cmbLanguages->addItem(QIcon(), tr("<Auto>"));
    cmbLanguages->setItemData(0, "auto");
    cmbLanguages->addItem(QIcon(), "English");
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




