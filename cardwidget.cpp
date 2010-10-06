#include "cardwidget.h"

CardWidget::CardWidget(QWidget *parent) :
    QWidget(parent)
{
    txtCard = new QTextEdit();
    txtCard->setReadOnly(true);
    QHBoxLayout* lt = new QHBoxLayout;
    lt->addWidget(txtCard);
    lt->setMargin(0);
    setLayout(lt);

    mFace = true;
}

void CardWidget::showWord(const WordCard& card, bool faceSide, bool showForeign)
{
    mFace = faceSide;
    mCurrWord = card;

    htmlCard.clear();

    htmlCard.append("<head><style type=\"text/css\">");
    htmlCard.append(getCSS());
    htmlCard.append("</style></head><body style=\"font-size:9pt;\">");

    if (faceSide) {
        showFace(card);
    } else {
        showBack(card, showForeign);
    }

    htmlCard.append("</body>");

    txtCard->setHtml(htmlCard);
}

void CardWidget::showWord(bool faceSide, bool showForeign)
{
    showWord(mCurrWord, faceSide, showForeign);
}

void CardWidget::showOtherSide()
{
    mFace = !mFace;
    showWord(mCurrWord, mFace);
}

QString CardWidget::getCSS() {
    QFile file(":/style.css");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();

    QTextStream stream(&file);
    return stream.readAll();
}

void CardWidget::showFace(const WordCard& card)
{
    // SHIT: Trying to align this fucked up QTextEdit
    htmlCard.append("<br />");
    htmlCard.append("<br />");
    htmlCard.append("<br />");
    htmlCard.append("<br />");

    // Word and its gender
    htmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                            "<span style=\"font-size:12pt\"> %2</span></p>")
                    .arg(card.word(), card.genderShortString()));


    // Category
    htmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                    .arg(card.lexCategoriesShortString()));

    // Transcription
    htmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                            "[%1]</p>")
                    .arg(card.transcription()));
}

void CardWidget::showBack(const WordCard& card, bool showForeign)
{
    if (showForeign) {
        // Word and its gender
        htmlCard.append(QString("<p class=\"word\" align=\"center\"><b>%1</b>"
                                "<span style=\"font-size:12pt\"> %2</span></p>")
                .arg(card.word(), card.genderShortString()));

        // Plural
        htmlCard.append(QString("<p class=\"plural\" align=\"center\">%1</p>")
                .arg(card.plural()));

        // Category
        htmlCard.append(QString("<p class=\"category\" align=\"center\">%1</p>")
                .arg(card.lexCategoriesShortString()));

        // Transcription
        htmlCard.append(QString("<p class=\"transcription\" align=\"center\">"
                                "[%1]</p>")
                .arg(card.transcription()));
    } else {
        // SHIT: Trying to align this fucked up QTextEdit
        htmlCard.append("<br />");
        htmlCard.append("<br />");
        htmlCard.append("<br />");
        htmlCard.append("<br />");
    }


    // Translation
    htmlCard.append(QString("<p class=\"translation\" align=\"center\">"
                            "<b>%1</b></p>")
            .arg(card.translation()));

    // Examples
    htmlCard.append("<p class=\"examples\">Examples:</p>");

    for (int i = 0; i < card.examplesSize(); i++) {
        if (showForeign) {
            htmlCard.append(QString("<p class=\"example\">%1. %2<br />— %3</p>")
                            .arg(i + 1).arg(card.exampleAt(i).first,
                                        card.exampleAt(i).second));
        } else {
            htmlCard.append(QString("<p class=\"example\">%1. %2</p>")
                            .arg(i + 1).arg(card.exampleAt(i).second));
        }

    }
}
