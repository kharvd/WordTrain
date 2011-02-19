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

#include "gettingstartedwidget.h"
#include <QLayout>
#include <QPushButton>
#include <QFrame>
#include <QLabel>

GettingStartedWidget::GettingStartedWidget(QWidget *parent) :
    QWidget(parent)
{
    const int iconSize = 32;
    QLabel *headerLbl = new QLabel(tr("Getting started"));

    QFont labelFont;
    labelFont.setPointSize(20);
    headerLbl->setMargin(10);
    headerLbl->setFont(labelFont);
    headerLbl->setAlignment(Qt::AlignLeft);

    QFont buttonFont;
    buttonFont.setPointSize(11);

    QPushButton *newBtn = new QPushButton(tr("Create new set"));
    newBtn->setFlat(true);
    newBtn->setIcon(QIcon(":/icons/new-48.png"));
    newBtn->setIconSize(QSize(iconSize, iconSize));
    newBtn->setFont(buttonFont);
    connect(newBtn, SIGNAL(clicked()), SIGNAL(newSet()));

    QPushButton *openBtn = new QPushButton(tr("Open set..."));
    openBtn->setFlat(true);
    openBtn->setIcon(QIcon(":/icons/open-48.png"));
    openBtn->setIconSize(QSize(iconSize, iconSize));
    openBtn->setFont(buttonFont);
    connect(openBtn, SIGNAL(clicked()), SIGNAL(openSet()));

    QPushButton *helpBtn = new QPushButton(tr("Help"));
    helpBtn->setFlat(true);
    helpBtn->setIcon(QIcon(":/icons/help-48.png"));
    helpBtn->setIconSize(QSize(iconSize, iconSize));
    helpBtn->setFont(buttonFont);
    connect(helpBtn, SIGNAL(clicked()), SIGNAL(help()));

    QPushButton *exitBtn = new QPushButton(tr("Quit"));
    exitBtn->setFlat(true);
    exitBtn->setIcon(QIcon(":/icons/exit-48.png"));
    exitBtn->setIconSize(QSize(iconSize, iconSize));
    exitBtn->setFont(buttonFont);
    connect(exitBtn, SIGNAL(clicked()), SIGNAL(quit()));

    QVBoxLayout *lt = new QVBoxLayout();
    lt->addWidget(headerLbl);
    lt->addWidget(newBtn);
    lt->addWidget(openBtn);
    lt->addWidget(helpBtn);
    lt->addWidget(exitBtn);

    QFrame *grp = new QFrame(this);
    grp->setFrameStyle(QFrame::Box);
    grp->setLayout(lt);

    QGridLayout *lt1 = new QGridLayout();
    lt1->addWidget(grp, 0, 0, Qt::AlignCenter);
    setLayout(lt1);
}
