/******************************************************************************
** WordTrain 0.9.2 -- Foreign words trainer
** Copyright (C) 2010  Valery Kharitonov <kharvd@gmail.com>
**
** This file is part of WordTrain.
**
** $QT_BEGIN_LICENSE:GPL$
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
** $QT_END_LICENSE$
**
** If you have questions regarding the use of this file, please contact me at
** kharvd@gmail.com.
**
******************************************************************************/

#ifndef QUIZQUESTION_H
#define QUIZQUESTION_H

#include <QStringList>
#include <QString>

enum QuestionType {
    QuestionTypeFirst = 0, QuestionTypeMultiChoice = 0, QuestionTypeNoChoice,
    QuestionTypeRandom, QuestionTypeLast = QuestionTypeRandom
};

// Quiz question. Task - type of the task (default QString),
// Answer - type of answer and choices
template <class Task = QString, class Answer = QString>
class QuizQuestion
{
public:
    QuizQuestion(QuestionType _type = QuestionTypeMultiChoice)
        : m_Type(_type) {}

    QuizQuestion(const Task &_task, const Answer &_correctAnswer,
                 const QList<Answer> &_choices = QList<Answer>(),
                 QuestionType _type = QuestionTypeNoChoice);

    Task task() const { return m_Task; }
    void setTask(const Task &_task);

    Answer correctAnswer() const { return m_CorrectAnswer; }
    void setCorrectAnswer(const Answer &_correctAnswer);

    Answer usersAnswer() const { return m_UsersAnswer; }
    void setUsersAnswer(const Answer &_usersAnswer);

    QuestionType type() const { return m_Type; }
    void setType(QuestionType _type);

    QList<Answer> choices() const { return m_Choices; }
    void setChoices(const QList<Answer> &_choices);

    Answer choiceAt(int index) const { return m_Choices.at(index); }
    void addChoice(const Answer &_choice);
    void setChoiceAt(int index, const Answer &_choice);
    void removeChoice(int index);
    void clearChoices() { m_Choices.clear(); }

private:
    Task m_Task;
    Answer m_CorrectAnswer;
    Answer m_UsersAnswer;
    QList<Answer> m_Choices;
    QuestionType m_Type;
};

template <class Task, class Answer>
QuizQuestion<Task, Answer>::QuizQuestion(const Task &_task,
                                         const Answer &_correctAnswer,
                                         const QList<Answer> &_choices,
                                         QuestionType _type)
    : m_Task(_task), m_Choices(_choices),
      m_CorrectAnswer(_correctAnswer), m_Type(_type)
{
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setTask(const Task &_task)
{
    m_Task = _task;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setCorrectAnswer(const Answer &_correctAnswer)
{
    m_CorrectAnswer = _correctAnswer;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setUsersAnswer(const Answer &_usersAnswer)
{
    m_UsersAnswer = _usersAnswer;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setType(QuestionType _type)
{
    m_Type = _type;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setChoices(const QList<Answer> &_choices)
{
    m_Choices = _choices;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::addChoice(const Answer &_choice)
{
    m_Choices.push_back(_choice);
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::setChoiceAt(int index, const Answer &_choice)
{
    m_Choices[index] = _choice;
}

template <class Task, class Answer>
void QuizQuestion<Task, Answer>::removeChoice(int index)
{
    m_Choices.removeAt(index);
}

#endif // QUIZQUESTION_H
