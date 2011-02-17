#ifndef SEARCHWORDFILTER_H
#define SEARCHWORDFILTER_H

#include "wordfilter.h"
#include "wordcard.h"

class SearchWordFilter : public WordFilter
{
public:
    SearchWordFilter():
            WordFilter() { }

    virtual WordsPtrSet filter(const QString &searchString,
                               const WordsPtrSet &set);
};

#endif // SEARCHWORDFILTER_H
