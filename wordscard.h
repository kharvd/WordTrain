/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov
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
******************************************************************************/

#ifndef WORDSCARD_H
#define WORDSCARD_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QStringList>
#include <QCoreApplication>

class WordCard;

typedef QList<WordCard> WordsSet;
typedef QList<WordCard *> WordsPtrSet;

/* The first element in the pair is
 * example itself and the second is its translation */
typedef QPair<QString, QString> Example;
typedef QVector<Example> Examples;

/* If something added or deleted, don't forget
to change condition in bounds checking */
enum LexicalCategory {
    LC_None, LC_Noun, LC_Verb, LC_Adjective, LC_Pronoun, LC_Adverb,
    LC_Preposition, LC_Conjunction, LC_Interjection, LC_Participle, LC_Article,
    LC_Clitic, LC_Particle
};

enum Gender {
    Gen_None, Gen_Masculine, Gen_Femenine, Gen_Neuter, Gen_Common
};

/* Words card class */
class WordCard
{
    // For using 'tr()'
    Q_DECLARE_TR_FUNCTIONS(WordCard)
public:
    /* Constructors */
    WordCard():
            mNumCorrectAnswers(0), mGender(Gen_None), mCategory(LC_None) {}

    /* Mutators */
    void setWord(const QString & word);
    void setTranscription(const QString & transcription);
    void setTranslation(const QString & translation);
    void setPlural(const QString & plural);
    void setCategory(LexicalCategory category);
    void setGender(Gender gender);
    void setCategory(int category);
    void setGender(int gender);
    void setNumCorrectAnswers(unsigned int num);
    void incCorrectAnswers();
    void setExamples(const Examples & examples);
    void setExampleAt(int index, const Example & example);
    void addExample(const QString & example, const QString & translation);
    void addExample(const Example & example);
    void clearAll();
    void clearExamples();

    /* Getters */
    const QString word() const { return mWord; }
    const QString transcription() const { return mTranscription; }
    const QString translation() const { return mTranslation; }
    const QString plural() const { return mPlural; }
    LexicalCategory category() const { return mCategory; }
    Gender gender() const { return mGender; }
    unsigned int numCorrectAnswers() const { return mNumCorrectAnswers; }
    const Example exampleAt(int index) const { return mExamples[index]; }
    const Examples examples() const { return mExamples; }
    int examplesSize() const { return mExamples.size(); }
    // Returns string interpretations of LexicalCategory enum values
    static QStringList lexCategoriesStrings();
    static QString lexCategoriesString(LexicalCategory cat);
    // String interpretations of Gender enum values
    static QStringList genderStrings();
    static QString genderString(Gender gen);

    // Returns short string interpretations of LexicalCategory enum values
    static QStringList lexCategoriesShortStrings();
    static QString lexCategoriesShortString(LexicalCategory cat);
    // String short interpretations of Gender enum values
    static QStringList genderShortStrings();
    static QString genderShortString(Gender gen);

    // Returns short string interpretations of LexicalCategory
    QString lexCategoriesShortString() const;
    // String short interpretations of Gender
    QString genderShortString() const;

    // TODO: Move it into 'settings'
    static const int corrAnsForLearned = 10;

private:
    unsigned int mNumCorrectAnswers;
    QString mWord;
    Gender mGender;
    LexicalCategory mCategory;
    QString mTranscription;
    QString mTranslation;
    QString mPlural;
    Examples mExamples;
};

#endif // WORDSCARD_H
