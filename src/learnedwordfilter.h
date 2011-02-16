#ifndef LEARNEDWORDFILTER_H
#define LEARNEDWORDFILTER_H

#include "wordfilter.h"

class LearnedWordFilter : public WordFilter
{
public:
    LearnedWordFilter():
            WordFilter() { }
    LearnedWordFilter(const WordsPtrSet &set):
            WordFilter(set) { }

    // Parameter is used as bool. Shows which words we should put into result.
    // If "true" we put only learned words, otherwise only not learned
    virtual WordsPtrSet filter(const QString &lrnd);
};

#endif // LEARNEDWORDFILTER_H
