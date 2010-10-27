#ifndef QUIZDIALOG_H
#define QUIZDIALOG_H

#include <QtGui>
#include <ctime>
#include "wordscard.h"
#include "neweditcarddialog.h"
#include "cardwidget.h"
#include "answerwidget.h"
#include "lineanswerwidget.h"
#include "multianswerwidget.h"
#include "wordschooser.h"

enum ChoiceMode { Choice_MultiChoice = 0, Choice_NoChoice };
enum HideMode { Hide_Random = 0, Hide_Translation, Hide_Word };

class QuizDialog : public QDialog
{
    Q_OBJECT
public:
    QuizDialog(const WordsPtrSet & cards, ChoiceMode choice,
               HideMode hide, const WordsPtrSet & allCards,
               QWidget *parent = 0);
    QuizDialog(WordsSet *cards, ChoiceMode choice,
               HideMode hide, const WordsPtrSet & allCards,
               QWidget *parent = 0);
    bool isModified();

private slots:
    void nextCheckWord();
    void dontKnow();

private:
    static const int numChoices = 4;

    void constructor(ChoiceMode choice, HideMode hide);
    void switchButtons();
    void setCurrentWord(int index);
    void createInterface();
    QStringList getAnswersMultiChoice(QString correct, bool translation);
    void checkAnswer();
    void showResult();

    QPushButton *btnDontKnow;
    QProgressBar *prgProgress;
    QLabel *lblProgress;
    QPushButton *btnCheckNext;
    CardWidget *cardText;
    AnswerWidget *wgtAnswer;

    int mCardsNumber;
    int mCorrectAnswers;
    bool mModified;
    HideMode mHideMode;
    ChoiceMode mChoiceMode;
    bool mHideTranslation;
    bool mThatsAll; // If there is no more words
    bool mAnswered; // If "Check" has been clicked
    int mCurrCard;
    WordsPtrSet mCards;
    WordsPtrSet mAllCards;
};

#endif // QUIZDIALOG_H
