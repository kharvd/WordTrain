#ifndef SEARCHWORDFILTER_H
#define SEARCHWORDFILTER_H

#include "wordfilter.h"
#include "wordscard.h"

class SearchWordFilter : public WordFilter
{
public:
    SearchWordFilter():
            WordFilter() { }
    SearchWordFilter(const WordsPtrSet &set):
            WordFilter(set) { }

    virtual WordsPtrSet filter(const QString &searchString);
};

#endif // SEARCHWORDFILTER_H
