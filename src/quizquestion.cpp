#include "quizquestion.h"

QuizQuestion::QuizQuestion(const QString &_question, const QString &_correctAnswer,
                           const Choices &_choices, QuestionType _type)
    : m_Question(_question), m_CorrectAnswer(_correctAnswer),
      m_Choices(_choices), m_Type(_type)
{

}

void QuizQuestion::setQuestion(const QString &_question)
{
    m_Question = _question;
}

void QuizQuestion::setCorrectAnswer(const QString &_correctAnswer)
{
    m_CorrectAnswer = _correctAnswer;
}

void QuizQuestion::setUsersAnswer(const QString &_usersAnswer)
{
    m_UsersAnswer = _usersAnswer;
}

void QuizQuestion::setType(QuestionType _type)
{
    m_Type = _type;
}

void QuizQuestion::addChoice(const QString &_choice)
{
    m_Choices.push_back(_choice);
}

void QuizQuestion::setChoiceAt(int index, const QString &_choice)
{
    m_Choices[index] = _choice;
}

void QuizQuestion::removeChoice(int index)
{
    m_Choices.removeAt(index);
}

void QuizQuestion::removeChoice(const QString &_choice)
{
    m_Choices.removeAll(_choice);
}
