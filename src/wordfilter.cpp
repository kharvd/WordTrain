#include "wordfilter.h"

#include "utilities.h"

WordFilter::WordFilter(const WordsPtrSet &set)
{
    setWords(set);
}

void WordFilter::setWords(const WordsPtrSet &set)
{
    m_Words = set;
}

WordsPtrSet WordFilter::shuffled()
{
    return shuffled(m_Words);
}

WordsPtrSet WordFilter::shuffled(const WordsPtrSet &set)
{
    return Utilities::shuffleContainer(set, set.size());
}

WordsPtrSet WordFilter::getFirstN(int n)
{
    return getFirstN(n, m_Words);
}

WordsPtrSet WordFilter::getFirstN(int n, const WordsPtrSet &set)
{
    WordsPtrSet tmp;

    for (int i = 0, size = set.size(); (i < size) && (i < n); i++) {
        tmp << set[i];
    }

    return tmp;
}
