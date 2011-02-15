#include "quizwordschoosertable.h"
#include <QCheckBox>

QuizWordsChooserTable::QuizWordsChooserTable(const WordsPtrSet &words, QWidget *parent) :
    QTableWidget(parent), m_Cards(words)
{
    setColumnCount(2);
    setRowCount(0);
    fillTable();
}

void QuizWordsChooserTable::fillTable()
{
    for (int i = 0, size = m_Cards.size(); i < size; i++) {
        insertRow(i);

        QCheckBox *chck = new QCheckBox();
        setCellWidget(i, 0, chck);

        QTableWidgetItem *item = new QTableWidgetItem(m_Cards[i]->word());
        setItem(i, 1, item);
    }
}

WordsPtrSet QuizWordsChooserTable::selectedCards()
{
    WordsPtrSet tmp;

    for (int i = 0, size = rowCount(); i < size; i++) {
        QCheckBox *chck = qobject_cast<QCheckBox*>(cellWidget(i, 0));
        if (chck->isChecked()) {
            tmp << m_Cards[i];
        }
    }

    return tmp;
}

void QuizWordsChooserTable::setCheckedAt(int index, bool checked)
{
    QCheckBox *chck = qobject_cast<QCheckBox*>(cellWidget(index, 0));
    chck->setChecked(checked);
}
