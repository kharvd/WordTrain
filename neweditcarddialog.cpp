/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov (kharvd@gmail.com)
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

#include "neweditcarddialog.h"

/* Add card */
NewEditCardDialog::NewEditCardDialog(QWidget *parent) :
        QDialog(parent)
{
    setWindowTitle(tr("New card"));
    createInterface();
}

/* Edit card */
NewEditCardDialog::NewEditCardDialog(const WordCard &card, QWidget *parent) :
        QDialog(parent)
{
    mNewCard = card;
    setWindowTitle(tr("Edit card"));
    createInterface();
    fillForm();
}

const WordCard& NewEditCardDialog::getNewCard()
{
    mNewCard.setWord(txtWord->text());
    mNewCard.setTranscription(txtTranscription->text());
    mNewCard.setTranslation(txtTranslation->text());
    mNewCard.setPlural(txtPlural->text());
    mNewCard.setCategory(cmbCategory->currentIndex());
    mNewCard.setGender(cmbGender->currentIndex());
    mNewCard.setNumCorrectAnswers(0);
    mNewCard.clearExamples();

    for (int i = 0; (i < wgtExamples->examplesCount())
        && (i <= ExamplesWidget::maxExamples); i++)
    {
        Example ex = wgtExamples->getExampleAt(i);
        if (!ex.first.isEmpty()) {
            mNewCard.addExample(wgtExamples->getExampleAt(i));
        }
    }

    return mNewCard;
}

void NewEditCardDialog::addExample() {
    scrollExamples->show();
    int optimalHeight = height() + 66;
    optimalHeight = (optimalHeight > 500) ? 500 : optimalHeight;
    resize(width(), optimalHeight);

    wgtExamples->addExample();
    if (!wgtExamples->canAdd()) {
        btnAddExample->setEnabled(false);
    }
}

void NewEditCardDialog::createInterface()
{
    resize(600, 300);
    setWindowFlags(Qt::Window);
    txtWord = new QLineEdit();

    txtTranscription = new QLineEdit();
    txtTranslation = new QLineEdit();
    txtPlural = new QLineEdit();

    cmbCategory = new QComboBox();
    cmbCategory->addItems(WordCard::lexCategoriesStrings());

    cmbGender = new QComboBox();
    cmbGender->addItems(WordCard::genderStrings());

    btnAddExample = new QPushButton(tr("Add example"));
    connect(btnAddExample, SIGNAL(clicked()), SLOT(addExample()));

    wgtExamples = new ExamplesWidget();
    scrollExamples = new QScrollArea;
    scrollExamples->setWidgetResizable(true);
    scrollExamples->setWidget(wgtExamples);
    scrollExamples->setFrameStyle(QFrame::NoFrame);
    scrollExamples->setAlignment(Qt::AlignTop);
    scrollExamples->hide();

    btnOk = new QPushButton(tr("OK"));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QFormLayout* fLayout = new QFormLayout();
    fLayout->addRow(tr("Word:"), txtWord);
    fLayout->addRow(tr("Transcription:"), txtTranscription);
    fLayout->addRow(tr("Translation:"), txtTranslation);
    fLayout->addRow(tr("Plural:"), txtPlural);
    fLayout->addRow(tr("Category:"), cmbCategory);
    fLayout->addRow(tr("Gender:"), cmbGender);
    fLayout->addRow(tr("Examples:"), btnAddExample);

    QVBoxLayout* hLayout = new QVBoxLayout();
    hLayout->addLayout(fLayout);
    hLayout->addWidget(scrollExamples);

    QGroupBox* grBox = new QGroupBox(windowTitle());
    grBox->setLayout(hLayout);

    QVBoxLayout* hButtonsLayout = new QVBoxLayout();
    hButtonsLayout->addWidget(btnOk);
    hButtonsLayout->addWidget(btnCancel);
    hButtonsLayout->addStretch(1);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(grBox);
    mainLayout->addLayout(hButtonsLayout);

    setLayout(mainLayout);
}

void NewEditCardDialog::fillForm()
{
    txtWord->setText(mNewCard.word());
    txtTranscription->setText(mNewCard.transcription());
    txtTranslation->setText(mNewCard.translation());
    txtPlural->setText(mNewCard.plural());
    cmbCategory->setCurrentIndex(mNewCard.category());
    cmbGender->setCurrentIndex(mNewCard.gender());

    for (int i = 0; i < (mNewCard.examplesSize())
        && (i <= ExamplesWidget::maxExamples); i++)
    {
        addExample();
        wgtExamples->setExampleAt(i, mNewCard.exampleAt(i));
    }
}
