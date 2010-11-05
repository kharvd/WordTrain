/******************************************************************************
** WordTrain 0.8.5 -- Foreign words trainer
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

#include "viewcarddialog.h"

#include "cardwidget.h"
#include "neweditcarddialog.h"
#include <QPushButton>
#include <QLayout>

ViewCardDialog::ViewCardDialog(const WordsPtrSet & cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards.size());
    mModified = false;
    mCards = cards;
    createInterface();
    setCurrentCard(0);
}

ViewCardDialog::ViewCardDialog(WordsSet *cards, QWidget *parent) :
    QDialog(parent)
{
    Q_ASSERT(cards->size());
    for (int i = 0; i < cards->size(); i++)
        mCards.push_back(&cards[0][i]);

    mModified = false;
    createInterface();
    setCurrentCard(0);
}

void ViewCardDialog::createInterface()
{
    setWindowTitle(tr("View card"));
    resize(defaultWidth, defaultHeight);

    cardText = new CardWidget();

    btnPrevious = new QPushButton(tr("Previous"));
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(prevCard()));

    btnNext = new QPushButton(tr("Next"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextCard()));
    btnNext->setDefault(true);

    btnEdit = new QPushButton(tr("Edit card"));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(editCard()));

    btnTurn = new QPushButton(tr("Show other side"));
    connect(btnTurn, SIGNAL(clicked()), cardText, SLOT(showOtherSide()));

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(btnEdit);
    hLayout1->addStretch(1);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(btnPrevious);
    hLayout2->addWidget(btnTurn);
    hLayout2->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout1);
    mainLayout->addWidget(cardText);
    mainLayout->addLayout(hLayout2);

    setLayout(mainLayout);
}

void ViewCardDialog::switchButtons()
{
    btnPrevious->setEnabled(mCurrentCard != 0);
    btnNext->setEnabled(mCurrentCard != (mCards.size() - 1));
}

void ViewCardDialog::setCurrentCard(int index)
{
    mCurrentCard = index;
    switchButtons();
    cardText->showCard(*(mCards[index]));
}

bool ViewCardDialog::isModified()
{
    return mModified;
}

void ViewCardDialog::prevCard()
{
    setCurrentCard(mCurrentCard - 1);
}

void ViewCardDialog::nextCard()
{
    setCurrentCard(mCurrentCard + 1);
}

void ViewCardDialog::editCard()
{
    NewEditCardDialog* dlg = new NewEditCardDialog(*(mCards[mCurrentCard]));
    if (dlg->exec()) {
        *mCards[mCurrentCard] = dlg->getNewCard();
        mModified = true;
        setCurrentCard(mCurrentCard); // Update
    }
}

