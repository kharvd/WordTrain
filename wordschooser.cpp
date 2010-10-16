#include "wordschooser.h"
#include "utilities.h"

WordsChooser::WordsChooser(const WordsPtrSet & words, bool random,
                           bool includeLearned, int number)
{
    qsrand(time(0));

    mWords = words;

    if (random) {
        shuffle();
    }

    mWords = getFirstNOf(number, includeLearned);
}

WordsPtrSet WordsChooser::getWords()
{
    return mWords;
}

void WordsChooser::shuffle() {
    mWords = shuffleContainer(mWords, mWords.size());
}

WordsPtrSet WordsChooser::getFirstNOf(int number, bool includeLearned) {
    WordsPtrSet tmp;
    number = (number == -1) ? mWords.size() : number;

    for (int i = 0, n = 0; (i < mWords.size()) && (n < number); i++) {
        if (!(!includeLearned && mWords.at(i)->numCorrectAnswers()
                                 / WordCard::corrAnsForLearned == 1)) {
            tmp.push_back(mWords.at(i));
            n++;
        }
    }

    return tmp;
}


