#include "learnedwordfilter.h"

WordsPtrSet LearnedWordFilter::filter(const QString &lrnd,
                                      const WordsPtrSet &set)
{
    bool learned = (lrnd == "true");

    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();

    WordsPtrSet result;
    foreach (WordCard *c, set) {
        if ((c->correctAnswers() >= corrAnsForLearned) == learned) {
            result << c;
        }
    }

    return result;
}
