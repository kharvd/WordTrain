#include "tagwordfilter.h"

WordsPtrSet TagWordFilter::filter(const QString &tag)
{
    WordsPtrSet result;

    foreach (WordCard *card, m_Words) {
        if (card->tags().contains(tag)) {
            result.push_back(card);
        }
    }

    return result;
}
