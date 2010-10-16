#ifndef MULTIANSWERWIDGET_H
#define MULTIANSWERWIDGET_H

#include "answerwidget.h"
#include <QtGui>

class MultiAnswerWidget : public AnswerWidget
{
    Q_OBJECT
public:
    explicit MultiAnswerWidget(QWidget *parent = 0);
    virtual QString getAnswer();
    virtual void setAnswers(const QStringList & answers);
    virtual void clear();

private:
    void createRadios();

protected:
    QButtonGroup *grpAnswers;
    QVBoxLayout *ltRadios;
    QStringList mAnswers;
};

#endif // MULTIANSWERWIDGET_H
