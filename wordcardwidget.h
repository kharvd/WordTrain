#ifndef WORDCARDWIDGET_H
#define WORDCARDWIDGET_H

#include <QtGui>
#include "wordscard.h"

enum WordCardShowMode { SM_HideNone, SM_HideTranslation, SM_HideWord, SM_HideRandom };

class WordCardWidget : public QWidget
{
    Q_OBJECT

//public:
//    WordCardWidget(const WordsPtrSet & cards, WordCardShowMode mode = SM_HideNone, QWidget *parent = 0);

//    setCurrentWord(int index);
//    setCurrentWord(const WordsPtrSet::iterator & iterator);
//signals:

//private slots:
//    void nextWord();
//    void prevWord();

//private:
//    void createInterface(WordCardShowMode mode = SM_HideNone);

//    WordsPtrSet::iterator mCurrWord;
//    WordsPtrSet mCards;
};

#endif // WORDCARDWIDGET_H
