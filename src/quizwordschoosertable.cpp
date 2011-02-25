#include "quizwordschoosertable.h"
#include <QCheckBox>
#include <QHeaderView>

QuizWordsChooserTable::QuizWordsChooserTable(const WordsPtrSet &words, QWidget *parent) :
    QTableWidget(parent), m_Cards(words)
{
    setColumnCount(2);
    setRowCount(0);

    setHorizontalHeaderLabels(QStringList() << "" << tr("Word"));
    // Select whole row without multi-selection.
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);

    // No edit triggers
    setEditTriggers(QAbstractItemView::NoEditTriggers);

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

    setColumnWidth(0, cellWidget(0, 0)->width() + 10);
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
