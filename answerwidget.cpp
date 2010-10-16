#include "answerwidget.h"

AnswerWidget::AnswerWidget(QWidget *parent) :
    QWidget(parent)
{
    imgCorrect = new QLabel();
}

void AnswerWidget::setCorrect(bool correct)
{
    if (correct) {
        imgCorrect->setPixmap(QPixmap(":/icons/correct.png"));
    } else {
        imgCorrect->setPixmap(QPixmap(":/icons/incorrect.png"));
    }
}
