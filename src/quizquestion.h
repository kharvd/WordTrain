#ifndef QUIZQUESTION_H
#define QUIZQUESTION_H

#include <QStringList>
#include <QString>

typedef QStringList Choices;
enum QuestionType { Choice_MultiChoice = 0, Choice_NoChoice };

class QuizQuestion
{
public:
    QuizQuestion(QuestionType _type = MultiChoice)
        : m_Type(_type) {}

    QuizQuestion(const QString &_question, const QString &_correctAnswer,
                 const Choices &_choices = Choices(), QuestionType _type = NoChoice);

    void setQuestion(const QString &_question);
    void setCorrectAnswer(const QString &_correctAnswer);
    void setUsersAnswer(const QString &_usersAnswer);
    void setType(QuestionType _type);
    void addChoice(const QString &_choice);
    void setChoiceAt(int index, const QString &_choice);
    void removeChoice(int index);
    void removeChoice(const QString &_choice);

    QString question() const { return m_Question; }
    QString correctAnswer() const { return m_CorrectAnswer; }
    QString usersAnswer() const { return m_UsersAnswer; }
    QString choiceAt(int index) const { return m_Choices.at(index); }
    QuestionType type() const { return m_Type; }

private:
    QString m_Question;
    QString m_CorrectAnswer;
    QString m_UsersAnswer;
    Choices m_Choices;
    QuestionType m_Type;
};

#endif // QUIZQUESTION_H
