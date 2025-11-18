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
public slots:
    void listItemClicked(QListWidgetItem *item);



signals:
    // 메인윈도우에게 "검색창 열어라" 요청
    void openSearch();
    void listdateSelected(const QDate &date);

private:
    QLabel *iconLabel;
    QPushButton *searchButton;
    QListWidget *todoList;
    QLabel *titleLabel;

    QDate highlightedDate;
};

#endif // LEFTLIST_H


