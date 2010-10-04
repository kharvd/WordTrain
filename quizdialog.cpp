#include "quizdialog.h"

QuizDialog::QuizDialog(const WordsPtrSet & cards, ChoiceMode choice,
                       HideMode hide, QWidget *parent)
                           : QDialog(parent)
{
    qsrand(time(0));
    Q_ASSERT(cards.size());
    mCards = cards;
    mAnswered = false;
    mThatsAll = false;
    mChoiceMode = choice;
    mHideMode = hide;
    createInterface();
    setCurrentWord(0);
}

QuizDialog::QuizDialog(WordsSet *cards, ChoiceMode choice, HideMode hide,
                       QWidget *parent)
    : QDialog(parent)
{
    qsrand(time(0));
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mAnswered = false;
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
    setWindowFlags(Qt::Window);
    setFixedSize(400, 360);

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
        cardText->showWord(*(mCards[index]));
        break;
    case Hide_Word:
        cardText->showWord(*(mCards[index]), false);
        break;
    case Hide_Random:
        mHideTranslation = qrand() % 2;
        cardText->showWord(*(mCards[index]), mHideTranslation);
        break;
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
        cardText->showOtherSide();
        QString correctAnswer
                = mHideTranslation ? mCards.at(mCurrCard)->translation()
                                   : mCards.at(mCurrCard)->word();

        bool correct = ((correctAnswer.indexOf(wgtAnswer->getAnswer(), 0,
                                Qt::CaseInsensitive)) != -1 )
                                && (!(wgtAnswer->getAnswer().isEmpty()));
        wgtAnswer->setCorrect(correct);

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
