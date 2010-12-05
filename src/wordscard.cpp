/******************************************************************************
** WordTrain 0.9.1 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
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

#include "wordscard.h"

bool WordCard::operator==(const WordCard &card)
{
    bool result =
            (mWord == card.word()) && (mGender == card.gender())
            && (mCategory == card.category())
            && (mTranscription == card.transcription())
            && (mTranslation == card.translation())
            && (mPlural == card.plural())
            && (mExamples == card.examples())
            && (mCorrectAnswers
 == card.correctAnswers
());
    return result;
}

void WordCard::setWord(const QString & word)
{
    mWord = word.trimmed();
}

void WordCard::setTranscription(const QString & transcription)
{
    mTranscription = transcription.trimmed();
}

void WordCard::setTranslation(const QString & translation)
{
    mTranslation = translation.trimmed();
}

void WordCard::setPlural(const QString & plural)
{
    mPlural = plural.trimmed();
}

void WordCard::setCategory(LexicalCategory category)
{
    if ((category <= LC_Particle) && (category >= 0)) {
        mCategory = category;
    }
}

void WordCard::setCategory(int category)
{
    setCategory(LexicalCategory(category));
}

void WordCard::setGender(Gender gender)
{
    if ((gender <= Gen_Common) && (gender >= 0)) {
        mGender = gender;
    }
}

void WordCard::setGender(int gender)
{
    setGender(Gender(gender));
}

void WordCard::setCorrectAnswers
(unsigned int num)
{
    mCorrectAnswers
 = num;
}

void WordCard::incCorrectAnswers()
{
    mCorrectAnswers
++;
}

void WordCard::setExamples(const Examples & examples)
{
    mExamples = examples;
}

void WordCard::setExampleAt(int index, const Example & example)
{
    if ((index < mExamples.size()) && (index >= 0)) {
        mExamples[index] = example;
    }
}

void WordCard::addExample(const QString & example,
                          const QString & translation)
{
    mExamples.push_back(qMakePair(example.trimmed(), translation.trimmed()));
}

void WordCard::addExample(const Example & example)
{
    mExamples.push_back(example);
}

void WordCard::clearExamples()
{
    mExamples.clear();
}

void WordCard::clearAll()
{
    setWord("");
    setTranscription("");
    setTranslation("");
    setPlural("");
    setCategory(LC_None);
    setGender(Gen_None);
    setCorrectAnswers
(0);
    clearExamples();
}

QStringList WordCard::lexCategoriesStrings()
{
    static const QStringList list =
            (QStringList() << tr("None") << tr("Noun") << tr("Verb")
             << tr("Adjective") << tr("Pronoun") <<  tr("Adverb")
             << tr("Preposition") << tr("Conjunction") << tr("Interjection")
             << tr("Participle") << tr("Article") << tr("Clitic")
             << tr("Particle"));

    return list;
}

QStringList WordCard::genderStrings()
{
    static const QStringList list =
            (QStringList() << tr("None") << tr("Masculine") << tr("Femenine")
             << tr("Neuter") << tr("Common"));

    return list;
}

QStringList WordCard::lexCategoriesShortStrings()
{
    static const QStringList list =
            (QStringList() << tr("") << tr("noun") << tr("verb")
             << tr("adj.") << tr("pronoun") <<  tr("adverb")
             << tr("prep.") << tr("conj.") << tr("interj.")
             << tr("participle") << tr("article") << tr("clitic")
             << tr("particle"));

    return list;
}

QStringList WordCard::genderShortStrings()
{
    static const QStringList list =
            (QStringList() << tr("") << tr("m.") << tr("f.")
             << tr("n.") << tr("common"));

    return list;
}

QString WordCard::lexCategoriesString(LexicalCategory cat)
{
    return lexCategoriesStrings().at((int) cat);
}

QString WordCard::genderString(Gender gen)
{
    return genderStrings().at((int) gen);
}

QString WordCard::lexCategoriesShortString(LexicalCategory cat)
{
    return lexCategoriesShortStrings().at((int) cat);
}

QString WordCard::genderShortString(Gender gen)
{
    return genderShortStrings().at((int) gen);
}

QString WordCard::lexCategoriesShortString() const
{
    return lexCategoriesShortStrings().at((int) mCategory);
}

QString WordCard::genderShortString() const
{
    return genderShortStrings().at((int) mGender);
}
