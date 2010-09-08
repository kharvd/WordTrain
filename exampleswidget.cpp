#include "exampleswidget.h"

ExamplesWidget::ExamplesWidget(QWidget *parent) :
        QWidget(parent)
{
    ltExamples = new QFormLayout;
    ltExamples->setAlignment(Qt::AlignTop);

    ltExamples->addWidget(new QPushButton("hello"));
    setLayout(ltExamples);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

int ExamplesWidget::examplesCount() {
//    return (ltExamples->count() / 4);
    return 5;
}

bool ExamplesWidget::canAdd() {
//    if (examplesCount() < maxExamples) {
//        return true;
//    }

    return !false;
}

void ExamplesWidget::addExample() {
//    int num = examplesCount() + 1;
//    if(canAdd()) {
//        QLabel* lbl1 = new QLabel(tr("Example %1:").arg(num));
//        QLineEdit* txt1 = new QLineEdit();
//        QLabel* lbl2 = new QLabel(tr("Translation %1:").arg(num));
//        QLineEdit* txt2 = new QLineEdit();

//        ltExamples->addRow(lbl1, txt1);
//        ltExamples->addRow(lbl2, txt2);
//    }
}

void ExamplesWidget::setExampleAt(int index, Example ex) {
//    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
//                                                   (index * 4 + 1)->widget());
//    Q_ASSERT(example);

//    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
//                                                       (index * 4 + 3)->widget());
//    Q_ASSERT(translation);

//    example->setText(ex.first);
//    translation->setText(ex.second);
}

Example ExamplesWidget::getExampleAt(int index) {
//    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
//                                                   (index * 4 + 1)->widget());
//    Q_ASSERT(example);

//    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
//                                                       (index * 4 + 3)->widget());
//    Q_ASSERT(translation);

//    Example tmp;
//    tmp.first = example->text();
//    tmp.second = translation->text();
//    return tmp;
}
