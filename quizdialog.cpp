#include "quizdialog.h"

QuizDialog::QuizDialog(const WordsPtrSet & cards, QuizMode mode,
                       QWidget *parent)
                           : QDialog(parent)
{
    Q_ASSERT(cards.size());
    mCards = cards;
    mAnswered = false;
    mThatsAll = false;
    mMode = mode;
    createInterface();
    setCurrentWord(0);
}

QuizDialog::QuizDialog(WordsSet *cards, QuizMode mode, QWidget *parent)
    : QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mAnswered = false;
    mThatsAll = false;
    mMode = mode;
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

    switch (mMode) {
    case Mode_NoChoice:
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
        btnCheckNext->setText(tr("Next"));
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
    cardText->showWord(*(mCards[index]));
}

void QuizDialog::nextCheckWord()
{
    if (mThatsAll) {
        close();
        return;
    }

    if (mAnswered) {
        mAnswered = false;
        if (mCurrCard == mCards.size() - 1) {
            btnCheckNext->setText(tr("Close"));
            mThatsAll = true;
        } else {
            setCurrentWord(mCurrCard + 1);
        }
    } else {
        cardText->showOtherSide();
        QString correctAnswer = mCards.at(mCurrCard)->translation();
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
