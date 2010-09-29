#include "wordscard.h"

WordCard::WordCard(const WordCard & card)
{
    mWord = card.mWord;
    mCategory = card.mCategory;
    mGender = card.mGender;
    mTranscription = card.mTranscription;
    mTranslation = card.mTranslation;
    mPlural = card.mPlural;
    mExamples = card.mExamples;
    mLearned = card.mLearned;
}

WordCard::WordCard(const QString & word, const QString & translation)
    : mGender(Gen_None), mCategory(LC_None), mLearned(false)
{
    mWord = word.trimmed();
    mTranslation = translation;
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

void WordCard::setGender(Gender gender) {
    if ((gender <= Gen_Common) && (gender >= 0)) {
        mGender = gender;
    }
}

void WordCard::setGender(int gender)
{
    setGender(Gender(gender));
}

void WordCard::setLearned(bool learned)
{
    mLearned = learned;
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
    setLearned(false);
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

QString WordCard::lexCategoriesString(LexicalCategory cat) {
    return lexCategoriesStrings().at((int) cat);
}

QString WordCard::genderString(Gender gen) {
    return genderStrings().at((int) gen);
}

QString WordCard::lexCategoriesShortString(LexicalCategory cat) {
    return lexCategoriesShortStrings().at((int) cat);
}

QString WordCard::genderShortString(Gender gen) {
    return genderShortStrings().at((int) gen);
}

QString WordCard::lexCategoriesShortString() {
    return lexCategoriesShortStrings().at((int) mCategory);
}

QString WordCard::genderShortString() {
    return genderShortStrings().at((int) mGender);
}
