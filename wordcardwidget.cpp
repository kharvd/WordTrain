#include "wordcardwidget.h"

WordCardWidget::WordCardWidget(const WordsPtrSet & cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards.size());
    mModified = false;
    isFace = true;
    mCards = cards;
    createInterface();
    setCurrentWord(0);
}

WordCardWidget::WordCardWidget(WordsSet *cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++) {
        mCards.push_back(&cards[0][i]);
    }

    mModified = false;
    isFace = true;
    createInterface();
    setCurrentWord(0);
}

void WordCardWidget::createInterface()
{
    setWindowTitle(tr("View card"));
    setWindowFlags(Qt::Window);
    setFixedSize(400, 360);

    btnPrevious = new QPushButton(tr("Previous"));
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(prevWord()));

    btnTurn = new QPushButton(tr("Show other side"));
    connect(btnTurn, SIGNAL(clicked()), this, SLOT(showOtherSide()));

    btnNext = new QPushButton(tr("Next"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextWord()));
    btnNext->setDefault(true);

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
    hLayout2->addWidget(btnTurn);
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
    btnTurn->setEnabled(true);
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

QString WordCardWidget::getCSS() {
    QFile file(":/style.css");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream stream(&file);
    return stream.readAll();
}

void WordCardWidget::setCurrentWord(int index, bool faceSide)
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


    QString htmlCard;
    htmlCard.append("<head><style type=\"text/css\">");
    htmlCard.append(getCSS());
    htmlCard.append("</style></head><body style=\"font-size:9pt;\">");

    if (faceSide) {
        // SHIT: Trying to align this fucked up QTextEdit
        htmlCard.append("<br />");
        htmlCard.append("<br />");
        htmlCard.append("<br />");
        htmlCard.append("<br />");

        // Word and its gender
        htmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                                "<span style=\"font-size:12pt\"> %2</span></p>")
                .arg(mCards[index]->word(), mCards[index]->genderShortString()));


        // Category
        htmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                .arg(mCards[index]->lexCategoriesShortString()));

        // Transcription
        htmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                                "[%1]</p>")
                .arg(mCards[index]->transcription()));
    }
    else
    {
        htmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                                "<span style=\"font-size:12pt\"> %2</span></p>")
                .arg(mCards[index]->word(), mCards[index]->genderShortString()));

        // Plural
        htmlCard.append(QString("<p class=\"plural\" align=\"center\">%1</p>")
                .arg(mCards[index]->plural()));

        // Category
        htmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                .arg(mCards[index]->lexCategoriesShortString()));

        // Transcription
        htmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                                "[%1]</p>")
                .arg(mCards[index]->transcription()));

        // Translation
        htmlCard.append(QString("<p class=\"translation\" align=\"center\">"
                                "<b>%1</b></p>")
                .arg(mCards[index]->translation()));

        // Examples
        htmlCard.append("<p class=\"examples\">Examples:</p>");

        for (int i = 0; i < mCards[index]->examplesSize(); i++) {
            htmlCard.append(QString("<p class=\"example\">%1. %2<br />— %3</p>")
                            .arg(i + 1).arg(mCards[index]->exampleAt(i).first,
                                        mCards[index]->exampleAt(i).second));
        }
    }

    htmlCard.append("</body>");

    txtCardText->setHtml(htmlCard);
}

bool WordCardWidget::isModified()
{
    return mModified;
}

void WordCardWidget::prevWord()
{
    isFace = true;
    setCurrentWord(mCurrCard - 1);
}

void WordCardWidget::nextWord()
{
    isFace = true;
    setCurrentWord(mCurrCard + 1);
}

void WordCardWidget::showOtherSide()
{
    isFace = !isFace;
    setCurrentWord(mCurrCard, isFace);
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
