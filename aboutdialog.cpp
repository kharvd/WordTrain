/******************************************************************************
** WordTrain 0.8.4 -- Foreign words trainer
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

#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    resize(500, 400);
    setWindowTitle(tr("About WordTrain"));

    QIcon icon = windowIcon();
    QSize size = icon.actualSize(QSize(64, 64));
    QLabel *lblIcon = new QLabel();
    lblIcon->setPixmap(icon.pixmap(size));

    QLabel *lblName = new QLabel();
    lblName->setText(QString("WordTrain v%1").arg(qApp->applicationVersion()));

    QFont font;
    font.setPointSize(18);
    lblName->setMargin(10);
    lblName->setFont(font);

    QLabel *lblDescr = new QLabel();
    lblDescr->setOpenExternalLinks(true);
    lblDescr->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    QString descr
            = tr("<b>WordTrain</b> - a simple program for learning foreign"
                 " words. You can test yourself with a quiz mode or just"
                 " store word cards. If you have any questions or wishes,"
                 " feel free to contact me at "
                 "<a href=\"mailto:kharvd@gmail.com\">kharvd@gmail.com</a>"
                 "<br/><br/><a href=\"http://sourceforge.net/projects/wordtrain\">"
                 "Web site</a><br/><br/>"
                 "Copyright © 2010 Valery Kharitonov <br/><br/>"
                 "This program comes with ABSOLUTELY NO WARRANTY. "
                 "This is free software, and you are welcome to redistribute it"
                 " under certain conditions. See 'License' tab or "
                 "<a href=\"http://www.gnu.org/licenses/gpl.html\">"
                 "http://www.gnu.org/licenses/gpl.html</a> for details");

    lblDescr->setText(descr);
    lblDescr->setWordWrap(true);
    lblDescr->setMargin(10);

    QTextEdit *license = new QTextEdit();

    QFile file(":/COPYING");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        license->setText(in.readAll());
    }

    QPushButton *btnClose = new QPushButton(tr("Close"));
    connect(btnClose, SIGNAL(clicked()), SLOT(close()));

    QTabWidget *tabs = new QTabWidget();
    tabs->addTab(lblDescr, QIcon(), tr("About"));
    tabs->addTab(license, QIcon(), tr("License"));

    QHBoxLayout *ltHeader = new QHBoxLayout();
    ltHeader->addWidget(lblIcon);
    ltHeader->addWidget(lblName);
    ltHeader->addStretch(1);

    QHBoxLayout *ltButton = new QHBoxLayout();
    ltButton->addStretch(1);
    ltButton->addWidget(btnClose);

    QVBoxLayout *ltMain = new QVBoxLayout();
    ltMain->addLayout(ltHeader);
    ltMain->addWidget(tabs);
    ltMain->addLayout(ltButton);

    setLayout(ltMain);
}
