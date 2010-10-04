#ifndef QUIZDIALOG_H
#define QUIZDIALOG_H

#include <QtGui>
#include <cstdlib>
#include "wordscard.h"
#include "neweditcarddialog.h"
#include "cardwidget.h"
#include "answerwidget.h"
#include "lineanswerwidget.h"

enum ChoiceMode { Choice_MultiChoice = 0, Choice_NoChoice };
enum HideMode { Hide_Translation = 0, Hide_Word, Hide_Random };

class QuizDialog : public QDialog
{
    Q_OBJECT
public:
    QuizDialog(const WordsPtrSet & cards, ChoiceMode choice,
               HideMode hide, QWidget *parent = 0);
    QuizDialog(WordsSet *cards, ChoiceMode choice,
               HideMode hide, QWidget *parent = 0);

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

    HideMode mHideMode;
    ChoiceMode mChoiceMode;
    bool mHideTranslation;
    bool mThatsAll; // If there is no more words
    bool mAnswered; // If "Check" has been clicked
    int mCurrCard;
    WordsPtrSet mCards;
};

#endif // QUIZDIALOG_H
