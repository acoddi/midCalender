// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include"CalendarWidget.h"
// #include "leftlist.h"
// #include"schedule.h"
// #include "searchengine.h"
// #include <QMainWindow>

// QT_BEGIN_NAMESPACE
// namespace Ui {
// class MainWindow;
// }
// QT_END_NAMESPACE

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();

// private:
//     Ui::MainWindow *ui;
//     CalendarWidget *calender;
//     Schedule *schedule;
//     searchengine *searchengine;
//     LeftList *left;
// };
// #endif // MAINWINDOW_H


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>

#include "calendarwidget.h"
#include "schedule.h"
#include "leftlist.h"
#include "searchengine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 🔍 검색 요청 처리
    void performSearch(const QString &keyword);

    // 🔎 검색 결과 더블클릭 → 달력 강조 이동
    void highlightFromSearch(const QDate &date);

    // 🔎 검색창 열기
    void openSearchWindow();

private:
    Ui::MainWindow *ui;

    CalendarWidget *calender;   // 중앙 달력
    Schedule *schedule;         // 오른쪽 일정 창
    LeftList *left;             // 왼쪽 리스트
    QDate currentSelectedDate;  // 현재 더블클릭한 날짜

    searchengine *activeSearchEngine = nullptr; // 현재 열린 검색창
};

#endif // MAINWINDOW_H


