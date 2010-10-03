#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include <QWidget>

class AnswerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnswerWidget(QWidget *parent = 0);
    virtual QString getAnswer() = 0;
    virtual void setCorrect(bool correct) = 0;
    virtual void clear() = 0;
};

#endif // ANSWERWIDGET_H
