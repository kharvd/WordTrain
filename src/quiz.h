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

#ifndef QUIZ_H
#define QUIZ_H

#include "quizquestion.h"

#include <QString>
#include <QList>

// Generic class for quizes
class Quiz
{
public:
    Quiz() {}

    void addQuestion(const QString &_question, const QString &_correctAnswer,
                     const Choices &_choices = Choices(), QuestionType _type = NoChoice);
    void addQuestion(const QuizQuestion &question);

    void removeQuestion(int index);

    void setQuestionAt(int index, const QString &_question, const QString &_correctAnswer,
                       const Choices &_choices = Choices(), QuestionType _type = NoChoice);
    void setQuestionAt(int index, const QuizQuestion &question);

    int correctAnswers() const;
    int incorrectAnswers() const;
    int questionsCount() const;

    QuizQuestion questionAt(int questionNumber) const;

    // Returns true if user's answer is correct and sets user's answer for question at index
    bool setUsersAnswer(int index, const QString &_answer);

    // Checks answer at index
    bool isUsersAnswerCorrect(int index) const;

private:
    QList<QuizQuestion> m_Questions;
};

#endif // QUIZ_H
