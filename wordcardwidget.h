#ifndef WORDCARDWIDGET_H
#define WORDCARDWIDGET_H

#include <QtGui>
#include "wordscard.h"
#include "neweditcarddialog.h"

enum WordCardShowMode { SM_HideNone, SM_HideTranslation, SM_HideWord, SM_HideRandom };

class WordCardWidget : public QDialog
{
    Q_OBJECT

public:
    WordCardWidget(const WordsPtrSet & cards, WordCardShowMode mode
                   = SM_HideNone, QWidget *parent = 0);
    WordCardWidget(WordsSet *cards, WordCardShowMode mode
                   = SM_HideNone, QWidget *parent = 0);

    void setCurrentWord(int index);
    bool isModified();

private slots:
    void nextWord();
    void showAnswer();
    void prevWord();
    void editCard();
    void setCardLearned();

private:
    void createInterface();
    void enableButtons();

    /* Convenience func for setting "Learned" button text */
    void setLearnedButtonText();

    QPushButton *btnPrevious;
    QPushButton *btnAnswer;
    QPushButton *btnNext;
    QPushButton *btnEdit;
    QPushButton *btnLearned;
    QTextEdit *txtCardText;

    bool mModified;
    WordCardShowMode mShowMode;
    int mCurrCard;
    WordsPtrSet mCards;
};

#endif // WORDCARDWIDGET_H
