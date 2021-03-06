/******************************************************************************
** WordTrain 0.9.3 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#include "quizwordschooserdialog.h"

#include <QPushButton>
#include <QLayout>
#include <QLabel>

#include "quizwordschoosertable.h"
#include "tagsscrollarea.h"

QuizWordsChooserDialog::QuizWordsChooserDialog(const WordsPtrSet &words,
                                               QWidget *parent) :
    QDialog(parent), m_Cards(words)
{
    createInterface();
    setWindowTitle(tr("Choose words for the quiz"));
    resize(kDefaultWidth, kDefaultHeight);
}

void QuizWordsChooserDialog::createInterface()
{
    tblWords = new QuizWordsChooserTable(m_Cards);

    Tags tmp;

    foreach (WordCard *card, m_Cards) {
        tmp += card->tags();
    }

    tagsArea = new TagsScrollArea(tmp);
    connect(tagsArea, SIGNAL(stateChanged(const QString&, bool)),
            SLOT(chooseWordsByTag(const QString&, bool)));

    QLabel *lblTags = new QLabel(tr("Choose words by tag:"));
    lblTags->setWordWrap(true);

    QPushButton *ok = new QPushButton(tr("OK"));
    connect(ok, SIGNAL(clicked()), SLOT(accept()));

    QVBoxLayout *ltLeft = new QVBoxLayout;
    ltLeft->addWidget(new QLabel(tr("Available words:")));
    ltLeft->addWidget(tblWords);

    QVBoxLayout *ltRight = new QVBoxLayout;
    ltRight->addWidget(ok);
    ltRight->addWidget(lblTags);
    ltRight->addWidget(tagsArea);

    QHBoxLayout *lt = new QHBoxLayout;
    lt->addLayout(ltLeft);
    lt->addLayout(ltRight);

    setLayout(lt);
}

void QuizWordsChooserDialog::chooseWordsByTag(const QString& tag, bool checked)
{
    for (int i = 0, size = m_Cards.size(); i < size; i++) {
        if (m_Cards.at(i)->tags().contains(tag))
            tblWords->setCheckedAt(i, checked);
    }
}

WordsPtrSet QuizWordsChooserDialog::cards()
{
    return tblWords->selectedCards();
}
