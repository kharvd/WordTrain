/******************************************************************************
** WordTrain 0.9.1 -- Foreign words trainer
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

#include "quiz.h"

#include <QString>
#include <QList>

void Quiz::addQuestion(const QString &_question, const QString &_correctAnswer,
                           const Choices &_choices, QuestionType _type)
{
    QuizQuestion q(_question, _correctAnswer, _choices, _type);
    m_Questions.push_back(q);
}

void Quiz::addQuestion(const QuizQuestion &question)
{
    m_Questions.push_back(question);
}

void Quiz::removeQuestion(int index)
{
    m_Questions.removeAt(index);
}

void Quiz::setQuestionAt(int index, const QString &_question, const QString &_correctAnswer,
                   const Choices &_choices, QuestionType _type)
{
    QuizQuestion q(_question, _correctAnswer, _choices, _type);
    m_Questions[index] = q;
}

void Quiz::setQuestionAt(int index, const QuizQuestion &question)
{
    m_Questions[index] = question;
}

int Quiz::correctAnswers() const
{
    int count = 0;

    foreach (QuizQuestion q, m_Questions) {
        if (q.correctAnswer() == q.usersAnswer()) {
            count++;
        }
    }

    return count;
}

int Quiz::incorrectAnswers() const
{
    return (m_Questions.size() - correctAnswers());
}

int Quiz::questionsCount() const
{
    return m_Questions.size();
}

QuizQuestion Quiz::questionAt(int questionNumber) const
{
    return m_Questions.at(questionNumber);
}

bool Quiz::setUsersAnswer(int index, const QString &_answer)
{
    m_Questions[index].setUsersAnswer(_answer);
    return (_answer == m_Questions.at(index).correctAnswer());
}

bool Quiz::isUsersAnswerCorrect(int index) const
{
    return (m_Questions.at(index).usersAnswer()
            == m_Questions.at(index).correctAnswer());
}
