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

#include "quizdialog.h"

#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QProgressBar>
#include <QMessageBox>
#include <ctime>

#include "utilities.h"
#include "neweditcarddialog.h"
#include "cardwidget.h"
#include "answerwidget.h"
#include "lineanswerwidget.h"
#include "multianswerwidget.h"
#include "wordschooser.h"

QuizDialog::QuizDialog(const WordsPtrSet & cards, QuestionType choice,
                       HideMode hide, const WordsPtrSet & allCards,
                       QWidget *parent)
                           : QDialog(parent)
{
    qsrand(time(NULL));
    Q_ASSERT(cards.size());

    m_Cards = cards;
    m_AllCards = allCards;
    m_Answered = false;
    m_Modified = false;
    m_ThatsAll = false;
    m_ChoiceMode = choice;
    m_HideMode = hide;

    fillQuiz();

    createInterface();
    setCurrentCard(0);
}

void QuizDialog::createInterface()
{
    setWindowTitle(tr("Test"));
    resize(kDefaultWidth, kDefaultHeight);

    wgtCard = new CardWidget();

    btnDontKnow = new QPushButton(tr("Don't know"));
    connect(btnDontKnow, SIGNAL(clicked()), this, SLOT(dontKnow()));

    btnCheckNext = new QPushButton();
    connect(btnCheckNext, SIGNAL(clicked()), this, SLOT(nextCheckWord()));
    btnCheckNext->setDefault(true);

    prgProgress = new QProgressBar();
    prgProgress->setMinimum(0);
    prgProgress->setMaximum(m_Cards.size());
    prgProgress->setValue(0);

    lblProgress = new QLabel(QString("0/%1").arg(m_Cards.size()));

    answerWgts.push_back(new MultiAnswerWidget(this));
    answerWgts[QuestionTypeMultiChoice]->hide();
    answerWgts.push_back(new LineAnswerWidget(this));
    answerWgts[QuestionTypeNoChoice]->hide();

    if (m_ChoiceMode != QuestionTypeRandom)
        wgtAnswer = answerWgts[m_ChoiceMode];

    QHBoxLayout *prgLayout = new QHBoxLayout;
    prgLayout->addWidget(prgProgress);
    prgLayout->addWidget(lblProgress);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(btnDontKnow);
    hLayout->addWidget(btnCheckNext);

    QLabel *lbl = new QLabel(tr("Your answer"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(prgLayout);
    mainLayout->addWidget(wgtCard);
    mainLayout->addWidget(lbl);

    foreach (AnswerWidget *wgt, answerWgts) {
        mainLayout->addWidget(wgt);
        connect(wgt, SIGNAL(answered()), SLOT(nextCheckWord()));
    }

    mainLayout->addLayout(hLayout);

    setLayout(mainLayout);
}

bool QuizDialog::isModified()
{
    return m_Modified;
}

void QuizDialog::switchButtons()
{
    // If user answered or pressed 'Don't know' but haven't pressed 'Next' yet
    if (m_Answered) {
        btnDontKnow->setEnabled(false);

        if (m_CurrentCard == m_Cards.size() - 1) {
            btnCheckNext->setText(tr("Finish"));
            m_ThatsAll = true;
        } else {
            btnCheckNext->setText(tr("Next"));
        }
    } else {
        btnDontKnow->setEnabled(true);
        btnCheckNext->setText(tr("Check Answer"));
    }
}

AnswerWidget * QuizDialog::switchAnsWidget(QuestionType type)
{
    foreach (AnswerWidget *wgt, answerWgts)
        wgt->hide();

    answerWgts[type]->show();
    return answerWgts[type];
}

void QuizDialog::setCurrentCard(int index)
{
    m_CurrentCard = index;

    wgtAnswer = switchAnsWidget(m_QuestionTypes.at(index));
    wgtAnswer->clear();

    switchButtons();

    CardWidget::CardElements WOCategory = CardWidget::BackAll;
    WOCategory &= ~CardWidget::Category;

    switch (m_HideModes.at(index)) {
    case HideTranslation:
        // Don't show category
        wgtCard->showCard(*m_Cards.at(index), CardWidget::FaceSide, WOCategory);
        break;

    case HideWord:
        wgtCard->showCard(*m_Cards.at(index), CardWidget::BackSide,
                           CardWidget::BackNoForeign);
        break;

    case HideRandom:  // Impossible
        break;
    }

    wgtAnswer->setAnswers(m_Quiz.choicesAt(index));
}

void QuizDialog::nextCheckWord()
{
    if (m_ThatsAll) {
        showResult();
        close();
    } else {
        if (m_Answered) {
            m_CurrentCard++;
            m_Answered = false;
            lblProgress->setText(QString("%1/%2").arg(m_CurrentCard).
                                 arg(m_Cards.size()));
            prgProgress->setValue(prgProgress->value() + 1);
            setCurrentCard(m_CurrentCard);
        } else {
            if (m_HideModes.at(m_CurrentCard) == HideTranslation)
                wgtCard->showOtherSide();
            else
                wgtCard->showCard(CardWidget::BackSide, CardWidget::BackAll);

            checkAnswer();

            m_Answered = true;
            switchButtons();
        }
    }
}

void QuizDialog::checkAnswer()
{
    bool correct = m_Quiz.setUsersAnswerAt(m_CurrentCard, wgtAnswer->answer());
    wgtAnswer->setCorrect(correct);

    if (correct) {
        m_Cards.at(m_CurrentCard)->incCorrectAnswers();
        m_Modified = true;
    }
}

void QuizDialog::showResult()
{
    QMessageBox msgBox;
    QString msg;

    double mark = (m_Quiz.correctAnswers() * 5.0) / m_Cards.size();
    mark = (mark < 2) ? 2 : mark;

    msg.append(tr("Correct answers: %1/%2\n").arg(m_Quiz.correctAnswers())
                  .arg(m_Cards.size()));
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
    if (m_HideModes.at(m_CurrentCard))
        wgtCard->showOtherSide();
    else
        wgtCard->showCard(CardWidget::BackSide, CardWidget::BackAll);

    wgtAnswer->setCorrect(false);
    m_Answered = true;
    switchButtons();
}

QList<QString> QuizDialog::getAnswersMultiChoice(QString correct, bool translation)
{
    // Shuffle words
    WordsPtrSet wrds = Utilities::shuffleContainer(m_AllCards, m_AllCards.size());

    QStringList tmp;
    QString choice;

    // FIXME It's not very beatiful part here...
    /* Choosing numChoices - 1 words for answers in MultiChoice. n - general
     counter, goes through the whole list; i counts selected answers.
     Answers should contain only one correct answer, so if chosen word
     equals correct, continue the loop without incrementing i.*/
    for (int i = 0, n = 0; (n < (wrds.size())) && (i < kNumOfChoices - 1); i++, n++) {
        choice = (translation ? wrds.at(n)->translation() : wrds.at(n)->word());
        if (choice == correct)
            i--;
        else
            tmp << choice;
    }

    // Pushing the correct answer
    tmp << correct;

    // Shuffling answers
    tmp = Utilities::shuffleContainer(tmp, tmp.size());

    return tmp;
}

void QuizDialog::fillQuiz()
{
    QString correct;

    m_HideModes.clear();

    for (int i = 0; i < m_Cards.size(); i++) {
        HideMode hMode;
        QuestionType chMode;

        if (m_HideMode == HideRandom)
            hMode = HideMode(qrand() % HideLast + 1);
        else
            hMode = m_HideMode;

        if (m_ChoiceMode == QuestionTypeRandom)
            chMode = QuestionType(qrand() % QuestionTypeLast);
        else
            chMode = m_ChoiceMode;

        bool translation = (hMode == HideTranslation);

        if (translation)
            correct = m_Cards.at(i)->translation();
        else
            correct = m_Cards.at(i)->word();

        m_Quiz.addQuestion(*m_Cards.at(i), correct,
                          getAnswersMultiChoice(correct, translation),
                          chMode);

        m_HideModes.push_back(hMode);
        m_QuestionTypes.push_back(chMode);
    }
}
