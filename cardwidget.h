#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QtGui>
#include "wordscard.h"

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = 0);
    void showWord(const WordCard& card, bool faceSide = true);

public slots:
    void showOtherSide();

private:
    QString getCSS();
    void showFace(const WordCard& card);
    void showBack(const WordCard& card);

    bool mFace;
    WordCard currCard;
    QString htmlCard;
    QTextEdit *txtCard;
};

#endif // CARDWIDGET_H
