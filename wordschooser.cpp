/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov (kharvd@gmail.com)
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "wordschooser.h"
#include "utilities.h"

WordsChooser::WordsChooser(const WordsPtrSet & words, bool random,
                           bool includeLearned, int number)
{
    qsrand(time(0));

    mWords = words;

    if (random) {
        shuffle();
    }

    mWords = getFirstNOf(number, includeLearned);
}

WordsPtrSet WordsChooser::getWords()
{
    return mWords;
}

void WordsChooser::shuffle()
{
    mWords = shuffleContainer(mWords, mWords.size());
}

WordsPtrSet WordsChooser::getFirstNOf(int number, bool includeLearned)
{
    WordsPtrSet tmp;
    number = (number == -1) ? mWords.size() : number;

    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();

    for (int i = 0, n = 0; (i < mWords.size()) && (n < number); i++) {
        if (!(!includeLearned && mWords.at(i)->numCorrectAnswers()
                                 / corrAnsForLearned == 1)) {
            tmp.push_back(mWords.at(i));
            n++;
        }
    }

    return tmp;
}


