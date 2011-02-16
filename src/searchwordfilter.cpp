#include "searchwordfilter.h"

WordsPtrSet SearchWordFilter::filter(const QString &searchString)
{
    WordsPtrSet result;

    if (!searchString.isEmpty()) {
        foreach (WordCard *card, m_Words) {
            if (card->word().contains(searchString, Qt::CaseInsensitive)
             || card->translation().contains(searchString, Qt::CaseInsensitive))
            {
                result.push_back(card);
            }
        }
    }

    return result;
}
