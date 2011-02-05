#include "wordfilter.h"

WordFilter::WordFilter(const WordsPtrSet &set)
{
    setWords(set);
}

void WordFilter::setWords(const WordsPtrSet &set)
{
    m_Words = set;
}
