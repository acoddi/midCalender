#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calendarwidget.h"
#include "schedule.h"
#include "leftlist.h"
#include "searchengine.h"
#include "toolbox.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 왼쪽 탭 + ToolBox 세로 레이아웃
    left = new LeftList(this);
    left->setMinimumWidth(250);

    tools = new ToolBox(this);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->setContentsMargins(0,0,0,0);
    leftLayout->setSpacing(5);
    leftLayout->addWidget(left, 1);      // LeftList 위
    leftLayout->addWidget(tools, 0);      // ToolBox 아래

    QWidget *leftContainer = new QWidget(this);
    leftContainer->setLayout(leftLayout);

    // 중앙 캘린더 위젯
    calender = new CalendarWidget(this);

    // 오른쪽 일정 창
    schedule = new Schedule(this);
    schedule->setMinimumWidth(330);
    schedule->hide();

    // 메인 레이아웃에 추가
    mainLayout->addWidget(leftContainer, 0);  // 왼쪽
    mainLayout->addWidget(calender, 1);       // 중앙
    mainLayout->addWidget(schedule, 0);       // 오른쪽

    central->setLayout(mainLayout);


    // 달력 더블클릭 → 일정 창에 날짜 전달 후 일정창 열기
    connect(calender, &CalendarWidget::dateDoubleClicked,
            this, [=](const QDate &date) {
                currentSelectedDate = date;
                schedule->setSelectedDate(currentSelectedDate);
                schedule->show();
            });

    // 스케줄(오른쪽) 창에서 일정 추가 → 캘린더 업데이트
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

                //왼쪽 탭에도 추가
                left->updateAllTodos(calender->getAllSchedules());

            });

    // 스케줄 창에서 일정 삭제 → 캘린더에서 제거
    connect(schedule, &Schedule::todoRemoved,
            this, [=](const QString &text, const QDate &date) {
                calender->removeSchedule(date, text);

                //왼쪽 탭에도 삭제
                left->updateAllTodos(calender->getAllSchedules());

            });

    // 스케줄 창에서 일정 변경 → 캘린더/왼쪽 탭 동기화
    connect(schedule, &Schedule::todoModified,
            this, [=](const QString &oldText, const QDate &oldDate,
                      const QString &newText, const QDate &newDate) {
                if(oldDate.isValid())
                    calender->removeSchedule(oldDate, oldText);

                if(newDate.isValid())
                    calender->addSchedule(newDate, newText);

                left->updateAllTodos(calender->getAllSchedules());
            });



    // LeftList에서 검색 버튼 클릭 → 검색창 띄우기
    connect(left, &LeftList::openSearch,
            this, &MainWindow::openSearchWindow);

    // 앱 시작 시 JSON 파일에서 할 일 목록 로드
    loadTodosFromJson();

    // 리스트에서 강조 이동
    connect(left, &LeftList::listdateSelected,
            this, &MainWindow::highlightFromlist);

    // 전체 초기화
    connect(tools, &ToolBox::clearAllRequested, this, [=]() {
        schedule->todosByDate.clear();
        calender->clearAllSchedules();   // 필요: CalendarWidget에 구현해야 함
        left->updateAllTodos({});
    });

    // 월 전체 초기화
    connect(tools, &ToolBox::clearMonthRequested, this, [=]() {

        int y = calender->currentYear();
        int m = calender->currentMonth();

        // 스케줄 자료에서 해당 월 날짜만 제거
        QList<QDate> keys = schedule->todosByDate.keys();
        for (const QDate &d : keys) {
            if (d.year() == y && d.month() == m)
                schedule->todosByDate.remove(d);
        }

        // 캘린더와 LeftList 리프레시
        calender->clearMonth(y, m);  // 일단 여기 함수는 내가 아래 만들어줄게
        left->updateAllTodos(schedule->todosByDate);

    });


    // 종료
    connect(tools, &ToolBox::exitRequested, this, [=]() {
        close();
    });


}


MainWindow::~MainWindow()
{
    // 앱 종료 시 할 일 목록을 JSON 파일로 저장
    saveTodosToJson();

    delete ui;
}


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

// 검색 실행
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


// 검색 결과 클릭 해당 날짜 강조
void MainWindow::highlightFromSearch(const QDate &date)
{
    calender->setHighlight(date);  // CalendarWidget에 구현된 함수

}

void MainWindow::highlightFromlist(const QDate &date){
    calender->setHighlight(date);

}
// JSON 파일에서 할 일 목록 로드
void MainWindow::loadTodosFromJson()
{
    QString filename = getTodosJsonPath();

    if(schedule->loadFromJson(filename))
    {
        // 로드 성공 시 CalendarWidget과 LeftList에 동기화
        QMap<QDate, QStringList> todos = schedule->todosByDate;

        // CalendarWidget에 일정 추가
        for(auto it = todos.begin(); it != todos.end(); ++it)
        {
            QDate date = it.key();
            QStringList todoList = it.value();

            for(const QString &todo : todoList)
            {
                calender->addSchedule(date, todo);
            }
        }

        // LeftList 업데이트
        left->updateAllTodos(calender->getAllSchedules());
    }
}

// 할 일 목록을 JSON 파일로 저장
void MainWindow::saveTodosToJson()
{
    QString filename = getTodosJsonPath();
    schedule->saveToJson(filename);
}

// JSON 파일 경로 반환
QString MainWindow::getTodosJsonPath() const
{
    // 사용자 데이터 디렉토리 가져오기
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // 디렉토리가 없으면 생성
    QDir dir;
    if(!dir.exists(dataDir))
    {
        dir.mkpath(dataDir);
    }

    // JSON 파일 경로 반환
    return dataDir + "/todos.json";
}


