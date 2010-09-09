#ifndef NEWCARDDIALOG_H
#define NEWCARDDIALOG_H

#include <QtGui>
#include "wordscard.h"
#include "exampleswidget.h"

// Create new card or edit existing one
class NewEditCardDialog : public QDialog
{
    Q_OBJECT
public:
    /* If we want to add new card (you should use getNewCard() to get values) */
    NewEditCardDialog(QWidget *parent = 0);
    /* If we want to edit existing card (we don't change value of card directly,
       you should use getNewCard() to get new values) */
    NewEditCardDialog(const WordCard &card, QWidget *parent = 0);
    const WordCard& getNewCard();

signals:

public slots:
    void addExample();

private:
    void createInterface();
    /* Fills the form with current card's content */
    void fillForm();

//    void setExampleAt(int index, Example ex);
//    Example getExampleAt(int index);
//    int examplesCount();

    QLineEdit* txtWord;
    QLineEdit* txtTranscription;
    QLineEdit* txtTranslation;
    QLineEdit* txtPlural;
    QComboBox* cmbCategory;
    QComboBox* cmbGender;
    QCheckBox* chckLearned;
    QPushButton* btnAddExample;
    QPushButton* btnOk;
    QPushButton* btnCancel;
    ExamplesWidget* wgtExamples;
    QScrollArea *scrollExamples;
//    QVBoxLayout* ltExamples;


    /* Temporary card for holding new values */
    WordCard mNewCard;
};

#endif // NEWCARDDIALOG_H
