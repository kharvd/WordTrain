#include "tagwordfilter.h"

WordsPtrSet TagWordFilter::filter(const QString &tag, const WordsPtrSet &set)
{
    WordsPtrSet result;

    foreach (WordCard *card, set) {
        if (card->tags().contains(tag)) {
            result << card;
        }
    }

    return result;
}
