#ifndef WORDCARDWIDGET_H
#define WORDCARDWIDGET_H

#include <QtGui>
#include "wordscard.h"
#include "neweditcarddialog.h"

class WordCardWidget : public QDialog
{
    Q_OBJECT

public:
    WordCardWidget(const WordsPtrSet & cards, QWidget *parent = 0);
    WordCardWidget(WordsSet *cards, QWidget *parent = 0);

    void setCurrentWord(int index, bool faceSide = true);
    bool isModified();

private slots:
    void nextWord();
    void showOtherSide();
    void prevWord();
    void editCard();
    void setCardLearned();

private:
    void createInterface();
    void enableButtons();
    QString getCSS();

    /* Convenience func for setting "Learned" button text */
    void setLearnedButtonText();

    QPushButton *btnPrevious;
    QPushButton *btnTurn;
    QPushButton *btnNext;
    QPushButton *btnEdit;
    QPushButton *btnLearned;
    QTextEdit *txtCardText;

    bool isFace;
    bool mModified;
    int mCurrCard;
    WordsPtrSet mCards;
};

#endif // WORDCARDWIDGET_H
