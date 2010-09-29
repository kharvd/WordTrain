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

    for (int i = 0; (i < wgtExamples->examplesCount())
        && (i <= ExamplesWidget::maxExamples); i++)
    {
        Example ex = wgtExamples->getExampleAt(i);
        if (!ex.first.isEmpty()) {
            mNewCard.addExample(wgtExamples->getExampleAt(i));
        }
    }

    return mNewCard;
}

void NewEditCardDialog::addExample() {
    scrollExamples->show();
    int optimalHeight = height() + 66;
    optimalHeight = (optimalHeight > 500) ? 500 : optimalHeight;
    resize(width(), optimalHeight);

    wgtExamples->addExample();
    if (!wgtExamples->canAdd()) {
        btnAddExample->setEnabled(false);
    }
}

void NewEditCardDialog::createInterface()
{
    resize(600, 300);
    setWindowFlags(Qt::Window);
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

    wgtExamples = new ExamplesWidget();
    scrollExamples = new QScrollArea;
    scrollExamples->setWidgetResizable(true);
    scrollExamples->setWidget(wgtExamples);
    scrollExamples->setFrameStyle(QFrame::NoFrame);
    scrollExamples->setAlignment(Qt::AlignTop);
    scrollExamples->hide();

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

    QVBoxLayout* hLayout = new QVBoxLayout();
    hLayout->addLayout(fLayout);
    hLayout->addWidget(scrollExamples);
    hLayout->addWidget(chckLearned);

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

    for (int i = 0; i < (mNewCard.examplesSize())
        && (i <= ExamplesWidget::maxExamples); i++)
    {
        addExample();
        wgtExamples->setExampleAt(i, mNewCard.exampleAt(i));
    }
}
