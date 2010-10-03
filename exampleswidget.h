#ifndef EXAMPLESWIDGET_H
#define EXAMPLESWIDGET_H

#include <QtGui>
#include "wordscard.h"

class ExamplesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExamplesWidget(QWidget *parent = 0);

    void setExampleAt(int index, Example ex);
    Example getExampleAt(int index);
    int examplesCount();
    bool canAdd();

    const static int maxExamples = 50;

private:
    QFormLayout *ltExamples;

public slots:
    void addExample();
};

#endif // EXAMPLESWIDGET_H
