#ifndef STARTQUIZDIALOG_H
#define STARTQUIZDIALOG_H

#include <QDialog>
#include <QtGui>

class StartQuizDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartQuizDialog(QWidget *parent = 0);

private:
    void createInterface();

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
signals:

public slots:

};

#endif // STARTQUIZDIALOG_H
