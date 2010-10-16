#include "lineanswerwidget.h"

LineAnswerWidget::LineAnswerWidget(QWidget *parent) :
    AnswerWidget(parent)
{
    txtAnswer = new QLineEdit();
    imgCorrect = new QLabel();

    QHBoxLayout *lt = new QHBoxLayout();
    lt->addWidget(txtAnswer);
    lt->addWidget(imgCorrect);

    lt->setMargin(0);

    setLayout(lt);
}

QString LineAnswerWidget::getAnswer()
{
    return txtAnswer->text();
}

void LineAnswerWidget::clear()
{
    txtAnswer->clear();
    imgCorrect->clear();
}
