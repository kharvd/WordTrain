#ifndef WORDFILTER_H
#define WORDFILTER_H

#include "wordcard.h"

// Abstract class for filtering words in different ways
class WordFilter
{
public:
    WordFilter() { }
    virtual ~WordFilter() { }

    virtual WordsPtrSet filter(const QString &parameter,
                               const WordsPtrSet &set) = 0;

    virtual WordsPtrSet shuffled(const WordsPtrSet &set);

    virtual WordsPtrSet getFirstN(int n, const WordsPtrSet &set);
};

#endif // WORDFILTER_H
