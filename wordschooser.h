#ifndef WORDSCHOOSER_H
#define WORDSCHOOSER_H

#include "wordscard.h"

// Class for choosing words to be tested.
class WordsChooser
{
public:
    WordsChooser(const WordsPtrSet & words, bool random = true,
                 bool includeLearned = true, int number = -1);
    WordsPtrSet getWords();
private:
    void shuffle();
    WordsPtrSet getFirstNOf(int number, bool includeLearned);

    WordsPtrSet mWords;
};

#endif // WORDSCHOOSER_H
