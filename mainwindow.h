#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>

#include "calendarwidget.h"
#include "schedule.h"
#include "leftlist.h"
#include "searchengine.h"
#include "toolbox.h"

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
    // 검색 요청 처리
    void performSearch(const QString &keyword);

    // 검색 결과 더블클릭 → 달력 강조 이동
    void highlightFromSearch(const QDate &date);

    // 검색창 열기
    void openSearchWindow();
    //리스트에서 강조
    void highlightFromlist(const QDate &date);


private:
    Ui::MainWindow *ui;

    CalendarWidget *calender;   // 중앙 달력
    Schedule *schedule;         // 오른쪽 일정 창
    LeftList *left;             // 왼쪽 리스트
    QDate currentSelectedDate;  // 현재 더블클릭한 날짜
    ToolBox *tools;

    searchengine *activeSearchEngine = nullptr; // 현재 열린 검색창

    // JSON 저장/로드 함수
    void loadTodosFromJson();   // JSON 파일에서 할 일 목록 로드
    void saveTodosToJson();     // 할 일 목록을 JSON 파일로 저장
    QString getTodosJsonPath() const;  // JSON 파일 경로 반환
};

#endif // MAINWINDOW_H


