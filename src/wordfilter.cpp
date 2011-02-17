#include "wordfilter.h"

#include "utilities.h"

WordsPtrSet WordFilter::shuffled(const WordsPtrSet &set)
{
    return Utilities::shuffleContainer(set, set.size());
}

WordsPtrSet WordFilter::getFirstN(int n, const WordsPtrSet &set)
{
    WordsPtrSet tmp;

    for (int i = 0, size = set.size(); (i < size) && (i < n); i++) {
        tmp << set[i];
    }

    return tmp;
}
