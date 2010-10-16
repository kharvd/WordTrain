#include "quizdialog.h"
#include "utilities.h"

QuizDialog::QuizDialog(const WordsPtrSet & cards, ChoiceMode choice,
                       HideMode hide, const WordsPtrSet & allCards,
                       QWidget *parent)
                           : QDialog(parent)
{
    Q_ASSERT(cards.size());
    mCards = cards;
    mAllCards = allCards;
    constructor(choice, hide);
}

QuizDialog::QuizDialog(WordsSet *cards, ChoiceMode choice, HideMode hide,
                       const WordsPtrSet & allCards, QWidget *parent)
    : QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mAllCards = allCards;
    constructor(choice, hide);
}

void QuizDialog::constructor(ChoiceMode choice, HideMode hide)
{
    qsrand(time(0));
    mAnswered = false;
    mModified = false;
    mThatsAll = false;
    mChoiceMode = choice;
    mHideMode = hide;
    if (mHideMode == Hide_Translation) {
        mHideTranslation = true;
    } else {
        mHideTranslation = false;
    }

    createInterface();
    setCurrentWord(0);
}

void QuizDialog::createInterface()
{
    setWindowTitle(tr("Test"));
    resize(400, 360);

    cardText = new CardWidget();

    btnDontKnow = new QPushButton(tr("Don't know"));
    connect(btnDontKnow, SIGNAL(clicked()), this, SLOT(dontKnow()));

    btnCheckNext = new QPushButton();
    connect(btnCheckNext, SIGNAL(clicked()), this, SLOT(nextCheckWord()));
    btnCheckNext->setDefault(true);

    switch (mChoiceMode) {
    case Choice_NoChoice:
        wgtAnswer = new LineAnswerWidget();
        break;
    case Choice_MultiChoice:
        wgtAnswer = new MultiAnswerWidget();
        break;
    }

    Q_ASSERT(wgtAnswer);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(btnDontKnow);
    hLayout->addWidget(btnCheckNext);

    QLabel *lbl = new QLabel(tr("Your answer"));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(cardText);
    mainLayout->addWidget(lbl);
    mainLayout->addWidget(wgtAnswer);
    mainLayout->addLayout(hLayout);

    setLayout(mainLayout);
}

bool QuizDialog::isModified()
{
    return mModified;
}

void QuizDialog::switchButtons()
{
    if (mAnswered) {
        btnDontKnow->setEnabled(false);

        if (mCurrCard == mCards.size() - 1) {
            btnCheckNext->setText(tr("Close"));
            mThatsAll = true;
        } else {
            btnCheckNext->setText(tr("Next"));
        }

    } else {
        btnDontKnow->setEnabled(true);
        btnCheckNext->setText(tr("Check Answer"));
    }
}

void QuizDialog::setCurrentWord(int index)
{
    mCurrCard = index;
    wgtAnswer->clear();
    switchButtons();

    switch (mHideMode) {
    case Hide_Translation:
        cardText->showWord(*mCards.at(index));
        break;
    case Hide_Word:
        cardText->showWord(*mCards.at(index), false, false);
        break;
    case Hide_Random:
        mHideTranslation = qrand() % 2;
        cardText->showWord(*mCards.at(index), mHideTranslation, mHideTranslation);
        break;
    }

    if (mChoiceMode == Choice_MultiChoice) {
        QString correctAnswer
                = mHideTranslation ? mCards.at(mCurrCard)->translation()
                                   : mCards.at(mCurrCard)->word();
        dynamic_cast<MultiAnswerWidget *>(wgtAnswer)->
                setAnswers(getAnswersMultiChoice(correctAnswer,
                                                 mHideTranslation));
    }
}

void QuizDialog::nextCheckWord()
{
    if (mThatsAll) {
        close();
        return;
    }

    if (mAnswered) {
        mAnswered = false;
        setCurrentWord(mCurrCard + 1);
    } else {
        if (!mHideTranslation) {
            cardText->showWord(false, true);
        } else {
            cardText->showOtherSide();
        }

        QString correctAnswer
                = mHideTranslation ? mCards.at(mCurrCard)->translation()
                                   : mCards.at(mCurrCard)->word();

        bool correct = ((correctAnswer.indexOf(wgtAnswer->getAnswer(), 0,
                                Qt::CaseInsensitive)) != -1 )
                                && (!(wgtAnswer->getAnswer().isEmpty()));

        wgtAnswer->setCorrect(correct);

        if (correct) {
            mCards.at(mCurrCard)->incCorrectAnswers();
            mModified = true;
        }

        mAnswered = true;
        switchButtons();
    }
}

void QuizDialog::dontKnow()
{
    mAnswered = true;
    cardText->showOtherSide();
    switchButtons();
}

QStringList QuizDialog::getAnswersMultiChoice(QString correct, bool translation)
{
    WordsPtrSet wrds = shuffleContainer(mAllCards, mAllCards.size());

    QStringList tmp;
    QString choice;
    for (int i = 0, n = 0; (n < (wrds.size())) && (i < numChoices - 1); i++, n++) {
        choice = (translation ? wrds.at(n)->translation() : wrds.at(n)->word());
        if (choice == correct) {
            i--;
            continue;
        }
        tmp << choice;
    }

    tmp << correct;
    tmp = shuffleContainer(tmp, tmp.size());

    return tmp;
}
