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

#include "neweditcarddialog.h"

#include <QLineEdit>
#include <QComboBox>
#include <QScrollArea>
#include <QFormLayout>
#include <QGroupBox>
#include <QScrollBar>
#include <QPushButton>
#include "exampleswidget.h"

/* Add card */
NewEditCardDialog::NewEditCardDialog(QWidget *parent) :
        QDialog(parent)
{
    m_isNew = true;
    setWindowTitle(tr("New card"));
    createInterface();
}

/* Edit card */
NewEditCardDialog::NewEditCardDialog(const WordCard &card, QWidget *parent) :
        QDialog(parent)
{
    m_isNew = false;
    m_NewCard = card;
    setWindowTitle(tr("Edit card"));
    createInterface();
    fillForm();
}

WordCard NewEditCardDialog::newCard()
{
    // Getting data from the form
    m_NewCard.setWord(txtWord->text());
    m_NewCard.setTranscription(txtTranscription->text());
    m_NewCard.setTranslation(txtTranslation->text());
    m_NewCard.setPlural(txtPlural->text());
    m_NewCard.setCategory(cmbCategory->currentIndex());
    m_NewCard.setGender(cmbGender->currentIndex());
    m_NewCard.clearExamples();

    for (int i = 0; (i < wgtExamples->examplesCount())
        && (i <= ExamplesWidget::kMaxExamples); i++)
    {
        Example ex = wgtExamples->exampleAt(i);
        if (!ex.first.isEmpty())
            m_NewCard.addExample(wgtExamples->exampleAt(i));
    }

    QStringList tags = txtTags->text().split(",", QString::SkipEmptyParts);
    m_NewCard.setTags(tags);

    return m_NewCard;
}

void NewEditCardDialog::addExample()
{
    // Adding new example and resizing the window
    scrollExamples->show();

    // Height for window with examples
    int optimalHeight = height() + kExampleHeight;
    optimalHeight = (optimalHeight > kMaxAutoHeight) ? kMaxAutoHeight
                                                    : optimalHeight;
    resize(width(), optimalHeight);

    wgtExamples->addExample();
    scrollExamples->verticalScrollBar()->
            triggerAction(QAbstractSlider::SliderToMaximum);

    // If we cannot add another example
    if (!wgtExamples->canAdd())
        btnAddExample->setEnabled(false);
}

void NewEditCardDialog::resetProgress()
{
    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();
    txtProgress->setText(QString("0/%1").arg(corrAnsForLearned));
    m_NewCard.setCorrectAnswers(0);
}

void NewEditCardDialog::switchPluralGender(int cat)
{
    // If the word is noun, show plural and gender fields, otherwise hide them
    if (cat == (int)CategoryNoun) {
        txtPlural->show();
        cmbGender->show();
        ltFields->labelForField(txtPlural)->show();
        ltFields->labelForField(cmbGender)->show();
    } else {
        txtPlural->hide();
        cmbGender->hide();
        ltFields->labelForField(txtPlural)->hide();
        ltFields->labelForField(cmbGender)->hide();
    }
}

void NewEditCardDialog::createInterface()
{
    resize(kDefaultWidth, kDefaultHeight);
    txtWord = new QLineEdit();

    txtTranscription = new QLineEdit();
    txtTranslation = new QLineEdit();
    txtPlural = new QLineEdit();
    txtTags = new QLineEdit();

    cmbCategory = new QComboBox();
    cmbCategory->addItems(WordCard::lexCategoriesStrings());

    cmbGender = new QComboBox();
    cmbGender->addItems(WordCard::genderStrings());

    btnAddExample = new QPushButton(tr("Add example"));
    connect(btnAddExample, SIGNAL(clicked()), SLOT(addExample()));

    wgtExamples = new ExamplesWidget();
    scrollExamples = new QScrollArea;
    scrollExamples->setWidgetResizable(true);
    scrollExamples->setWidget(wgtExamples);
    scrollExamples->setFrameStyle(QFrame::NoFrame);
    scrollExamples->setAlignment(Qt::AlignTop);
    scrollExamples->hide();

    QPushButton *btnOk;
    QPushButton *btnCancel;
    btnOk = new QPushButton(tr("OK"));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QHBoxLayout *progressLayout = NULL;
    QPushButton *btnResetProgress = NULL;

    // If we create new card, don't show progress field and "Reset button"
    if (!m_isNew) {
        txtProgress = new QLineEdit();
        txtProgress->setReadOnly(true);

        btnResetProgress = new QPushButton(tr("Reset progress"));
        connect(btnResetProgress, SIGNAL(clicked(bool)), SLOT(resetProgress()));

        progressLayout = new QHBoxLayout;
        progressLayout->addWidget(txtProgress);
        progressLayout->addWidget(btnResetProgress);
        progressLayout->addStretch();
    }

    ltFields = new QFormLayout();
    ltFields->addRow(tr("Word:"), txtWord);
    ltFields->addRow(tr("Transcription:"), txtTranscription);
    ltFields->addRow(tr("Translation:"), txtTranslation);
    ltFields->addRow(tr("Category:"), cmbCategory);
    ltFields->addRow(tr("Plural:"), txtPlural);
    ltFields->addRow(tr("Gender:"), cmbGender);

    if (!m_isNew) {
        ltFields->addRow(tr("Progress:"), progressLayout);
    }

    ltFields->addRow(tr("Tags (comma-separated):"), txtTags);
    ltFields->addRow(tr("Examples:"), btnAddExample);

    connect(cmbCategory, SIGNAL(currentIndexChanged(int)),
            SLOT(switchPluralGender(int)));
    switchPluralGender(0);

    QVBoxLayout *hLayout = new QVBoxLayout();
    hLayout->addLayout(ltFields);
    hLayout->addWidget(scrollExamples);

    QGroupBox *grBox = new QGroupBox(windowTitle());
    grBox->setLayout(hLayout);

    QVBoxLayout *hButtonsLayout = new QVBoxLayout();
    hButtonsLayout->addWidget(btnOk);
    hButtonsLayout->addWidget(btnCancel);
    hButtonsLayout->addStretch(1);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(grBox);
    mainLayout->addLayout(hButtonsLayout);

    setLayout(mainLayout);
}

void NewEditCardDialog::fillForm()
{
    QSettings settings;
    int corrAnsForLearned = settings.value("corr_answers", 10).toInt();

    // Filling the form with the word card's contents
    txtWord->setText(m_NewCard.word());
    txtTranscription->setText(m_NewCard.transcription());
    txtTranslation->setText(m_NewCard.translation());
    txtPlural->setText(m_NewCard.plural());
    txtProgress->setText(QString("%1/%2").arg(m_NewCard.correctAnswers())
                         .arg(corrAnsForLearned));
    cmbCategory->setCurrentIndex(m_NewCard.category());
    cmbGender->setCurrentIndex(m_NewCard.gender());

    QString tags;
    foreach (QString tag, m_NewCard.tags()) {
        tags += tag + ", ";
    }
    // Chop ", " from the end
    tags.chop(2);
    txtTags->setText(tags);

    for (int i = 0; i < (m_NewCard.examplesSize())
            && (i <= ExamplesWidget::kMaxExamples); i++) {
        addExample();
        wgtExamples->setExampleAt(i, m_NewCard.exampleAt(i));
    }
}
