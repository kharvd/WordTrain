#ifndef TAGWORDFILTER_H
#define TAGWORDFILTER_H

#include "wordfilter.h"
#include "wordcard.h"

class TagWordFilter : public WordFilter
{
public:
    TagWordFilter():
            WordFilter() { }

    virtual WordsPtrSet filter(const QString &tag, const WordsPtrSet &set);
};

#endif // TAGWORDFILTER_H
