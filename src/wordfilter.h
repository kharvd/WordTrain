#ifndef WORDFILTER_H
#define WORDFILTER_H

#include "wordcard.h"

// Abstract class for filtering words in different ways
class WordFilter
{
public:
    WordFilter() { }
    WordFilter(const WordsPtrSet &set);
    virtual ~WordFilter() { }

    virtual void setWords(const WordsPtrSet &set);
    virtual WordsPtrSet filter(const QString &parameter) = 0;

    virtual WordsPtrSet shuffled();
    virtual WordsPtrSet shuffled(const WordsPtrSet &set);

    virtual WordsPtrSet getFirstN(int n);
    virtual WordsPtrSet getFirstN(int n, const WordsPtrSet &set);

protected:
    WordsPtrSet m_Words;
};

#endif // WORDFILTER_H
