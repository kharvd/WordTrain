#ifndef QUIZWORDSCHOOSERTABLE_H
#define QUIZWORDSCHOOSERTABLE_H

#include <QTableWidget>
#include "wordcard.h"

class QuizWordsChooserTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit QuizWordsChooserTable(const WordsPtrSet &cards, QWidget *parent = 0);
    virtual ~QuizWordsChooserTable() { }

    WordsPtrSet selectedCards();
    void setCheckedAt(int index, bool checked);

private:
    void fillTable();

    WordsPtrSet m_Cards;
};

#endif // QUIZWORDSCHOOSERTABLE_H
