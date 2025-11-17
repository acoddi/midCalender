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


#pragma once
#include <QWidget>
#include <QTabWidget>
#include <QDate>

class Schedule : public QWidget
{
    Q_OBJECT
public:
    explicit Schedule(QWidget *parent = nullptr);

signals:
    void todoAdded(const QString &text, const QDate &start, const QDate &end);

private:
    QTabWidget *todoWidget;
};
