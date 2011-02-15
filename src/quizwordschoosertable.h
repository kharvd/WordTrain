#ifndef QUIZWORDSCHOOSERTABLE_H
#define QUIZWORDSCHOOSERTABLE_H

#include <QTableWidget>
#include "wordscard.h"

class QuizWordsChooserTable : public QTableWidget
{
    Q_OBJECT

public:
    explicit QuizWordsChooserTable(const WordsPtrSet &cards, QWidget *parent = 0);

    WordsPtrSet selectedCards();
    void setCheckedAt(int index, bool checked);

private:
    void fillTable();

    WordsPtrSet m_Cards;
};

#endif // QUIZWORDSCHOOSERTABLE_H
