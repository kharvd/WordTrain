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

#include "quizdialog.h"
#include "utilities.h"

QuizDialog::QuizDialog(const WordsPtrSet & cards, ChoiceMode choice,
                       HideMode hide, const WordsPtrSet & allCards,
                       QWidget *parent)
                           : QDialog(parent)
{
    Q_ASSERT(cards.size());
    mCards = cards;
    mAllCards = allCards;
    constructor(choice, hide);
}

QuizDialog::QuizDialog(WordsSet *cards, ChoiceMode choice, HideMode hide,
                       const WordsPtrSet & allCards, QWidget *parent)
    : QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mAllCards = allCards;
    constructor(choice, hide);
}

void QuizDialog::constructor(ChoiceMode choice, HideMode hide)
{
    qsrand(time(0));
    mCorrectAnswers = 0;
    mCardsNumber = mCards.size();
    mAnswered = false;
    mModified = false;
    mThatsAll = false;
    mChoiceMode = choice;
    mHideMode = hide;
    if (mHideMode == Hide_Translation) {
        mHideTranslation = true;
    } else {
        mHideTranslation = false;
    }

    createInterface();
    setCurrentWord(0);
}

void QuizDialog::createInterface()
{
    setWindowTitle(tr("Test"));
    resize(400, 360);

    cardText = new CardWidget();

    btnDontKnow = new QPushButton(tr("Don't know"));
    connect(btnDontKnow, SIGNAL(clicked()), this, SLOT(dontKnow()));

    btnCheckNext = new QPushButton();
    connect(btnCheckNext, SIGNAL(clicked()), this, SLOT(nextCheckWord()));
    btnCheckNext->setDefault(true);

    prgProgress = new QProgressBar();
    prgProgress->setMinimum(0);
    prgProgress->setMaximum(mCardsNumber);
    prgProgress->setValue(0);

    lblProgress = new QLabel(QString("0/%1").arg(mCardsNumber));

    switch (mChoiceMode) {
    case Choice_NoChoice:
        wgtAnswer = new LineAnswerWidget();
        break;
    case Choice_MultiChoice:
        wgtAnswer = new MultiAnswerWidget();
        break;
    }

    Q_ASSERT(wgtAnswer);

    QHBoxLayout *prgLayout = new QHBoxLayout;
    prgLayout->addWidget(prgProgress);
    prgLayout->addWidget(lblProgress);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(btnDontKnow);
    hLayout->addWidget(btnCheckNext);

    QLabel *lbl = new QLabel(tr("Your answer"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(prgLayout);
    mainLayout->addWidget(cardText);
    mainLayout->addWidget(lbl);
    mainLayout->addWidget(wgtAnswer);
    mainLayout->addLayout(hLayout);

    setLayout(mainLayout);
}

bool QuizDialog::isModified()
{
    return mModified;
}

void QuizDialog::switchButtons()
{
    if (mAnswered) {
        btnDontKnow->setEnabled(false);

        if (mCurrCard == mCards.size() - 1) {
            btnCheckNext->setText(tr("Finish"));
            mThatsAll = true;
        } else {
            btnCheckNext->setText(tr("Next"));
        }
    } else {
        btnDontKnow->setEnabled(true);
        btnCheckNext->setText(tr("Check Answer"));
    }
}

void QuizDialog::setCurrentWord(int index)
{
    mCurrCard = index;
    wgtAnswer->clear();
    switchButtons();

    switch (mHideMode) {
    case Hide_Translation:
        cardText->showWord(*mCards.at(index));
        break;
    case Hide_Word:
        cardText->showWord(*mCards.at(index), false, false);
        break;
    case Hide_Random:
        mHideTranslation = qrand() % 2;
        cardText->showWord(*mCards.at(index), mHideTranslation, mHideTranslation);
        break;
    }

    if (mChoiceMode == Choice_MultiChoice) {
        QString correctAnswer
                = mHideTranslation ? mCards.at(mCurrCard)->translation()
                                   : mCards.at(mCurrCard)->word();
        qobject_cast<MultiAnswerWidget *>(wgtAnswer)->
                setAnswers(getAnswersMultiChoice(correctAnswer,
                                                 mHideTranslation));
    }
}

void QuizDialog::nextCheckWord()
{
    if (mThatsAll) {
        showResult();
        close();
    } else {
        if (mAnswered) {
            mCurrCard++;
            mAnswered = false;
            lblProgress->setText(QString("%1/%2").arg(mCurrCard).
                                 arg(mCardsNumber));
            prgProgress->setValue(prgProgress->value() + 1);
            setCurrentWord(mCurrCard);
        } else {
            if (!mHideTranslation) {
                cardText->showWord(false, true);
            } else {
                cardText->showOtherSide();
            }

            checkAnswer();

            mAnswered = true;
            switchButtons();
        }
    }
}

void QuizDialog::checkAnswer()
{
    QString correctAnswer
            = mHideTranslation ? mCards.at(mCurrCard)->translation()
                               : mCards.at(mCurrCard)->word();

    bool correct = ((correctAnswer.indexOf(wgtAnswer->getAnswer(), 0,
                            Qt::CaseInsensitive)) != -1 )
                            && (!(wgtAnswer->getAnswer().isEmpty()));

    wgtAnswer->setCorrect(correct);

    if (correct) {
        mCorrectAnswers++;
        mCards.at(mCurrCard)->incCorrectAnswers();
        mModified = true;
    }
}

void QuizDialog::showResult()
{
    QMessageBox msgBox;
    QString msg;
    double mark = (mCorrectAnswers * 5.0) / mCardsNumber;

    msg.append(tr("Correct answers: %1/%2\n").arg(mCorrectAnswers)
                  .arg(mCards.size()));
    msg.append(tr("Your mark: %1").arg(mark));

    msgBox.setText(msg);
    msgBox.setWindowTitle(tr("Your result"));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setStandardButtons(QMessageBox::Close);
    msgBox.setDefaultButton(QMessageBox::Close);
    msgBox.exec();
}

void QuizDialog::dontKnow()
{
    if (!mHideTranslation) {
        cardText->showWord(false, true);
    } else {
        cardText->showOtherSide();
    }

    wgtAnswer->setCorrect(false);
    mAnswered = true;
    switchButtons();
}

QStringList QuizDialog::getAnswersMultiChoice(QString correct, bool translation)
{
    WordsPtrSet wrds = shuffleContainer(mAllCards, mAllCards.size());

    QStringList tmp;
    QString choice;
    for (int i = 0, n = 0; (n < (wrds.size())) && (i < numChoices - 1); i++, n++) {
        choice = (translation ? wrds.at(n)->translation() : wrds.at(n)->word());
        if (choice == correct) {
            i--;
            continue;
        }
        tmp << choice;
    }

    tmp << correct;
    tmp = shuffleContainer(tmp, tmp.size());

    return tmp;
}
