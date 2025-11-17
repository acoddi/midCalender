// #ifndef CALENDARWIDGET_H
// #define CALENDARWIDGET_H

// #include <QWidget>
// #include<QTableWidget>
// #include<QPushButton>
// #include <QLabel>

// class CalendarWidget : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit CalendarWidget(QWidget *parent = nullptr);
//     void resizeEvent(QResizeEvent *event) override;

// private slots:
//     void showNextMonth();
//     void showPrevMonth();
//     void onCellDoubleClicked(int row, int column);

// private:
//     void setupUI();
//     void populateCalendar();

//     int year;
//     int month;
//     QTableWidget *table;
//     QPushButton *nextBtn;
//     QPushButton *prevBtn;
//     QLabel *monthLabel;

// signals:
//     void dateDoubleClicked(const QDate &date);
// };

// #endif // CALENDARWIDGET_H




// #pragma once
// #include <QWidget>
// #include <QTableWidget>
// #include <QPushButton>
// #include <QLabel>
// #include <QDate>
// #include "leftlist.h"

// class CalendarWidget : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit CalendarWidget(QWidget *parent = nullptr);

// signals:
//     void dateDoubleClicked(const QDate &date);

// private:
//     void setupUI();
//     void populateCalendar();
//     void showPrevMonth();
//     void showNextMonth();
//     void resizeEvent(QResizeEvent *event) override;

//     int year, month;
//     QTableWidget *table;
//     QPushButton *prevBtn;
//     QPushButton *nextBtn;
//     QLabel *monthLabel;
// };


///////////////////////////////////////////////////////////////수정 전


#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QMap>
#include <QStringList>
#include <QResizeEvent>

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);

    void addSchedule(const QDate &date, const QString &text);
    void removeSchedule(const QDate &date, const QString &text);

private slots:
    void showNextMonth();
    void showPrevMonth();
    void onCellDoubleClicked(int row, int column);


private:
    void setupUI();
    void populateCalendar();
    void updateCellSchedules(const QDate &date);

    int year;
    int month;
    QTableWidget *table;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QLabel *monthLabel;
    QMap<QDate, QStringList> scheduleData;

signals:
    void dateDoubleClicked(const QDate &date);

protected:
    void resizeEvent(QResizeEvent *event) override; // 캘린더 크기/위치 조정용
};

#endif // CALENDARWIDGET_H

