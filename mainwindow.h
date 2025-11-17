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
#include <QMap>
#include <QList>
#include "calendarwidget.h"
#include "schedule.h"
#include "leftlist.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CalendarWidget *calender;
    Schedule *schedule;
    QDate currentSelectedDate;
    LeftList *left;
};

#endif // MAINWINDOW_H
