/******************************************************************************
** WordTrain 0.9.2 -- Foreign words trainer
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

#ifndef WORDCARD_H
#define WORDCARD_H

#include <QtCore>

class WordCard;

typedef QList<WordCard> WordsSet;
typedef QList<WordCard *> WordsPtrSet;

/* The first element in the pair is
 * example itself and the second is its translation */
typedef QPair<QString, QString> Example;
typedef QList<Example> Examples;
typedef QSet<QString> Tags;

enum LexicalCategory {
    CategoryFirst = 0,
    CategoryNone = 0, CategoryNoun, CategoryVerb,
    CategoryAdjective, CategoryPronoun, CategoryAdverb, CategoryPreposition,
    CategoryConjunction, CategoryInterjection, CategoryParticiple,
    CategoryArticle, CategoryClitic, CategoryParticle,
    CategoryLast = CategoryParticle
};

enum Gender {
    GenderFirst = 0, GenderNone = 0, GenderMasculine, GenderFemenine,
    GenderNeuter, GenderCommon, GenderLast = GenderCommon
};

/* Word card class */
class WordCard
{
    // For using 'tr()'
    Q_DECLARE_TR_FUNCTIONS(WordCard)

public:
    /* Constructors */
    WordCard():
        m_Gender(GenderNone), m_Category(CategoryNone), m_CorrectAnswers(0) {}

    /* Operators */
    bool operator==(const WordCard &card);

    /* Setters */
    void setWord(const QString & word);
    void setTranscription(const QString & transcription);
    void setTranslation(const QString & translation);
    void setPlural(const QString & plural);
    void setCategory(LexicalCategory category);
    void setGender(Gender gender);
    void setCategory(int category);
    void setGender(int gender);
    void setCorrectAnswers(int num);
    void incCorrectAnswers();
    void addExample(const QString & example, const QString & translation);
    void addExample(const Example & example);
    void clearExamples();
    void setTags(const QStringList &tags);
    void addTag(const QString &tag);
    void clearTags();

    /* Getters */
    const QString word() const { return m_Word; }
    const QString transcription() const { return m_Transcription; }
    const QString translation() const { return m_Translation; }
    const QString plural() const { return m_Plural; }
    LexicalCategory category() const { return m_Category; }
    Gender gender() const { return m_Gender; }
    int correctAnswers() const { return m_CorrectAnswers; }
    const Example exampleAt(int index) const { return m_Examples[index]; }
    const Examples examples() const { return m_Examples; }
    int examplesSize() const { return m_Examples.size(); }
    const Tags tags() const { return m_Tags; }

    // Returns short string interpretations of LexicalCategory
    QString lexCategoriesShortString() const;
    // String short interpretations of Gender
    QString genderShortString() const;

    // Returns string representations of LexicalCategory enum values
    static QStringList lexCategoriesStrings();
    static QString lexCategoriesString(LexicalCategory cat);

    // String representations of Gender enum values
    static QStringList genderStrings();
    static QString genderString(Gender gen);

    // Returns short string representations of LexicalCategory enum values
    static QStringList lexCategoriesShortStrings();
    static QString lexCategoriesShortString(LexicalCategory cat);

    // String short representations of Gender enum values
    static QStringList genderShortStrings();
    static QString genderShortString(Gender gen);

private:
    QString m_Word;
    Gender m_Gender;
    LexicalCategory m_Category;
    QString m_Transcription;
    QString m_Translation;
    QString m_Plural;
    Examples m_Examples;
    Tags m_Tags;
    int m_CorrectAnswers;
};

#endif // WORDCARD_H
