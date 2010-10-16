#ifndef LINEANSWERWIDGET_H
#define LINEANSWERWIDGET_H

#include <QtGui>
#include "answerwidget.h"

class LineAnswerWidget : public AnswerWidget
{
    Q_OBJECT
public:
    explicit LineAnswerWidget(QWidget *parent = 0);
    virtual QString getAnswer();
    virtual void clear();

protected:
    QLineEdit *txtAnswer;
};

#endif // LINEANSWERWIDGET_H
