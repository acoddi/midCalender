// // #ifndef LEFTLIST_H
// // #define LEFTLIST_H

// // #include <QWidget>
// // #include <QPushButton>
// // #include <QLabel>
// // #include <QListWidget>
// // #include "searchengine.h"

// // class LeftList : public QWidget
// // {
// //     Q_OBJECT
// // public:
// //     explicit LeftList(QWidget *parent = nullptr);
// //     ~LeftList();

// // private slots:
// //     void clickedSearch();

// // private:
// //     QPushButton *searchButton;
// //     QLabel *iconLabel;
// //     QListWidget *taskList;
// // };
// // #endif // LEFTLIST_H


// // #pragma once
// // #include <QWidget>
// // #include <QPushButton>
// // #include <QListWidget>
// // #include <QLabel>
// // #include <QHBoxLayout>
// // #include <QVBoxLayout>


// // class LeftList : public QWidget
// // {
// //     Q_OBJECT
// // public:
// //     explicit LeftList(QWidget *parent = nullptr);
// //     ~LeftList();

// // signals:
// //     void searchClicked();

// // private:
// //     QPushButton *searchButton;
// //     QLabel *iconLabel;
// //     QListWidget *todoList;
// // };
// ////////////////////////////////////////////////////////////////////////////////////////////////////////
// // #ifndef LEFTLIST_H
// // #define LEFTLIST_H

// // #include <QWidget>
// // #include <QListWidget>
// // #include <QPushButton>
// // #include <QLabel>
// // #include <QMap>
// // #include <QDate>

// // class LeftList : public QWidget
// // {
// //     Q_OBJECT
// // public:
// //     explicit LeftList(QWidget *parent = nullptr);

// //     // 일정 추가 / 삭제
// //     void addScheduleItem(const QDate &date, const QString &text);
// //     void removeScheduleItem(const QDate &date, const QString &text);

// //     // 검색 결과 표시 / 전체복원
// //     void showSearchResult(const QList<QPair<QDate, QString>> &results);
// //     void restoreFullList();

// // signals:
// //     void scheduleSelected(const QDate &date);   // 리스트 더블클릭 → 날짜 전송
// //     void openSearchWindow();                    // Search 버튼 클릭 시

// // private:
// //     QListWidget *todoList;
// //     QPushButton *searchButton;
// //     QLabel *iconLabel;

// //     // 전체 일정 저장
// //     QList<QPair<QDate, QString>> fullList;  // 정렬 대상

// //     void refreshListWidget();

// //     // 정렬 함수
// //     void sortItemsAscending();
// // };

// // #endif // LEFTLIST_H


// #ifndef LEFTLIST_H
// #define LEFTLIST_H

// #include <QWidget>
// #include <QListWidget>
// #include <QPushButton>
// #include <QLabel>
// #include <QList>
// #include <QPair>
// #include <QDate>

// class SearchEngine;

// class LeftList : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit LeftList(QWidget *parent = nullptr);
//     ~LeftList();

//     void setScheduleMap(const QMap<QDate, QStringList> &map);   // Calendar → LeftList
//     void refreshList();                                         // 전체 출력

// private slots:
//     void openSearchWindow();
//     void applySearchResult(const QList<QPair<QDate, QString>> &result);
//     void restoreAll();                                           // 검색 닫힘 → 전체 목록 복원

// private:
//     QLabel *iconLabel;
//     QPushButton *searchButton;
//     QListWidget *todoList;

//     SearchEngine *searchWindow = nullptr;

//     QMap<QDate, QStringList> scheduleData;    // 전체 일정
//     QList<QPair<QDate, QString>> entireList;  // 정렬된 전체 일정 리스트
// };

// #endif // LEFTLIST_H


//////////////////////////////
#ifndef LEFTLIST_H
#define LEFTLIST_H

#include <QWidget>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QDate>

class LeftList : public QWidget
{
    Q_OBJECT

public:
    explicit LeftList(QWidget *parent = nullptr);
    ~LeftList();

    // 날짜 강조 (달력 이동용)
    void highlightDate(const QDate &date);
    void updateAllTodos(const QMap<QDate, QStringList> &data);


signals:
    // 🔍 메인윈도우에게 "검색창 열어라" 요청
    void openSearch();

private:
    QLabel *iconLabel;
    QPushButton *searchButton;
    QListWidget *todoList;

    QDate highlightedDate;
};

#endif // LEFTLIST_H


