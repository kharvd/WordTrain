/******************************************************************************
** WordTrain 0.9.2 -- Foreign words trainer
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
    m_Modified = false;
    m_Cards = cards;
    createInterface();
    setCurrentCard(0);
}

void ViewCardDialog::createInterface()
{
    setWindowTitle(tr("View card"));
    resize(kDefaultWidth, kDefaultHeight);

    wgtCard = new CardWidget();

    btnPrevious = new QPushButton(tr("Previous"));
    connect(btnPrevious, SIGNAL(clicked()), this, SLOT(prevCard()));

    btnNext = new QPushButton(tr("Next"));
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextCard()));
    btnNext->setDefault(true);

    btnEdit = new QPushButton(tr("Edit card"));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(editCard()));

    btnTurn = new QPushButton(tr("Show other side"));
    connect(btnTurn, SIGNAL(clicked()), wgtCard, SLOT(showOtherSide()));

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    hLayout1->addWidget(btnEdit);
    hLayout1->addStretch(1);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(btnPrevious);
    hLayout2->addWidget(btnTurn);
    hLayout2->addWidget(btnNext);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(hLayout1);
    mainLayout->addWidget(wgtCard);
    mainLayout->addLayout(hLayout2);

    setLayout(mainLayout);
}

void ViewCardDialog::switchButtons()
{
    btnPrevious->setEnabled(m_CurrentCard != 0);
    btnNext->setEnabled(m_CurrentCard != (m_Cards.size() - 1));
}

void ViewCardDialog::setCurrentCard(int index)
{
    m_CurrentCard = index;
    switchButtons();
    wgtCard->showCard(*(m_Cards[index]));
}

bool ViewCardDialog::isModified()
{
    return m_Modified;
}

void ViewCardDialog::prevCard()
{
    setCurrentCard(m_CurrentCard - 1);
}

void ViewCardDialog::nextCard()
{
    setCurrentCard(m_CurrentCard + 1);
}

void ViewCardDialog::editCard()
{
    NewEditCardDialog* dlg = new NewEditCardDialog(*(m_Cards[m_CurrentCard]));
    if (dlg->exec()) {
        *m_Cards[m_CurrentCard] = dlg->newCard();
        m_Modified = true;
        setCurrentCard(m_CurrentCard); // Update
    }
}

