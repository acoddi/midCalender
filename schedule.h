#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <QTabWidget>
#include <QDate>
#include <QListWidget>
#include <QDateEdit>
#include <QLabel>
#include <QTextEdit>

class Schedule : public QWidget
{
    Q_OBJECT
public:
    explicit Schedule(QWidget *parent = nullptr);

    void addTodoItem(const QString &text);  // 일정 추가
    void setSelectedDate(const QDate &date);  // 선택된 날짜 설정

public slots:

signals:
    void todoAdded(const QString &text, const QDate &start, const QDate &end);  // 할 일 추가 시그널

private:
    QTabWidget *todoWidget;  // 할 일 목록을 표시하는 탭 위젯
    QListWidget *todoListWidget;  // 할 일 목록을 표시하는 리스트 위젯
    QTextEdit *todoEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QDate selectedDate;  // 현재 선택된 날짜
    QLabel *fromLabel;
};

#endif // SCHEDULE_H
