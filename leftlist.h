// #ifndef LEFTLIST_H
// #define LEFTLIST_H

// #include <QWidget>
// #include <QPushButton>
// #include <QLabel>
// #include <QListWidget>
// #include "searchengine.h"

// class LeftList : public QWidget
// {
//     Q_OBJECT
// public:
//     explicit LeftList(QWidget *parent = nullptr);
//     ~LeftList();

// private slots:
//     void clickedSearch();

// private:
//     QPushButton *searchButton;
//     QLabel *iconLabel;
//     QListWidget *taskList;
// };
// #endif // LEFTLIST_H


#pragma once
#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>


class LeftList : public QWidget
{
    Q_OBJECT
public:
    explicit LeftList(QWidget *parent = nullptr);
    ~LeftList();

signals:
    void searchClicked();

private:
    QPushButton *searchButton;
    QLabel *iconLabel;
    QListWidget *todoList;
};
