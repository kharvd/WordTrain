#ifndef VIEWCARDDIALOG_H
#define VIEWCARDDIALOG_H

#include <QtGui>
#include "wordscard.h"
#include "neweditcarddialog.h"
#include "cardwidget.h"

class ViewCardDialog : public QDialog
{
    Q_OBJECT

public:
    ViewCardDialog(const WordsPtrSet & cards, QWidget *parent = 0);
    ViewCardDialog(WordsSet *cards, QWidget *parent = 0);

    void setCurrentWord(int index);
    bool isModified();

private slots:
    void nextWord();
    void prevWord();
    void editCard();
    void setCardLearned();

private:
    void createInterface();
    void enableButtons();

    /* Convenience func for setting "Learned" button text */
    void setLearnedButtonText();

    QPushButton *btnPrevious;
    QPushButton *btnTurn;
    QPushButton *btnNext;
    QPushButton *btnEdit;
    QPushButton *btnLearned;
    CardWidget *cardText;

    bool mModified;
    int mCurrCard;
    WordsPtrSet mCards;
};

#endif // VIEWCARDDIALOG_H
