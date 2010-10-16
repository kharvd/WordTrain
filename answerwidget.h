#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include <QtGui>

class AnswerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnswerWidget(QWidget *parent = 0);
    virtual QString getAnswer() = 0;
    virtual void setCorrect(bool correct);
    virtual void clear() = 0;
protected:
    QLabel *imgCorrect;
};

#endif // ANSWERWIDGET_H
