#include "searchwordfilter.h"

WordsPtrSet SearchWordFilter::filter(const QString &searchString,
                                     const WordsPtrSet &set)
{
    WordsPtrSet result;

    if (!searchString.isEmpty()) {
        foreach (WordCard *card, set) {
            if (card->word().contains(searchString, Qt::CaseInsensitive)
             || card->translation().contains(searchString, Qt::CaseInsensitive))
            {
                result.push_back(card);
            }
        }
    }

    return result;
}
