#ifndef TAGWORDFILTER_H
#define TAGWORDFILTER_H

#include "wordfilter.h"
#include "wordscard.h"

class TagWordFilter : public WordFilter
{
public:
    TagWordFilter():
            WordFilter() { }
    TagWordFilter(const WordsPtrSet &set):
            WordFilter(set) { }

    virtual WordsPtrSet filter(const QString &tag);
};

#endif // TAGWORDFILTER_H
