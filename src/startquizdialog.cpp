/******************************************************************************
** WordTrain 0.9.2 -- Foreign words trainer
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

#include "startquizdialog.h"

#include "learnedwordfilter.h"
#include "quizwordschooserdialog.h"

#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QValidator>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

StartQuizDialog::StartQuizDialog(WordsSet * words, QWidget *parent) :
    QDialog(parent)
{
    for (int i = 0; i < words->size(); i++)
        m_Cards.push_back(&words[0][i]);

    dlgWordsChooser = new QuizWordsChooserDialog(m_Cards, this);

    setWindowTitle(tr("Start test"));
    createInterface();
}

void StartQuizDialog::createInterface()
{
    // Mode
    radioMultiChoiceMode = new QRadioButton(tr("Multiple choice"));
    radioNoChoiceMode    = new QRadioButton(tr("Without choice"));
    radioRandomMode      = new QRadioButton(tr("Random", "Mode"));

    grpChoiceMode = new QButtonGroup(this);
    grpChoiceMode->addButton(radioMultiChoiceMode, QuestionTypeMultiChoice);
    grpChoiceMode->addButton(radioNoChoiceMode, QuestionTypeNoChoice);
    grpChoiceMode->addButton(radioRandomMode, QuestionTypeRandom);

    radioMultiChoiceMode->setChecked(true);

    // Words
    radioNumWords = new QRadioButton();
    connect(radioNumWords, SIGNAL(toggled(bool)), SLOT(toggleTxtNumWords(bool)));

    txtNumWords = new QLineEdit();
    QIntValidator *validator = new QIntValidator(1, 1000, this);
    txtNumWords->setValidator(validator);
    txtNumWords->setEnabled(false);

    radioAllWords = new QRadioButton(tr("All words"));

    radioChooseManually = new QRadioButton(tr("Choose words manually"));
    connect(radioChooseManually, SIGNAL(clicked()),
            SLOT(openWordsChooserDialog()));

    grpWords = new QButtonGroup(this);
    grpWords->addButton(radioNumWords);
    grpWords->addButton(radioAllWords);
    grpWords->addButton(radioChooseManually);

    radioAllWords->setChecked(true);

    // Include learned words
    chckIncLearned = new QCheckBox(tr("Include learned words"));

    // Order
    radioRandomOrder = new QRadioButton(tr("Random", "Order"));
    radioListOrder   = new QRadioButton(tr("As in list"));

    QButtonGroup *grpOrder = new QButtonGroup(this);
    grpOrder->addButton(radioRandomOrder);
    grpOrder->addButton(radioListOrder);

    radioRandomOrder->setChecked(true);

    // Hiding
    radioTranslationHide = new QRadioButton(tr("Translation", "Hide"));
    radioWordHide        = new QRadioButton(tr("Word", "Hide"));
    radioRandomHide      = new QRadioButton(tr("Random", "Hide"));

    grpHideMode = new QButtonGroup(this);
    grpHideMode->addButton(radioRandomHide, HideRandom);
    grpHideMode->addButton(radioTranslationHide, HideTranslation);
    grpHideMode->addButton(radioWordHide, HideWord);

    radioRandomHide->setChecked(true);

    // Layouts
    QVBoxLayout *ltRadioMode = new QVBoxLayout;
    ltRadioMode->addWidget(radioMultiChoiceMode);
    ltRadioMode->addWidget(radioNoChoiceMode);
    ltRadioMode->addWidget(radioRandomMode);

    QHBoxLayout *ltNumWords = new QHBoxLayout;
    ltNumWords->addWidget(radioNumWords);
    ltNumWords->addWidget(txtNumWords, 1);
    ltNumWords->addWidget(radioAllWords);
    ltNumWords->addWidget(radioChooseManually);

    QVBoxLayout *ltRadioOrder = new QVBoxLayout;
    ltRadioOrder->addWidget(radioRandomOrder);
    ltRadioOrder->addWidget(radioListOrder);

    QVBoxLayout *ltRadioHide = new QVBoxLayout;
    ltRadioHide->addWidget(radioRandomHide);
    ltRadioHide->addWidget(radioTranslationHide);
    ltRadioHide->addWidget(radioWordHide);

    QFormLayout *ltFormQuiz = new QFormLayout;
    ltFormQuiz->addRow(tr("Mode:", "Quiz"), ltRadioMode);
    ltFormQuiz->addRow(new QLabel(tr("Number of words:")));
    ltFormQuiz->addRow(ltNumWords);
    ltFormQuiz->addRow(chckIncLearned);
    ltFormQuiz->addRow(tr("Order:", "Quiz"), ltRadioOrder);
    ltFormQuiz->addRow(tr("Hide:", "Quiz"), ltRadioHide);

    // Main stuff
    QGroupBox *grpBox = new QGroupBox(windowTitle());
    grpBox->setLayout(ltFormQuiz);

    QPushButton *btnOk = new QPushButton(tr("OK"));
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));

    QPushButton *btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QVBoxLayout *ltBtns = new QVBoxLayout();
    ltBtns->addWidget(btnOk);
    ltBtns->addWidget(btnCancel);
    ltBtns->addStretch(1);

    QHBoxLayout *ltMain = new QHBoxLayout();
    ltMain->addWidget(grpBox);
    ltMain->addLayout(ltBtns);

    setLayout(ltMain);
}

void StartQuizDialog::openWordsChooserDialog()
{
    dlgWordsChooser->exec();
}

void StartQuizDialog::toggleTxtNumWords(bool state)
{
    txtNumWords->setEnabled(state);
}

WordsPtrSet StartQuizDialog::cards()
{
    // Getting words from the set according to selected options
    LearnedWordFilter filter;

    WordsPtrSet tmp;

    if (radioChooseManually->isChecked())
        filter.setWords(dlgWordsChooser->cards());
    else
        filter.setWords(m_Cards);

    // Do not include learned
    if (!chckIncLearned->isChecked()) {
        tmp = filter.filter("false");
    } else {
        tmp = m_Cards;
    }

    // Random
    if (radioRandomOrder->isChecked()) {
        tmp = filter.shuffled(tmp);
    }

    // First N words
    if (radioNumWords->isChecked()) {
        tmp = filter.getFirstN(txtNumWords->text().toInt(), tmp);
    }

    return tmp;
}

QuestionType StartQuizDialog::choiceMode()
{
    return (QuestionType) grpChoiceMode->checkedId();
}

HideMode StartQuizDialog::hideMode()
{
    return (HideMode) grpHideMode->checkedId();
}
