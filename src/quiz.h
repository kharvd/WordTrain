/******************************************************************************
** WordTrain 0.9.3 -- Foreign words trainer
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

#ifndef QUIZ_H
#define QUIZ_H

#include "quizquestion.h"
#include "wordcard.h"

#include <QString>
#include <QList>
#include <QtCore>

class QString;

// Generic class for quizes. Task - type of asked question, Answer - type of answer
template <class Task = QString, class Answer = QString>
class Quiz
{
public:
    Quiz(bool _exactComparsion = false)
        : m_exactComparison(_exactComparsion) {}

    void addQuestion(const Task &_task, const Answer &_correctAnswer,
                     const QList<Answer> &_choices = QList<Answer>(),
                     QuestionType _type = QuestionTypeNoChoice);
    void setQuestionAt(int index, const Task &_task, const Answer &_correctAnswer,
                       const QList<Answer> &_choices = QList<Answer>(),
                       QuestionType _type = QuestionTypeNoChoice);
    void removeQuestionAt(int index);
    void clear();

    Task taskAt(int index) const;
    void setTaskAt(int index, const Task &_task);

    Answer correctAnswerAt(int index) const;
    void setCorrectAnswerAt(int index, const Answer &_correctAnswer);

    QList<Answer> choicesAt(int index) const;
    void addChoiceAt(int index, const Answer &_choice);
    void setChoicesAt(int index, const QList<Answer> &_choices);
    void clearChoicesAt(int index);

    QuestionType typeAt(int index) const;
    void setTypeAt(int index, QuestionType _type);

    // Returns true if user's answer is correct and sets user's answer for question at index
    bool setUsersAnswerAt(int index, const Answer &_answer);
    Answer usersAnswerAt(int index) const;
    bool isUsersAnswerCorrectAt(int index) const;

    // Returns number of correct answers in the quiz
    int correctAnswers() const;
    int incorrectAnswers() const;
    int questionsCount() const;

private:
    // If true, correct answer is compared with user's one exactly, otherwise
    // any substrring of the correct answer is considered as correct (only if Answer == QString)
    bool m_exactComparison;
    QList<QuizQuestion<Task, Answer> > m_Questions;
};

template <class Task, class Answer>
void Quiz<Task, Answer>::addQuestion(const Task &_task,
                                     const Answer &_correctAnswer,
                                     const QList<Answer> &_choices,
                                     QuestionType _type)
{
    QuizQuestion<Task, Answer> q(_task, _correctAnswer, _choices, _type);
    m_Questions.push_back(q);
}

template <class Task, class Answer>
void Quiz<Task, Answer>::setQuestionAt(int index, const Task &_task,
                                       const Answer &_correctAnswer,
                                       const QList<Answer> &_choices,
                                       QuestionType _type)
{
    QuizQuestion<Task, Answer> q(_task, _correctAnswer, _choices, _type);
    m_Questions.replace(index, q);
}

template <class Task, class Answer>
void Quiz<Task, Answer>::removeQuestionAt(int index)
{
    m_Questions.removeAt(index);
}

template <class Task, class Answer>
void Quiz<Task, Answer>::clear()
{
    m_Questions.clear();
}

template <class Task, class Answer>
Task Quiz<Task, Answer>::taskAt(int index) const
{
    return m_Questions.at(index).task();
}

template <class Task, class Answer>
void Quiz<Task, Answer>::setTaskAt(int index, const Task &_task)
{
    m_Questions[index].setTask(_task);
}

template <class Task, class Answer>
Answer Quiz<Task, Answer>::correctAnswerAt(int index) const
{
    return m_Questions.at(index).correctAnswer();
}

template <class Task, class Answer>
void Quiz<Task, Answer>::setCorrectAnswerAt(int index,
                                            const Answer &_correctAnswer)
{
    m_Questions[index].setCorrectAnswer(_correctAnswer);
}

template <class Task, class Answer>
QList<Answer> Quiz<Task, Answer>::choicesAt(int index) const
{
    return m_Questions.at(index).choices();
}

template <class Task, class Answer>
void Quiz<Task, Answer>::addChoiceAt(int index, const Answer &_choice)
{
    m_Questions[index].addChoice(_choice);
}

template <class Task, class Answer>
void Quiz<Task, Answer>::setChoicesAt(int index, const QList<Answer> &_choices)
{
    m_Questions[index].setChoices(_choices);
}

template <class Task, class Answer>
void Quiz<Task, Answer>::clearChoicesAt(int index)
{
    m_Questions[index].clearChoices();
}

template <class Task, class Answer>
QuestionType Quiz<Task, Answer>::typeAt(int index) const
{
    return m_Questions.at(index).type();
}

template <class Task, class Answer>
void Quiz<Task, Answer>::setTypeAt(int index, QuestionType _type)
{
    m_Questions[index].setType(_type);
}

template <class Task, class Answer>
bool Quiz<Task, Answer>::setUsersAnswerAt(int index, const Answer &_answer)
{
    m_Questions[index].setUsersAnswer(_answer);
    return isUsersAnswerCorrectAt(index);
}

template <class Task, class Answer>
Answer Quiz<Task, Answer>::usersAnswerAt(int index) const
{
    return m_Questions.at(index).usersAnswer();
}

template <class Task, class Answer>
extern bool Quiz<Task, Answer>::isUsersAnswerCorrectAt(int index) const
{
    qDebug() << "bool Quiz<Task, Answer>::isUsersAnswerCorrectAt(int index) const";
    return (m_Questions.at(index).usersAnswer()
               == m_Questions.at(index).correctAnswer());
}

// Defined in quiz.cpp
template <>
bool Quiz<WordCard, QString>::isUsersAnswerCorrectAt(int index) const;

template <class Task, class Answer>
int Quiz<Task, Answer>::correctAnswers() const
{
    int count = 0;

    for (int i = 0; i < m_Questions.size(); i++) {
        if (isUsersAnswerCorrectAt(i))
            count++;
    }

    return count;
}

template <class Task, class Answer>
int Quiz<Task, Answer>::incorrectAnswers() const
{
    return (m_Questions.size() - correctAnswers());
}

template <class Task, class Answer>
int Quiz<Task, Answer>::questionsCount() const
{
    return m_Questions.size();
}

#endif // QUIZ_H
