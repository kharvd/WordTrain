#ifndef CARDWIDGET_H
#define CARDWIDGET_H

#include <QtGui>
#include "wordscard.h"

class CardWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CardWidget(QWidget *parent = 0);

    // showForeign - if true, shows 'word', 'plural', 'transcription' etc on
    // the back side
    void showWord(const WordCard& card, bool faceSide = true,
                  bool showForeign = true);
    void showWord(bool faceSide = true, bool showForeign = true);

public slots:
    void showOtherSide();

private:
    QString getCSS();
    void showFace(const WordCard& card);

    // showForeign - if true, shows 'word', 'plural', 'transcription' etc
    void showBack(const WordCard& card, bool showForeign);

    bool mFace;
    WordCard mCurrWord;
    QString htmlCard;
    QTextEdit *txtCard;
};

#endif // CARDWIDGET_H
