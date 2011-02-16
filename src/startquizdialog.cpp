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

#include "wordschooser.h"
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

    setWindowTitle(tr("Start test"));
    createInterface();
}

void StartQuizDialog::createInterface()
{
    QFormLayout *ltFormQuiz = new QFormLayout;

    // Mode
    radioMultiChoiceMode = new QRadioButton(tr("Multiple choice"));
    radioNoChoiceMode = new QRadioButton(tr("Without choice"));
    radioRandomMode = new QRadioButton(tr("Random", "Mode"));

    grpChoiceMode = new QButtonGroup(this);
    grpChoiceMode->addButton(radioMultiChoiceMode, QuestionTypeMultiChoice);
    grpChoiceMode->addButton(radioNoChoiceMode, QuestionTypeNoChoice);
    grpChoiceMode->addButton(radioRandomMode, QuestionTypeRandom);

    radioMultiChoiceMode->setChecked(true);

    QVBoxLayout *ltRadioMode = new QVBoxLayout;
    ltRadioMode->addWidget(radioMultiChoiceMode);
    ltRadioMode->addWidget(radioNoChoiceMode);
    ltRadioMode->addWidget(radioRandomMode);

    ltFormQuiz->addRow(tr("Mode:", "Quiz"), ltRadioMode);

    // Num of words
    QLabel *lblNumWords = new QLabel(tr("Number of words:"));

    txtNumWords = new QLineEdit();
    QIntValidator *validator = new QIntValidator(1, 1000, this);
    txtNumWords->setValidator(validator);

    chckAllWords = new QCheckBox(tr("All words"));
    connect(chckAllWords, SIGNAL(toggled(bool)), SLOT(toggleTxtNumWords(bool)));
    chckAllWords->setChecked(true);

    QHBoxLayout *ltNumWords = new QHBoxLayout;
    ltNumWords->addWidget(lblNumWords);
    ltNumWords->addWidget(txtNumWords, 1);
    ltNumWords->addWidget(chckAllWords);

    ltFormQuiz->addRow(ltNumWords);

    // Include learned words
    chckIncLearned = new QCheckBox(tr("Include learned words"));
    ltFormQuiz->addRow(chckIncLearned);

    // Order
    radioRandomOrder = new QRadioButton(tr("Random", "Order"));
    radioListOrder = new QRadioButton(tr("As in list"));
    QButtonGroup *grpOrder = new QButtonGroup(this);
    grpOrder->addButton(radioRandomOrder);
    grpOrder->addButton(radioListOrder);
    radioRandomOrder->setChecked(true);

    QVBoxLayout *ltRadioOrder = new QVBoxLayout;
    ltRadioOrder->addWidget(radioRandomOrder);
    ltRadioOrder->addWidget(radioListOrder);

    ltFormQuiz->addRow(tr("Order:", "Quiz"), ltRadioOrder);

    // Hiding
    radioTranslationHide = new QRadioButton(tr("Translation", "Hide"));
    radioWordHide = new QRadioButton(tr("Word", "Hide"));
    radioRandomHide = new QRadioButton(tr("Random", "Hide"));
    radioRandomHide->setChecked(true);
    grpHideMode = new QButtonGroup(this);
    grpHideMode->addButton(radioRandomHide, HideRandom);
    grpHideMode->addButton(radioTranslationHide, HideTranslation);
    grpHideMode->addButton(radioWordHide, HideWord);

    QVBoxLayout *ltRadioHide = new QVBoxLayout;
    ltRadioHide->addWidget(radioRandomHide);
    ltRadioHide->addWidget(radioTranslationHide);
    ltRadioHide->addWidget(radioWordHide);

    ltFormQuiz->addRow(tr("Hide:", "Quiz"), ltRadioHide);

    // Main stuff
    QGroupBox *grpBox = new QGroupBox(windowTitle());
    grpBox->setLayout(ltFormQuiz);

    QPushButton *btnOk = new QPushButton(tr("OK"));
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    QPushButton *btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));
    QPushButton *btnChooser = new QPushButton(tr("Blah"));
    connect(btnChooser, SIGNAL(clicked()), SLOT(openWordsChooserDialog()));

    QVBoxLayout *ltBtns = new QVBoxLayout();
    ltBtns->addWidget(btnOk);
    ltBtns->addWidget(btnCancel);
    ltBtns->addWidget(btnChooser);
    ltBtns->addStretch(1);

    QHBoxLayout *ltMain = new QHBoxLayout();
    ltMain->addWidget(grpBox);
    ltMain->addLayout(ltBtns);

    setLayout(ltMain);
}

void StartQuizDialog::openWordsChooserDialog()
{
    QuizWordsChooserDialog *dlg = new QuizWordsChooserDialog(m_Cards);
    dlg->exec();
    delete dlg;
}

void StartQuizDialog::toggleTxtNumWords(bool disable)
{
    txtNumWords->setEnabled(!disable);
}

WordsPtrSet StartQuizDialog::cards()
{
    // Getting words from the set according to selected options
    int numWords = chckAllWords->isChecked() ? -1 : txtNumWords->text().toInt();
    WordsChooser chooser(m_Cards, radioRandomOrder->isChecked(),
                         chckIncLearned->isChecked(), numWords);
    return chooser.getCards();
}

QuestionType StartQuizDialog::choiceMode()
{
    return (QuestionType)grpChoiceMode->checkedId();
}

HideMode StartQuizDialog::hideMode()
{
    return (HideMode)grpHideMode->checkedId();
}
