// #ifndef SCHEDULE_H
// #define SCHEDULE_H

// #include <QWidget>
// #include<QTabWidget>

// class Schedule : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit Schedule(QWidget *parent = nullptr);

// signals:
// private:
//     QTabWidget *todoWidget;
// };

// #endif // SCHEDULE_H

/////////////////////////////////////////////////////////////////////수정 전 코드
// #pragma once
// #include <QWidget>
// #include <QTabWidget>
// #include <QTextEdit>
// #include <QDateEdit>
// #include <QPushButton>

// class Schedule : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit Schedule(QWidget *parent = nullptr);

//     // 나중에 MainWindow에서 할 일 추가 시그널 연결 가능
//     // signals:
//     // void todoAdded(const QString &text, const QDate &start, const QDate &end);

// private:
//     QTabWidget *todoWidget;
// };

///////////////////////////////////////////////////////////////////수정 후 코드(레이아웃만)


#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <QTabWidget>
#include <QDate>
#include <QListWidget>
#include <QDateEdit>
#include <QLabel>
#include <QTextEdit>
#include <QMap>
#include <QStringList>

class Schedule : public QWidget
{
    Q_OBJECT
public:
    explicit Schedule(QWidget *parent = nullptr);

    void addTodoItem(const QString &text, const QDate &start, const QDate &end);  // 일정 추가
    void setSelectedDate(const QDate &date);  // 선택된 날짜 설정

    QTabWidget *todoWidget;  // 할 일 목록을 표시하는 탭 위젯
    QListWidget *todoListWidget;  // 할 일 목록을 표시하는 리스트 위젯
    QTextEdit *todoEdit;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QDate selectedDate;  // 현재 선택된 날짜
    QLabel *fromLabel;

    // 날짜별 할 일 목록 저장
    QMap<QDate, QStringList> todosByDate;

    void refreshTodoList();   // 현재 선택된 날짜의 리스트를 UI에 반영


public slots:

signals:
    void todoAdded(const QString &text, const QDate &start, const QDate &end);  // 할 일 추가 시그널
    void todoRemoved(const QString &text, const QDate &date);                   // 특정 날짜에서 할 일 삭제 시그널

private:




};

#endif // SCHEDULE_H


