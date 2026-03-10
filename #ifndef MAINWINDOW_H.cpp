#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Question {
    QString question;
    QString optionA;
    QString optionB;
    QString optionC;
    QString optionD;
    QString correctAnswer;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void changeEvent(QEvent *event) override;     // For focus lost detection
    void closeEvent(QCloseEvent *event) override; // To log quiz end

private slots:
    void on_pushButtonA_clicked();
    void on_pushButtonB_clicked();
    void on_pushButtonC_clicked();
    void on_pushButtonD_clicked();

private:
    Ui::MainWindow *ui;
    QVector<Question> questions;
    int currentQuestionIndex;
    int score;
    bool quizFinished;

    void loadQuestions();
    void showQuestion(int index);
    void checkAnswer(const QString &selected);
    void showResult();
    void logActivity(const QString &text);
};

#endif // MAINWINDOW_H
