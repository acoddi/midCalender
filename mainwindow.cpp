// #include "mainwindow.h"
// #include "./ui_mainwindow.h"
// #include "CalendarWidget.h"
// #include "schedule.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include "searchengine.h"
// #include "leftlist.h"
// #include <QDebug>
// #include <qdatetime.h>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     ui->setupUi(this);
//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);
//     QHBoxLayout *mainLayout = new QHBoxLayout(central);
//     mainLayout->setContentsMargins(10,10,10,10);

//     left = new LeftList(this);
//     left->setMinimumWidth(250);
//     mainLayout->addWidget(left, 0);

//     calender = new CalendarWidget(this);
//     mainLayout->addWidget(calender, 1);

//     schedule = new Schedule(this);
//     schedule->setMinimumWidth(250);
//     schedule->hide();
//     mainLayout->addWidget(schedule, 0);


//     connect(calender, &CalendarWidget::dateDoubleClicked, this, [=](const QDate &date){
//         schedule->show();
//         //qDebug() << "Selected date:" << date;
//         qDebug() << "Selected date:" << date.toString("yyyy-MM-dd");
//     });

// }

// MainWindow::~MainWindow()
// {
//     delete ui;
// }


// #include "mainwindow.h"
// #include <QHBoxLayout>
// #include <QDebug>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
// {
//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);
//     QHBoxLayout *mainLayout = new QHBoxLayout(central);
//     mainLayout->setContentsMargins(0,0,0,0);
//     mainLayout->setSpacing(0);

//     left = new LeftList(this);
//     left->setMinimumWidth(250);
//     mainLayout->addWidget(left,0);

//     calender = new CalendarWidget(this);
//     mainLayout->addWidget(calender,1);

//     schedule = new Schedule(this);
//     schedule->setMinimumWidth(250);
//     schedule->hide();
//     mainLayout->addWidget(schedule,0);

//     connect(calender, &CalendarWidget::dateDoubleClicked, this, [=](const QDate &date){
//         schedule->show();
//         qDebug() << "Selected date:" << date.toString("yyyy-MM-dd");
//     });
// }

// MainWindow::~MainWindow(){}

////////////////////////////////////////////////////////////////////////////////////////////////////
// #include "mainwindow.h"
// #include "./ui_mainwindow.h"
// #include "calendarwidget.h"
// #include "schedule.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <algorithm>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent)
//     , ui(new Ui::MainWindow)
// {
//     ui->setupUi(this);
//     QWidget *central = new QWidget(this);
//     setCentralWidget(central);
//     QHBoxLayout *mainLayout = new QHBoxLayout(central);
//     mainLayout->setContentsMargins(0, 0, 0, 0);
//     mainLayout->setSpacing(0);

//     left = new LeftList(this);
//     left->setMinimumWidth(250);
//     mainLayout->addWidget(left,0);


//     calender = new CalendarWidget(this);
//     mainLayout->addWidget(calender, 1);

//     schedule = new Schedule(this);
//     schedule->setMinimumWidth(250);
//     schedule->hide();
//     mainLayout->addWidget(schedule, 0);



//     connect(calender, &CalendarWidget::dateDoubleClicked, this, [=](const QDate &date) {
//         currentSelectedDate = date;
//         schedule->setSelectedDate(currentSelectedDate);
//         schedule->show();
//     });

//     connect(schedule, &Schedule::todoAdded, this, [=](const QString &text, const QDate &start, const QDate &end) {
//         if (!start.isValid() || !end.isValid())
//             return;

//         QDate from = start;
//         QDate to = end;
//         if(to < from)
//             std::swap(from, to);

//         for(QDate day = from; day <= to; day = day.addDays(1))
//         {
//             calender->addSchedule(day, text);
//         }
//     });

//     // 스케줄 창에서 삭제된 할 일은 캘린더에서도 삭제
//     connect(schedule, &Schedule::todoRemoved, this, [=](const QString &text, const QDate &date) {
//         calender->removeSchedule(date, text);
//     });








// }

// MainWindow::~MainWindow()
// {
//     delete ui;
// }
////////////////////////////////////////////////////////
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calendarwidget.h"
#include "schedule.h"
#include "leftlist.h"
#include "searchengine.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // -----------------------------
    //  LEFT LIST
    // -----------------------------
    left = new LeftList(this);
    left->setMinimumWidth(250);
    mainLayout->addWidget(left, 0);


    // -----------------------------
    //  CALENDAR WIDGET
    // -----------------------------
    calender = new CalendarWidget(this);
    mainLayout->addWidget(calender, 1);


    // -----------------------------
    //  SCHEDULE WIDGET (RIGHT)
    // -----------------------------
    schedule = new Schedule(this);
    schedule->setMinimumWidth(250);
    schedule->hide();
    mainLayout->addWidget(schedule, 0);


    // =============================
    //    SIGNAL CONNECTIONS
    // =============================

    // 🔵 1. 달력 더블클릭 → 일정 창에 날짜 전달 후 일정창 열기
    connect(calender, &CalendarWidget::dateDoubleClicked,
            this, [=](const QDate &date) {
                currentSelectedDate = date;
                schedule->setSelectedDate(currentSelectedDate);
                schedule->show();
            });

    // 🔵 2. 스케줄 창에서 일정 추가 → 캘린더 업데이트
    connect(schedule, &Schedule::todoAdded,
            this, [=](const QString &text, const QDate &start, const QDate &end) {

                if (!start.isValid() || !end.isValid())
                    return;

                QDate from = start;
                QDate to = end;

                if (to < from)
                    std::swap(from, to);

                for (QDate day = from; day <= to; day = day.addDays(1))
                    calender->addSchedule(day, text);

                //추가
                left->updateAllTodos(calender->getAllSchedules());

            });

    // 🔵 3. 스케줄 창에서 일정 삭제 → 캘린더에서 제거
    connect(schedule, &Schedule::todoRemoved,
            this, [=](const QString &text, const QDate &date) {
                calender->removeSchedule(date, text);
                left->updateAllTodos(calender->getAllSchedules());

            });


    // =====================================================
    //  🔎 검색엔진 연결 (LEFT LIST → 검색창)
    // =====================================================

    // LeftList에서 검색 버튼 클릭 → 검색창 띄우기
    connect(left, &LeftList::openSearch,
            this, &MainWindow::openSearchWindow);


}

MainWindow::~MainWindow()
{
    delete ui;
}


// =====================================================
//  🔎  검색창 열기
// =====================================================
void MainWindow::openSearchWindow()
{
    // 새 검색 엔진 창 생성
    searchengine *engine = new searchengine(this);
    engine->setAttribute(Qt::WA_DeleteOnClose);
    engine->show();

    activeSearchEngine = engine;

    // 🔵 A. 검색 엔진 → 검색 요청 signal
    connect(engine, &searchengine::searchRequested,
            this, &MainWindow::performSearch);

    // 🔵 B. 검색 결과 클릭 → 달력 날짜 강조 이동
    connect(engine, &searchengine::dateSelected,
            this, &MainWindow::highlightFromSearch);
}


// =====================================================
//  🔍 검색 수행 (일정 전체 검색)
// =====================================================
void MainWindow::performSearch(const QString &keyword)
{
    QList<QPair<QDate, QString>> results;

    // Schedule 내부 일정 데이터 전체 검색 (todosByDate 사용)
    for (auto it = schedule->todosByDate.begin();
         it != schedule->todosByDate.end(); ++it)
    {
        QDate date = it.key();
        QStringList list = it.value();

        for (const QString &text : list)
        {
            if (text.contains(keyword, Qt::CaseInsensitive))
                results.append({date, text});
        }
    }

    // 🔵 날짜 오름차순 정렬
    std::sort(results.begin(), results.end(),
              [](const QPair<QDate, QString> &a,
                 const QPair<QDate, QString> &b)
              {
                  return a.first < b.first;
              });

    // 🔵 검색 결과 전달
    if (activeSearchEngine)
        activeSearchEngine->showSearchResults(results);
}


// =====================================================
//  📌 검색 결과 클릭 → 해당 날짜로 이동 + 강조
// =====================================================
void MainWindow::highlightFromSearch(const QDate &date)
{
    calender->setHighlight(date);  // CalendarWidget에 구현된 함수
}


