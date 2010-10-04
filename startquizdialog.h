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
    ChoiceMode getChoiceMode();
    HideMode getHideMode();

private:
    void createInterface();

    QButtonGroup *grpChoiceMode;
    QButtonGroup *grpHideMode;
    QRadioButton *radioMultiChoiceMode;
    QRadioButton *radioNoChoiceMode;
    QRadioButton *radioRandomOrder;
    QRadioButton *radioListOrder;
    QRadioButton *radioWordHide;
    QRadioButton *radioTranslationHide;
    QRadioButton *radioRandomHide;
    QCheckBox *chckIncLearned;
    QLineEdit *txtNumWords;
    QPushButton *btnOk;
    QPushButton *btnCancel;
    WordsPtrSet mWords;
};

#endif // STARTQUIZDIALOG_H
