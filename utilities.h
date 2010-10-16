#ifndef UTILITIES_H
#define UTILITIES_H

#include <ctime>
#include <QtCore>
#include <QtAlgorithms>

// Generick shuffle
template <class T>
T shuffleContainer(const T & container, int size) {
    T tmp = container;

    int newPos;

    for (int i = 0; i < size; i++) {
        newPos = qrand() % size;
        qSwap(tmp[i], tmp[newPos]);
    }

    return tmp;
}

#endif // UTILITIES_H
