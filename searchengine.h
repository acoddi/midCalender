// #ifndef SEARCHENGINE_H
// #define SEARCHENGINE_H

// #include <QWidget>

// namespace Ui {
// class searchengine;
// }

// class searchengine : public QWidget
// {
//     Q_OBJECT

// public:
//     explicit searchengine(QWidget *parent = nullptr);
//     ~searchengine();

// public slots:
//     void textChanged(QString str);
//     void searchButton();


// private:
//     Ui::searchengine *ui;
// };

// #endif // SEARCHENGINE_H

//////////////////////////////////////////////////////////////////

#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QWidget>
#include <QListWidget>

namespace Ui {
class searchengine;
}

class searchengine : public QWidget
{
    Q_OBJECT

public:
    explicit searchengine(QWidget *parent = nullptr);
    ~searchengine();

signals:
    void searchRequested(const QString &keyword);   // 🔥 검색 요청
    void dateSelected(const QDate &date);           // 🔥 검색 결과 중 날짜 클릭됨

public slots:
    void showSearchResults(const QList<QPair<QDate, QString>> &results);  // 🔥 MainWindow → 결과 표시

private slots:
    void onSearchButtonClicked();
    void onResultItemClicked(QListWidgetItem *item);

private:
    Ui::searchengine *ui;
    QListWidget *resultList;   // 검색 결과 리스트
};

#endif // SEARCHENGINE_H

