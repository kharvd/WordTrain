#include "neweditcarddialog.h"

/* Add card */
NewEditCardDialog::NewEditCardDialog(QWidget *parent) :
        QDialog(parent)
{
    setWindowTitle(tr("New card"));
    createInterface();
}

/* Edit card */
NewEditCardDialog::NewEditCardDialog(const WordCard &card, QWidget *parent) :
        QDialog(parent)
{
    mNewCard = card;
    setWindowTitle(tr("Edit card"));
    createInterface();
    fillForm();
}

const WordCard& NewEditCardDialog::getNewCard()
{
    mNewCard.setWord(txtWord->text());
    mNewCard.setTranscription(txtTranscription->text());
    mNewCard.setTranslation(txtTranslation->text());
    mNewCard.setPlural(txtPlural->text());
    mNewCard.setCategory(cmbCategory->currentIndex());
    mNewCard.setGender(cmbGender->currentIndex());
    mNewCard.setLearned(chckLearned->isChecked());
    mNewCard.clearExamples();

    for (int i = 0; (i < examplesCount()) && (i <= maxExamples); i++) {
        Example ex = getExampleAt(i);
        if (!ex.first.isEmpty()) {
            mNewCard.addExample(getExampleAt(i));
        }
    }

    return mNewCard;
}

int NewEditCardDialog::examplesCount() {
    return (ltExamples->count() / 4);
}

void NewEditCardDialog::addExample() {
    int count = examplesCount();
    if(++count <= maxExamples) {
        QLabel* lbl1 = new QLabel(tr("Example %1:").arg(count));
        QLineEdit* txt1 = new QLineEdit();
        QLabel* lbl2 = new QLabel(tr("Translation %1:").arg(count));
        QLineEdit* txt2 = new QLineEdit();

        ltExamples->addWidget(lbl1);
        ltExamples->addWidget(txt1);
        ltExamples->addWidget(lbl2);
        ltExamples->addWidget(txt2);
    }
    if (count == maxExamples) {
        btnAddExample->setEnabled(false);
    }

}

void NewEditCardDialog::createInterface()
{
    resize(500, 250);

    QLabel* lblWord = new QLabel(tr("Word:"));
    txtWord = new QLineEdit();

    QLabel* lblTranscription = new QLabel(tr("Transcription:"));
    txtTranscription = new QLineEdit();

    QLabel* lblTranslation = new QLabel(tr("Translation:"));
    txtTranslation = new QLineEdit();

    QLabel* lblPlural = new QLabel(tr("Plural:"));
    txtPlural = new QLineEdit();

    QLabel* lblCategory = new QLabel(tr("Category:"));
    cmbCategory = new QComboBox();
    cmbCategory->addItems(WordCard::lexCategoriesStrings());

    QLabel* lblGender = new QLabel(tr("Gender:"));
    cmbGender = new QComboBox();
    cmbGender->addItems(WordCard::genderStrings());

    QLabel* lblExamples = new QLabel(tr("Examples:"));
    btnAddExample = new QPushButton(tr("Add example"));
    connect(btnAddExample, SIGNAL(clicked()), SLOT(addExample()));

    ltExamples = new QVBoxLayout;

    btnOk = new QPushButton(tr("OK"));
    btnOk->setDefault(true);
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    chckLearned = new QCheckBox(tr("Learned?"));
    chckLearned->setChecked(false);

    QFormLayout* fLayout = new QFormLayout();
    fLayout->addRow(lblWord, txtWord);
    fLayout->addRow(lblTranscription, txtTranscription);
    fLayout->addRow(lblTranslation, txtTranslation);
    fLayout->addRow(lblPlural, txtPlural);
    fLayout->addRow(lblCategory, cmbCategory);
    fLayout->addRow(lblGender, cmbGender);
    fLayout->addRow(lblExamples, btnAddExample);
    fLayout->addRow(ltExamples);

    QVBoxLayout* hLayout = new QVBoxLayout();
    hLayout->addLayout(fLayout);
    hLayout->addWidget(chckLearned);
    hLayout->addStretch(1);

    QGroupBox* grBox = new QGroupBox(windowTitle());
    grBox->setLayout(hLayout);

    QVBoxLayout* hButtonsLayout = new QVBoxLayout();
    hButtonsLayout->addWidget(btnOk);
    hButtonsLayout->addWidget(btnCancel);
    hButtonsLayout->addStretch(1);

    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(grBox);
    mainLayout->addLayout(hButtonsLayout);

    setLayout(mainLayout);
}

void NewEditCardDialog::fillForm()
{
    txtWord->setText(mNewCard.word());
    txtTranscription->setText(mNewCard.transcription());
    txtTranslation->setText(mNewCard.translation());
    txtPlural->setText(mNewCard.plural());
    cmbCategory->setCurrentIndex(mNewCard.category());
    cmbGender->setCurrentIndex(mNewCard.gender());
    chckLearned->setChecked(mNewCard.isLearned());

    for (int i = 0; i < (mNewCard.examplesSize()) && (i <= maxExamples); i++) {
        addExample();
        setExampleAt(i, mNewCard.exampleAt(i));
    }
}

void NewEditCardDialog::setExampleAt(int index, Example ex) {
    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                  (index * 4 + 1)->widget());
    Q_ASSERT(example);

    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                  (index * 4 + 3)->widget());
    Q_ASSERT(translation);

    example->setText(ex.first);
    translation->setText(ex.second);
}

Example NewEditCardDialog::getExampleAt(int index) {
    QLineEdit* example = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                  (index * 4 + 1)->widget());
    Q_ASSERT(example);

    QLineEdit* translation = qobject_cast<QLineEdit *>(ltExamples->itemAt
                                                  (index * 4 + 3)->widget());
    Q_ASSERT(translation);

    Example tmp;
    tmp.first = example->text();
    tmp.second = translation->text();
    return tmp;
}
