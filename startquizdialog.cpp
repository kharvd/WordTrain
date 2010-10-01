#include "startquizdialog.h"

StartQuizDialog::StartQuizDialog(QWidget *parent) :
    QDialog(parent)
{
    createInterface();
    setWindowTitle(tr("Start test"));
}

void StartQuizDialog::createInterface()
{
    QFormLayout *ltFormQuiz = new QFormLayout;

    // Mode
    radioMultiChoiceMode = new QRadioButton(tr("Multiple choice"));
    radioNoChoiceMode = new QRadioButton(tr("Without choice"));
    QButtonGroup *grpMode = new QButtonGroup(this);
    grpMode->addButton(radioMultiChoiceMode);
    grpMode->addButton(radioNoChoiceMode);
    radioMultiChoiceMode->setEnabled(false);
    radioNoChoiceMode->setChecked(true);

    QVBoxLayout *ltRadioMode = new QVBoxLayout;
    ltRadioMode->addWidget(radioMultiChoiceMode);
    ltRadioMode->addWidget(radioNoChoiceMode);

    ltFormQuiz->addRow(tr("Mode:", "Quiz"), ltRadioMode);


    // Num of words
    QLabel *lblNumWords = new QLabel(
            tr("Number of words (-1 for all words):"));

    txtNumWords = new QLineEdit();
    QIntValidator *validator = new QIntValidator(-1, 1000, this);
    txtNumWords->setValidator(validator);

    QHBoxLayout *ltNumWords = new QHBoxLayout;
    ltNumWords->addWidget(lblNumWords);
    ltNumWords->addWidget(txtNumWords, 1);

    ltFormQuiz->addRow(ltNumWords);

    // Include learned words
    chckIncLearned = new QCheckBox(tr("Include learned words"));
    ltFormQuiz->addRow(chckIncLearned);

    // Order
    radioRandomOrder = new QRadioButton(tr("Random", "Order"));
    radioListOrder = new QRadioButton(tr("As in list"));
    QButtonGroup *grpOrder = new QButtonGroup(this);
    grpOrder->addButton(radioRandomOrder);
    grpOrder->addButton(radioListOrder);
    radioRandomOrder->setChecked(true);

    QVBoxLayout *ltRadioOrder = new QVBoxLayout;
    ltRadioOrder->addWidget(radioRandomOrder);
    ltRadioOrder->addWidget(radioListOrder);

    ltFormQuiz->addRow(tr("Order:", "Quiz"), ltRadioOrder);

    // Hiding
    radioTranslationHide = new QRadioButton(tr("Transcription", "Hide"));
    radioWordHide = new QRadioButton(tr("Word", "Hide"));
    radioRandomHide = new QRadioButton(tr("Random", "Hide"));
    QButtonGroup *grpHide = new QButtonGroup(this);
    grpHide->addButton(radioTranslationHide);
    grpHide->addButton(radioWordHide);
    grpHide->addButton(radioRandomHide);
    radioTranslationHide->setChecked(true);

    QVBoxLayout *ltRadioHide = new QVBoxLayout;
    ltRadioHide->addWidget(radioTranslationHide);
    ltRadioHide->addWidget(radioWordHide);
    ltRadioHide->addWidget(radioRandomHide);

    ltFormQuiz->addRow(tr("Hide:", "Quiz"), ltRadioHide);

    // Show examples
    chckExamples = new QCheckBox(tr("Examples", "Quiz"));
    ltFormQuiz->addRow(chckExamples);

    // Show transcription
    chckTranscription = new QCheckBox(tr("Transcrption", "Quiz"));
    ltFormQuiz->addRow(chckTranscription);

    // Main stuff
    QGroupBox *grpBox = new QGroupBox(windowTitle());
    grpBox->setLayout(ltFormQuiz);

    btnOk = new QPushButton(tr("OK"));
    connect(btnOk, SIGNAL(clicked()), SLOT(accept()));
    btnCancel = new QPushButton(tr("Cancel"));
    connect(btnCancel, SIGNAL(clicked()), SLOT(reject()));

    QVBoxLayout *ltBtns = new QVBoxLayout();
    ltBtns->addWidget(btnOk);
    ltBtns->addWidget(btnCancel);
    ltBtns->addStretch(1);

    QHBoxLayout *ltMain = new QHBoxLayout();
    ltMain->addWidget(grpBox);
    ltMain->addLayout(ltBtns);

    setLayout(ltMain);
}
