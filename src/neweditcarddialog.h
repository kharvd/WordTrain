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

#ifndef NEWCARDDIALOG_H
#define NEWCARDDIALOG_H

#include <QDialog>
#include "wordcard.h"

class QLineEdit;
class QComboBox;
class QScrollArea;
class QFormLayout;
class ExamplesWidget;

// Create new card or edit existing one
class NewEditCardDialog : public QDialog
{
    Q_OBJECT
public:
    //== Constructors =======================================

    /* If we want to add new card (you should use getNewCard() to get values) */
    NewEditCardDialog(QWidget *parent = 0);

    /* If we want to edit existing card (we don't change value of card directly,
       you should use getNewCard() to get new values) */
    NewEditCardDialog(const WordCard &card, QWidget *parent = 0);

    //=======================================================


    WordCard newCard();

public slots:
    void addExample();

    // Show plural and gender boxes only if the word is noun
    void switchPluralGender(int);

    // Set cards answers field to 0
    void resetProgress();

private:
    //== Static constant members ============================

    static const int kDefaultWidth = 610;
    static const int kDefaultHeight = 250;
    static const int kExampleHeight = 50;
    static const int kMaxAutoHeight = 420;

    //=======================================================


    //== Private member functions ===========================

    void createInterface();

    // Fills the form with current card's contents
    void fillForm();

    //=======================================================


    //== Private member variables ===========================

    bool m_isNew;

    // Temporary card for holding new values
    WordCard m_NewCard;

    //=======================================================


    //== Private widgets and other QObjects =================

    QFormLayout *ltFields;

    QLineEdit *txtWord;
    QLineEdit *txtTranscription;
    QLineEdit *txtTranslation;
    QLineEdit *txtPlural;
    QLineEdit *txtProgress;
    QLineEdit *txtTags;

    QComboBox *cmbCategory;
    QComboBox *cmbGender;

    QPushButton *btnAddExample;
    QPushButton *btnOk;
    QPushButton *btnCancel;

    QScrollArea    *scrollExamples;
    ExamplesWidget *wgtExamples;

    //=======================================================
};

#endif // NEWCARDDIALOG_H
