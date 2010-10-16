#include "multianswerwidget.h"

MultiAnswerWidget::MultiAnswerWidget(QWidget *parent) :
    AnswerWidget(parent)
{
    ltRadios = new QVBoxLayout();
    grpAnswers = new QButtonGroup(this);

    QHBoxLayout *lt = new QHBoxLayout();
    lt->addLayout(ltRadios);
    lt->addWidget(imgCorrect);

    lt->setMargin(0);

    setLayout(lt);
}

void MultiAnswerWidget::setAnswers(const QStringList & answers)
{
    mAnswers = answers;
    createRadios();
}

void MultiAnswerWidget::createRadios()
{
    for (int i = 0; i < mAnswers.size(); i++) {
        QRadioButton *tmp = new QRadioButton(mAnswers[i]);
        grpAnswers->addButton(tmp, i);
        ltRadios->addWidget(tmp);
    }
}

QString MultiAnswerWidget::getAnswer()
{
    if (grpAnswers->checkedId() == -1) {
        return "";
    }
    return mAnswers.at(grpAnswers->checkedId());
}

void MultiAnswerWidget::clear()
{
    QLayoutItem *child;
    QRadioButton *btn;
    while ((child = ltRadios->takeAt(0)) != 0) {
        btn = qobject_cast<QRadioButton *>(child->widget());
        Q_ASSERT(btn);
        grpAnswers->removeButton(btn);

        delete btn;
    }

    mAnswers.clear();
    imgCorrect->clear();
}
