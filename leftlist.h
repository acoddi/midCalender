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
    // 메인윈도우에게 "검색창 열어라" 요청
    void openSearch();



private:
    QLabel *iconLabel;
    QPushButton *searchButton;
    QListWidget *todoList;

    QDate highlightedDate;
};

#endif // LEFTLIST_H


