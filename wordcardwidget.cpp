#include "wordcardwidget.h"

WordCardWidget::WordCardWidget(const WordsPtrSet & cards, WordCardShowMode mode,
               QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards.size());
    mModified = false;
    mCards = cards;
    mShowMode = mode;
    createInterface();
    setCurrentWord(0);
}

WordCardWidget::WordCardWidget(WordsSet *cards, WordCardShowMode mode,
               QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mModified = false;
    mShowMode = mode;
    createInterface();
    setCurrentWord(0);
}

void WordCardWidget::createInterface()
{
    setWindowTitle(tr("View card"));
    resize(400, 360);

    btnPrevious = new QPushButton(tr("Previous"));
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(prevWord()));

    btnAnswer = new QPushButton(tr("Show Answer"));
    connect(btnAnswer, SIGNAL(clicked()), this, SLOT(showAnswer()));

    btnNext = new QPushButton(tr("Next"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextWord()));

    btnEdit = new QPushButton(tr("Edit card"));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(editCard()));

    btnLearned = new QPushButton();
    connect(btnLearned, SIGNAL(clicked()), this, SLOT(setCardLearned()));

    txtCardText = new QTextEdit();

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(btnEdit);
    hLayout1->addStretch(1);
    hLayout1->addWidget(btnLearned);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(btnPrevious);
    hLayout2->addWidget(btnAnswer);
    hLayout2->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout1);
    mainLayout->addWidget(txtCardText);
    mainLayout->addLayout(hLayout2);

    setLayout(mainLayout);
}

void WordCardWidget::enableButtons()
{
    btnPrevious->setEnabled(true);
    btnAnswer->setEnabled(true);
    btnNext->setEnabled(true);
    btnEdit->setEnabled(true);
    btnLearned->setEnabled(true);
}

void WordCardWidget::setLearnedButtonText()
{
    if (mCards[mCurrCard]->isLearned()) {
        btnLearned->setText(tr("Set unlearned"));
    } else {
        btnLearned->setText(tr("Set learned"));
    }
}

void WordCardWidget::setCurrentWord(int index)
{
    mCurrCard = index;

    enableButtons();

    if (mCurrCard == 0) {
        btnPrevious->setEnabled(false);
    }

    if (mCurrCard == mCards.size() - 1) {
        btnNext->setEnabled(false);
    }

    if (mShowMode == SM_HideNone) {
        btnAnswer->setEnabled(false);
    }

    setLearnedButtonText();


    QString htmlCard;
    htmlCard.append("<body style=\"font-size:9pt;\">");

    // Word and its gender
    htmlCard.append(QString("<p align=\"center\" style=\"margin-top:12px;"
                    "margin-bottom:0px;font-size:15pt;\"><b>%1</b>, %2</p>")
            .arg(mCards[index]->word(), mCards[index]->genderShortString()));

    // Plural
    htmlCard.append(QString("<p align=\"center\" style=\"margin-top:0px;"
                            "margin-bottom:0px;\">%1</p>")
            .arg(mCards[index]->plural()));

    // Category
    htmlCard.append(QString("<p align=\"center\" style=\"margin-top:0px;"
                            "margin-bottom:12px;\">%1</p>")
            .arg(mCards[index]->lexCategoriesShortString()));

    // Transcription
    htmlCard.append(QString("<p align=\"center\" style=\"margin-top:0px;"
                            "margin-bottom:12px; font-style:italic;"
                            "font-size:12pt;\">[%1]</p>")
            .arg(mCards[index]->transcription()));

    // Translation
    htmlCard.append(QString("<p align=\"center\" style=\"margin-top:12px;"
                           "margin-bottom:12px;font-size:12pt;\"><b>%1</b></p>")
            .arg(mCards[index]->translation()));

    // Examples
    htmlCard.append("<p style=\"margin-top:12px;margin-bottom:12px;"
                    "font-size:8pt;font-style:italic;\">Examples:</p>");

    for (int i = 0; i < mCards[index]->examplesSize(); i++) {
        htmlCard.append(QString("<p style=\"margin-top:12px;margin-bottom:12px;"
                                "font-style:italic;\">%1. %2<br />— %3</p>")
                        .arg(i + 1).arg(mCards[index]->exampleAt(i).first,
                                    mCards[index]->exampleAt(i).second));
    }

    htmlCard.append("</body>");

    txtCardText->setText(htmlCard);
}

bool WordCardWidget::isModified()
{
    return mModified;
}

void WordCardWidget::nextWord()
{
    setCurrentWord(mCurrCard + 1);
}

void WordCardWidget::showAnswer()
{

}

void WordCardWidget::prevWord()
{
    setCurrentWord(mCurrCard - 1);
}

void WordCardWidget::editCard()
{
    NewEditCardDialog* dlg = new NewEditCardDialog(*(mCards[mCurrCard]));
    if (dlg->exec()) {
        *mCards[mCurrCard] = dlg->getNewCard();
        mModified = true;
        setCurrentWord(mCurrCard); // Update
    }
}

void WordCardWidget::setCardLearned()
{
    mCards[mCurrCard]->setLearned(!mCards[mCurrCard]->isLearned());
    setLearnedButtonText();
    mModified = true;
}
