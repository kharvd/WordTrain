/******************************************************************************
** WordTrain 0.9.1 -- Foreign words trainer
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

#include "neweditcarddialog.h"

#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QFormLayout>
#include <QGroupBox>
#include <QScrollBar>
#include <QPushButton>
#include "exampleswidget.h"

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
    // Getting data from the form
    mNewCard.setWord(txtWord->text());
    mNewCard.setTranscription(txtTranscription->text());
    mNewCard.setTranslation(txtTranslation->text());
    mNewCard.setPlural(txtPlural->text());
    mNewCard.setCategory(cmbCategory->currentIndex());
    mNewCard.setGender(cmbGender->currentIndex());
    mNewCard.clearExamples();

    for (int i = 0; (i < wgtExamples->examplesCount())
        && (i <= ExamplesWidget::maxExamples); i++)
    {
        Example ex = wgtExamples->getExampleAt(i);
        if (!ex.first.isEmpty())
            mNewCard.addExample(wgtExamples->getExampleAt(i));
    }

    return mNewCard;
}

void NewEditCardDialog::addExample() 
{
    // Adding new example and resizing the window
    scrollExamples->show();

    // Height for window with examples
    int optimalHeight = height() + exampleHeight;
    optimalHeight = (optimalHeight > maxAutoHeight) ? maxAutoHeight
                                                    : optimalHeight;
    resize(width(), optimalHeight);

    wgtExamples->addExample();
    scrollExamples->verticalScrollBar()->
            triggerAction(QAbstractSlider::SliderToMaximum);
	    
    // If we cannot add another example
    if (!wgtExamples->canAdd())
        btnAddExample->setEnabled(false);
}

void NewEditCardDialog::resetProgress()
{
    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();
    txtProgress->setText(QString("0/%1").arg(corrAnsForLearned));
    mNewCard.setCorrectAnswers
(0);
}

void NewEditCardDialog::switchPluralGender(int cat)
{
    // If the word is noun, show plural and gender fields, otherwise hide them
    if (cat == (int)LC_Noun) {
        txtPlural->show();
        cmbGender->show();
        fLayout->labelForField(txtPlural)->show();
        fLayout->labelForField(cmbGender)->show();
    } else {
        txtPlural->hide();
        cmbGender->hide();
        fLayout->labelForField(txtPlural)->hide();
        fLayout->labelForField(cmbGender)->hide();
    }
}

void NewEditCardDialog::createInterface()
{
    resize(defaultWidth, defaultHeight);
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

    QPushButton *btnOk;
    QPushButton *btnCancel;
    btnOk = new QPushButton(tr("OK"));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));
    
    txtProgress = new QLineEdit();
    txtProgress->setReadOnly(true);
    
    QPushButton *btnResetProgress = new QPushButton(tr("Reset progress"));
    connect(btnResetProgress, SIGNAL(clicked(bool)), SLOT(resetProgress()));
    
    QHBoxLayout *progressLayout = new QHBoxLayout;
    progressLayout->addWidget(txtProgress);
    progressLayout->addWidget(btnResetProgress);
    progressLayout->addStretch();
    
    fLayout = new QFormLayout();
    fLayout->addRow(tr("Word:"), txtWord);
    fLayout->addRow(tr("Transcription:"), txtTranscription);
    fLayout->addRow(tr("Translation:"), txtTranslation);
    fLayout->addRow(tr("Category:"), cmbCategory);
    fLayout->addRow(tr("Plural:"), txtPlural);
    fLayout->addRow(tr("Gender:"), cmbGender);
    fLayout->addRow(tr("Progress:"), progressLayout);
    fLayout->addRow(tr("Examples:"), btnAddExample);

    connect(cmbCategory, SIGNAL(currentIndexChanged(int)),
            SLOT(switchPluralGender(int)));
    switchPluralGender(0);
    
    QVBoxLayout *hLayout = new QVBoxLayout();
    hLayout->addLayout(fLayout);
    hLayout->addWidget(scrollExamples);

    QGroupBox *grBox = new QGroupBox(windowTitle());
    grBox->setLayout(hLayout);

    QVBoxLayout *hButtonsLayout = new QVBoxLayout();
    hButtonsLayout->addWidget(btnOk);
    hButtonsLayout->addWidget(btnCancel);
    hButtonsLayout->addStretch(1);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(grBox);
    mainLayout->addLayout(hButtonsLayout);

    setLayout(mainLayout);
}

void NewEditCardDialog::fillForm()
{
    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();

    // Filling the form with the word card's contents
    txtWord->setText(mNewCard.word());
    txtTranscription->setText(mNewCard.transcription());
    txtTranslation->setText(mNewCard.translation());
    txtPlural->setText(mNewCard.plural());
    txtProgress->setText(QString("%1/%2").arg(mNewCard.correctAnswers
())
                         .arg(corrAnsForLearned));
    cmbCategory->setCurrentIndex(mNewCard.category());
    cmbGender->setCurrentIndex(mNewCard.gender());

    for (int i = 0; i < (mNewCard.examplesSize())
            && (i <= ExamplesWidget::maxExamples); i++) {
        addExample();
        wgtExamples->setExampleAt(i, mNewCard.exampleAt(i));
    }
}
