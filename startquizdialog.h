#ifndef STARTQUIZDIALOG_H
#define STARTQUIZDIALOG_H

#include <QtGui>
#include "wordschooser.h"
#include "wordscard.h"
#include "quizdialog.h"

class StartQuizDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartQuizDialog(WordsSet * words, QWidget *parent = 0);
    WordsPtrSet getWords();
    QuizMode getMode();

private:
    void createInterface();

    QButtonGroup *grpMode;
    QRadioButton *radioMultiChoiceMode;
    QRadioButton *radioNoChoiceMode;
    QRadioButton *radioRandomOrder;
    QRadioButton *radioListOrder;
    QRadioButton *radioWordHide;
    QRadioButton *radioTranslationHide;
    QRadioButton *radioRandomHide;
    QCheckBox *chckIncLearned;
    QCheckBox *chckExamples;
    QCheckBox *chckTranscription;
    QLineEdit *txtNumWords;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    WordsPtrSet mWords;
};

#endif // STARTQUIZDIALOG_H
