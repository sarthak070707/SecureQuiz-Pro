#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentQuestionIndex(0)
    , score(0)
    , quizFinished(false)
{
    ui->setupUi(this);

    // Basic proctoring: log session start.
    logActivity("Quiz session started.");

    loadQuestions();

    if (!questions.isEmpty()) {
        showQuestion(currentQuestionIndex);
    } else {
        QMessageBox::critical(this, "Error", "No questions loaded. Check Quiz.txt.");
        logActivity("No questions loaded. Quiz aborted.");
        quizFinished = true;
        close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadQuestions()
{
    QFile file("Quiz.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open Quiz.txt");
        logActivity("Failed to open Quiz.txt.");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        Question q;
        q.question       = in.readLine();
        q.optionA        = in.readLine();
        q.optionB        = in.readLine();
        q.optionC        = in.readLine();
        q.optionD        = in.readLine();
        q.correctAnswer  = in.readLine();

        if (q.question.isEmpty() || q.correctAnswer.isEmpty())
            continue; // skip incomplete blocks

        questions.append(q);
    }

    logActivity(QString("Loaded %1 questions.").arg(questions.size()));
}

void MainWindow::showQuestion(int index)
{
    if (index < 0 || index >= questions.size())
        return;

    const Question &q = questions[index];

    ui->labelQuestion->setText(q.question);
    ui->pushButtonA->setText("A. " + q.optionA);
    ui->pushButtonB->setText("B. " + q.optionB);
    ui->pushButtonC->setText("C. " + q.optionC);
    ui->pushButtonD->setText("D. " + q.optionD);

    logActivity(QString("Showing question %1.").arg(index + 1));
}

void MainWindow::checkAnswer(const QString &selected)
{
    if (quizFinished || currentQuestionIndex >= questions.size())
        return;

    const Question &q = questions[currentQuestionIndex];
    QString correct = q.correctAnswer.trimmed().toLower();

    if (!selected.isEmpty()) {
        logActivity(QString("User selected option '%1' for question %2.")
                    .arg(selected)
                    .arg(currentQuestionIndex + 1));
    } else {
        logActivity(QString("No option selected for question %1.")
                    .arg(currentQuestionIndex + 1));
    }

    if (selected == correct) {
        score++;
        logActivity("Answer is correct.");
    } else {
        logActivity("Answer is incorrect.");
    }

    currentQuestionIndex++;

    if (currentQuestionIndex < questions.size()) {
        showQuestion(currentQuestionIndex);
    } else {
        showResult();
    }
}

void MainWindow::showResult()
{
    quizFinished = true;
    QString resultText = QString("Your score: %1 / %2")
                         .arg(score)
                         .arg(questions.size());

    QMessageBox::information(this, "Quiz Finished", resultText);
    logActivity("Quiz finished. " + resultText);

    close();
}

void MainWindow::logActivity(const QString &text)
{
    QFile logFile("proctor_log.txt");
    if (!logFile.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&logFile);
    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
        << " - "
        << text
        << "\n";
}

// Slots for buttons
void MainWindow::on_pushButtonA_clicked()
{
    checkAnswer("a");
}

void MainWindow::on_pushButtonB_clicked()
{
    checkAnswer("b");
}

void MainWindow::on_pushButtonC_clicked()
{
    checkAnswer("c");
}

void MainWindow::on_pushButtonD_clicked()
{
    checkAnswer("d");
}

// Proctoring: detect when window loses focus (user switches app)
void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        if (!this->isActiveWindow() && !quizFinished) {
            logActivity("WARNING: Window focus lost (user switched away from quiz).");
            QMessageBox::warning(this, "Proctoring Warning",
                                 "You switched away from the quiz window.\n"
                                 "Please stay on the quiz screen.");
        }
    }

    QMainWindow::changeEvent(event);
}

// Proctoring: log quiz end on close
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!quizFinished && !questions.isEmpty()) {
        logActivity("Quiz window closed before completion.");
    }
    logActivity("Quiz session ended.");
    QMainWindow::closeEvent(event);
}
