#ifndef QUIZDIALOG_H
#define QUIZDIALOG_H

#include <QtGui>
#include "wordscard.h"
#include "neweditcarddialog.h"
#include "cardwidget.h"
#include "answerwidget.h"
#include "lineanswerwidget.h"

enum QuizMode { Mode_MultiChoice = 0, Mode_NoChoice };

class QuizDialog : public QDialog
{
    Q_OBJECT
public:
    QuizDialog(const WordsPtrSet & cards, QuizMode mode, QWidget *parent = 0);
    QuizDialog(WordsSet *cards, QuizMode mode, QWidget *parent = 0);

private slots:
    void nextCheckWord();
    void dontKnow();

private:
    void switchButtons();
    void setCurrentWord(int index);
    void createInterface();

    QPushButton *btnDontKnow;
    QPushButton *btnCheckNext;
    CardWidget *cardText;
    AnswerWidget *wgtAnswer;

    QuizMode mMode;
    bool mThatsAll; // If there is no more words
    bool mAnswered; // If "Check" has been clicked
    int mCurrCard;
    WordsPtrSet mCards;
};

#endif // QUIZDIALOG_H
