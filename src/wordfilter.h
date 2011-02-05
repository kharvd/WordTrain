#ifndef WORDFILTER_H
#define WORDFILTER_H

#include "wordscard.h"

// Abstract class for filtering words in different ways
class WordFilter
{
public:
    WordFilter() { }
    WordFilter(const WordsPtrSet &set);
    virtual ~WordFilter() { }

    virtual void setWords(const WordsPtrSet &set);
    virtual WordsPtrSet filter(const QString &parameter) = 0;

protected:
    WordsPtrSet m_Words;
};

#endif // WORDFILTER_H
