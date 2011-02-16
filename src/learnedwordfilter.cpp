#include "learnedwordfilter.h"

WordsPtrSet LearnedWordFilter::filter(const QString &lrnd)
{
    bool learned = (lrnd == "true");

    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();

    WordsPtrSet result;
    foreach (WordCard *c, m_Words) {
        if ((c->correctAnswers() >= corrAnsForLearned) == learned) {
            result << c;
        }
    }

    return result;
}
