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

#ifndef QUIZDIALOG_H
#define QUIZDIALOG_H

#include <QDialog>
#include "wordscard.h"
#include "quiz.h"

class QProgressBar;
class QPushButton;
class QLabel;
class AnswerWidget;
class CardWidget;

enum HideMode { Hide_First = 0, Hide_Random = 0, Hide_Translation, Hide_Word,
                Hide_Last = Hide_Word};

// Dialog with the quiz, similar to ViewCardDialog.
class QuizDialog : public QDialog
{
    Q_OBJECT

public:
    // This class should be able to edit contents of the cards
    QuizDialog(const WordsPtrSet & cards, QuestionType choice,
               HideMode hide, const WordsPtrSet & allCards,
               QWidget *parent = 0);
    QuizDialog(WordsSet *cards, QuestionType choice,
               HideMode hide, const WordsPtrSet & allCards,
               QWidget *parent = 0);

    bool isModified();

private slots:
    void nextCheckWord();
    void dontKnow();

private:
    static const int defaultWidth = 400;
    static const int defaultHeight = 360;
    static const int numChoices = 4;

    // Convenience function for initialization
    void constructor(QuestionType choice, HideMode hide);

    // Changes buttons' text to appropriate, when something is clicked
    void switchButtons();

    // Sets current displayed card
    void setCurrentCard(int index);

    void createInterface();

    // Selects some words for choices in MultiAnswerWidget
    QList<QString> getAnswersMultiChoice(QString correct, bool translation);

    // Checks answer
    void checkAnswer();

    // Shows result of the quiz
    void showResult();

    // Fills quiz with questions
    void fillQuiz();

    // Changes the type of the current answer widget
    // and returns pointer to current widget
    AnswerWidget * switchAnsWidget(QuestionType type);

    QPushButton *btnDontKnow;
    QProgressBar *prgProgress;
    QLabel *lblProgress;
    QPushButton *btnCheckNext;
    CardWidget *cardText;
    AnswerWidget *wgtAnswer;
    QList<AnswerWidget*> answerWgts;

    // If user answered correctly at least one question
    bool mModified;

    HideMode mHideMode;
    QuestionType mChoiceMode;

    // If user answered all words
    bool mThatsAll;

    // If "Check" has been clicked
    bool mAnswered;

    int mCurrentCard;

    // Cards to be tested
    WordsPtrSet mCards;

    // All cards of current set
    WordsPtrSet mAllCards;

    Quiz<WordCard, QString> mQuiz;
    QList<HideMode> mHideModes;
    QList<QuestionType> mQuestionTypes;
};

#endif // QUIZDIALOG_H