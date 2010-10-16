#include "startquizdialog.h"

StartQuizDialog::StartQuizDialog(WordsSet * words, QWidget *parent) :
    QDialog(parent)
{
    for (int i = 0; i < words->size(); i++) {
        mWords.push_back(&words[0][i]);
    }

    setWindowTitle(tr("Start test"));
    createInterface();
}

void StartQuizDialog::createInterface()
{
    QFormLayout *ltFormQuiz = new QFormLayout;

    // Mode
    radioMultiChoiceMode = new QRadioButton(tr("Multiple choice"));
    radioNoChoiceMode = new QRadioButton(tr("Without choice"));

    grpChoiceMode = new QButtonGroup(this);
    grpChoiceMode->addButton(radioMultiChoiceMode, 0);
    grpChoiceMode->addButton(radioNoChoiceMode, 1);

    radioNoChoiceMode->setChecked(true);

    QVBoxLayout *ltRadioMode = new QVBoxLayout;
    ltRadioMode->addWidget(radioMultiChoiceMode);
    ltRadioMode->addWidget(radioNoChoiceMode);

    ltFormQuiz->addRow(tr("Mode:", "Quiz"), ltRadioMode);


    // Num of words
    QLabel *lblNumWords = new QLabel(
            tr("Number of words (-1 for all words):"));

    txtNumWords = new QLineEdit("-1");
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
    radioTranslationHide = new QRadioButton(tr("Translation", "Hide"));
    radioWordHide = new QRadioButton(tr("Word", "Hide"));
    radioRandomHide = new QRadioButton(tr("Random", "Hide"));
    grpHideMode = new QButtonGroup(this);
    grpHideMode->addButton(radioTranslationHide, 0);
    grpHideMode->addButton(radioWordHide, 1);
    grpHideMode->addButton(radioRandomHide, 2);

    radioTranslationHide->setChecked(true);

    QVBoxLayout *ltRadioHide = new QVBoxLayout;
    ltRadioHide->addWidget(radioTranslationHide);
    ltRadioHide->addWidget(radioWordHide);
    ltRadioHide->addWidget(radioRandomHide);

    ltFormQuiz->addRow(tr("Hide:", "Quiz"), ltRadioHide);

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

WordsPtrSet StartQuizDialog::getWords()
{
    WordsChooser chooser(mWords, radioRandomOrder->isChecked(),
                         chckIncLearned->isChecked(),
                         txtNumWords->text().toInt());
    return chooser.getWords();
}

ChoiceMode StartQuizDialog::getChoiceMode()
{
    return (ChoiceMode)grpChoiceMode->checkedId();
}

HideMode StartQuizDialog::getHideMode() {
    return (HideMode)grpHideMode->checkedId();
}
