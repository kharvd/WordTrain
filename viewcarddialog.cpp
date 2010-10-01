#include "viewcarddialog.h"

ViewCardDialog::ViewCardDialog(const WordsPtrSet & cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards.size());
    mModified = false;
    mCards = cards;
    createInterface();
    setCurrentWord(0);
}

ViewCardDialog::ViewCardDialog(WordsSet *cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mModified = false;
    createInterface();
    setCurrentWord(0);
}

void ViewCardDialog::createInterface()
{
    setWindowTitle(tr("View card"));
    setWindowFlags(Qt::Window);
    setFixedSize(400, 360);

    cardText = new CardWidget();

    btnPrevious = new QPushButton(tr("Previous"));
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(prevWord()));

    btnNext = new QPushButton(tr("Next"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextWord()));
    btnNext->setDefault(true);

    btnEdit = new QPushButton(tr("Edit card"));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(editCard()));

    btnLearned = new QPushButton();
    connect(btnLearned, SIGNAL(clicked()), this, SLOT(setCardLearned()));

    btnTurn = new QPushButton(tr("Show other side"));
    connect(btnTurn, SIGNAL(clicked()), cardText, SLOT(showOtherSide()));

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(btnEdit);
    hLayout1->addStretch(1);
    hLayout1->addWidget(btnLearned);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(btnPrevious);
    hLayout2->addWidget(btnTurn);
    hLayout2->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout1);
    mainLayout->addWidget(cardText);
    mainLayout->addLayout(hLayout2);

    setLayout(mainLayout);
}

void ViewCardDialog::enableButtons()
{
    btnPrevious->setEnabled(true);
    btnTurn->setEnabled(true);
    btnNext->setEnabled(true);
    btnEdit->setEnabled(true);
    btnLearned->setEnabled(true);
}

void ViewCardDialog::setLearnedButtonText()
{
    if (mCards[mCurrCard]->isLearned()) {
        btnLearned->setText(tr("Set unlearned"));
    } else {
        btnLearned->setText(tr("Set learned"));
    }
}

void ViewCardDialog::setCurrentWord(int index)
{
    mCurrCard = index;

    enableButtons();

    if (mCurrCard == 0) {
        btnPrevious->setEnabled(false);
    }

    if (mCurrCard == mCards.size() - 1) {
        btnNext->setEnabled(false);
    }

    setLearnedButtonText();

    cardText->showWord(*(mCards[index]));
}

bool ViewCardDialog::isModified()
{
    return mModified;
}

void ViewCardDialog::prevWord()
{
    setCurrentWord(mCurrCard - 1);
}

void ViewCardDialog::nextWord()
{
    setCurrentWord(mCurrCard + 1);
}

void ViewCardDialog::editCard()
{
    NewEditCardDialog* dlg = new NewEditCardDialog(*(mCards[mCurrCard]));
    if (dlg->exec()) {
        *mCards[mCurrCard] = dlg->getNewCard();
        mModified = true;
        setCurrentWord(mCurrCard); // Update
    }
}

void ViewCardDialog::setCardLearned()
{
    mCards[mCurrCard]->setLearned(!mCards[mCurrCard]->isLearned());
    setLearnedButtonText();
    mModified = true;
}

