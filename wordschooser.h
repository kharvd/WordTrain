#ifndef WORDSCHOOSER_H
#define WORDSCHOOSER_H

#include "wordscard.h"
#include <QtAlgorithms>

// Class for choosing words to be tested.
class WordsChooser
{
public:
    WordsChooser(const WordsPtrSet & words, bool random, bool includeLearned,
                 int number);
    WordsPtrSet getWords();
private:
    void shuffle();
    WordsPtrSet getFirstNOf(int number, bool includeLearned);

    WordsPtrSet mWords;
};

#endif // WORDSCHOOSER_H
