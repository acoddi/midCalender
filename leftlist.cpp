// // // #include "leftlist.h"
// // // #include <QVBoxLayout>
// // // #include <QHBoxLayout>
// // // #include <QPixmap>

// // // LeftList::LeftList(QWidget *parent)
// // //     : QWidget(parent)
// // // {
// // //     QVBoxLayout *mainLayout = new QVBoxLayout(this);
// // //     mainLayout->setContentsMargins(0,0,0,0); // 전체 여백 제거
// // //     mainLayout->setSpacing(0);               // 위젯 사이 spacing 제거

// // //     // --- 상단: Search 버튼 + 아이콘 ---
// // //     QHBoxLayout *searchLayout = new QHBoxLayout();
// // //     searchLayout->setContentsMargins(5,5,5,5); // 버튼-아이콘 약간 여백
// // //     searchLayout->setSpacing(5);

// // //     iconLabel = new QLabel(this);
// // //     QPixmap icon(":/new/prefix1/search_2356562.png");
// // //     iconLabel->setPixmap(icon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
// // //     searchLayout->addWidget(iconLabel);

// // //     searchButton = new QPushButton("Search", this);
// // //     searchButton->setFixedHeight(30);
// // //     searchLayout->addWidget(searchButton, 1); // 버튼 stretch

// // //     mainLayout->addLayout(searchLayout);

// // //     // --- 하단: 할일 리스트 ---
// // //     taskList = new QListWidget(this);
// // //     taskList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
// // //     mainLayout->addWidget(taskList, 1); // stretch 1

// // //     // --- 시그널 연결 ---
// // //     connect(searchButton, &QPushButton::clicked, this, &LeftList::clickedSearch);
// // // }


// // // LeftList::~LeftList()
// // // {
// // //     // 부모가 자식 위젯 삭제하므로 별도 delete 불필요
// // // }

// // // void LeftList::clickedSearch()
// // // {
// // //     searchengine *search = new searchengine(this);
// // //     search->setAttribute(Qt::WA_DeleteOnClose);
// // //     search->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
// // //     search->setWindowModality(Qt::ApplicationModal);
// // //     search->show();
// // //     search->raise();
// // //     search->activateWindow();
// // // }



// // // #include "leftlist.h"
// // // #include "searchengine.h"

// // // LeftList::LeftList(QWidget *parent) : QWidget(parent)
// // // {
// // //     QVBoxLayout *mainLayout = new QVBoxLayout(this);
// // //     mainLayout->setContentsMargins(0,0,0,0);
// // //     mainLayout->setSpacing(0);

// // //     QHBoxLayout *topLayout = new QHBoxLayout();
// // //     topLayout->setContentsMargins(0,0,0,0);
// // //     topLayout->setSpacing(5);

// // //     iconLabel = new QLabel(this);
// // //     QPixmap pixmap(":/new/prefix1/search_2356562.png");
// // //     iconLabel->setPixmap(pixmap.scaled(24,24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

// // //     searchButton = new QPushButton("Search", this);
// // //     searchButton->setFixedHeight(30);


// // //     topLayout->addWidget(iconLabel);
// // //     topLayout->addWidget(searchButton, 1);  // stretch 1로 todoList와 비슷하게
// // //     topLayout->addStretch();

// // //     searchButton->setStyleSheet(
// // //         "background-color: rgba(0,0,0,0);"
// // //         "border: none;"
// // //         );

// // //     todoList = new QListWidget(this);

// // //     mainLayout->addLayout(topLayout);
// // //     mainLayout->addWidget(todoList, 1);

// // //     connect(searchButton, &QPushButton::clicked, this, [=](){
// // //         searchengine *search = new searchengine(this);
// // //         search->setAttribute(Qt::WA_DeleteOnClose);
// // //         search->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
// // //         search->setWindowModality(Qt::ApplicationModal);
// // //         search->show();
// // //         search->raise();
// // //         search->activateWindow();
// // //     });
// // // }

// // // LeftList::~LeftList() {}

// // ////////////////////////////////////////////////////////////////////////////////////
// // #include "leftlist.h"
// // #include "searchengine.h"
// // #include <QVBoxLayout>
// // #include <QHBoxLayout>

// // LeftList::LeftList(QWidget *parent)
// //     : QWidget(parent)
// // {
// //     QVBoxLayout *mainLayout = new QVBoxLayout(this);
// //     mainLayout->setContentsMargins(0,0,0,0);
// //     mainLayout->setSpacing(0);

// //     QHBoxLayout *topLayout = new QHBoxLayout();
// //     topLayout->setContentsMargins(0,0,0,0);
// //     topLayout->setSpacing(5);

// //     iconLabel = new QLabel(this);
// //     QPixmap pix(":/new/prefix1/search_2356562.png");
// //     iconLabel->setPixmap(pix.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

// //     searchButton = new QPushButton("Search", this);
// //     searchButton->setFixedHeight(30);
// //     searchButton->setStyleSheet("background-color: transparent; border: none;");

// //     topLayout->addWidget(iconLabel);
// //     topLayout->addWidget(searchButton, 1);
// //     topLayout->addStretch();

// //     todoList = new QListWidget(this);

// //     mainLayout->addLayout(topLayout);
// //     mainLayout->addWidget(todoList, 1);

// //     // ---------------------------
// //     // SearchEngine 창 열기 요청
// //     // ---------------------------
// //     connect(searchButton, &QPushButton::clicked, this, [=]() {
// //         emit openSearchWindow();
// //     });

// //     // ---------------------------
// //     // 리스트 더블클릭 → 날짜 전달
// //     // ---------------------------
// //     connect(todoList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
// //         QString line = item->text();          // "2025-02-13   회의"
// //         QString dateStr = line.split(" ").first();
// //         QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
// //         if(date.isValid())
// //             emit scheduleSelected(date);
// //     });
// // }

// // //
// // // 일정 추가
// // //
// // void LeftList::addScheduleItem(const QDate &date, const QString &text)
// // {
// //     if(!date.isValid() || text.trimmed().isEmpty())
// //         return;

// //     fullList.append({date, text});
// //     sortItemsAscending();
// //     refreshListWidget();
// // }

// // //
// // // 일정 삭제
// // //
// // void LeftList::removeScheduleItem(const QDate &date, const QString &text)
// // {
// //     for(int i = 0; i < fullList.size(); ++i)
// //     {
// //         if(fullList[i].first == date && fullList[i].second == text)
// //         {
// //             fullList.removeAt(i);
// //             break;
// //         }
// //     }

// //     sortItemsAscending();
// //     refreshListWidget();
// // }

// // //
// // // 정렬
// // //
// // void LeftList::sortItemsAscending()
// // {
// //     std::sort(fullList.begin(), fullList.end(),
// //               [](auto &a, auto &b){
// //                   return a.first < b.first;
// //               });
// // }

// // //
// // // QListWidget에 표시
// // //
// // void LeftList::refreshListWidget()
// // {
// //     todoList->clear();
// //     for(const auto &p : fullList)
// //         todoList->addItem(p.first.toString("yyyy-MM-dd") + "   " + p.second);
// // }

// // //
// // // 검색 결과만 표시
// // //
// // void LeftList::showSearchResult(const QList<QPair<QDate, QString>> &results)
// // {
// //     todoList->clear();
// //     for(const auto &p : results)
// //         todoList->addItem(p.first.toString("yyyy-MM-dd") + "   " + p.second);
// // }

// // //
// // // 전체 일정 복원
// // //
// // void LeftList::restoreFullList()
// // {
// //     refreshListWidget();
// // }



// #include "leftlist.h"
// #include "searchengine.h"

// LeftList::LeftList(QWidget *parent)
//     : QWidget(parent)
// {
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setContentsMargins(0,0,0,0);
//     mainLayout->setSpacing(0);

//     QHBoxLayout *topLayout = new QHBoxLayout();
//     topLayout->setSpacing(5);

//     iconLabel = new QLabel(this);
//     QPixmap pix(":/new/prefix1/search_2356562.png");
//     iconLabel->setPixmap(pix.scaled(24, 24, Qt::KeepAspectRatio));

//     searchButton = new QPushButton("Search", this);
//     searchButton->setFixedHeight(30);
//     searchButton->setStyleSheet("background: transparent; border: none;");

//     topLayout->addWidget(iconLabel);
//     topLayout->addWidget(searchButton);
//     topLayout->addStretch();

//     todoList = new QListWidget(this);

//     mainLayout->addLayout(topLayout);
//     mainLayout->addWidget(todoList, 1);

//     connect(searchButton, &QPushButton::clicked,
//             this, &LeftList::openSearchWindow);
// }

// LeftList::~LeftList() {}

// //
// // CalendarWidget → 일정 전달
// //
// void LeftList::setScheduleMap(const QMap<QDate, QStringList> &map)
// {
//     scheduleData = map;
//     entireList.clear();

//     // QMap은 자동 정렬 → 날짜 오름차순 보장
//     for (auto it = scheduleData.begin(); it != scheduleData.end(); ++it)
//     {
//         const QDate &date = it.key();
//         const QStringList &list = it.value();

//         for (const QString &s : list)
//             entireList.append({date, s});
//     }

//     refreshList();
// }

// //
// // 전체 일정 화면에 출력
// //
// void LeftList::refreshList()
// {
//     todoList->clear();
//     for (auto &p : entireList)
//     {
//         todoList->addItem(QString("%1 - %2")
//                               .arg(p.first.toString("yyyy-MM-dd"))
//                               .arg(p.second));
//     }
// }

// //
// // 검색창 열기
// //
// void LeftList::openSearchWindow()
// {
//     if(searchWindow != nullptr)
//         return;

//     searchWindow = new SearchEngine(this);
//     searchWindow->setAttribute(Qt::WA_DeleteOnClose);
//     searchWindow->setFullSchedule(entireList);

//     connect(searchWindow, &SearchEngine::searchResult,
//             this, &LeftList::applySearchResult);

//     connect(searchWindow, &SearchEngine::searchClosed,
//             this, &LeftList::restoreAll);

//     searchWindow->show();
//     searchWindow->raise();
//     searchWindow->activateWindow();
// }

// //
// // 검색 결과 적용
// //
// void LeftList::applySearchResult(const QList<QPair<QDate, QString>> &result)
// {
//     todoList->clear();

//     for (auto &p : result)
//     {
//         todoList->addItem(QString("%1 - %2")
//                               .arg(p.first.toString("yyyy-MM-dd"))
//                               .arg(p.second));
//     }
// }

// //
// // 검색창 닫힘 → 전체 일정 복원
// //
// void LeftList::restoreAll()
// {
//     searchWindow = nullptr;
//     refreshList();
// }


#include "leftlist.h"
#include "searchengine.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>

LeftList::LeftList(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setSpacing(5);

    iconLabel = new QLabel(this);
    QPixmap pix(":/new/prefix1/search_2356562.png");
    iconLabel->setPixmap(pix.scaled(24,24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    searchButton = new QPushButton("Search", this);
    searchButton->setFixedHeight(30);

    searchButton->setStyleSheet(
        "background-color: rgba(0,0,0,0);"
        "border: none;"
        );

    topLayout->addWidget(iconLabel);
    topLayout->addWidget(searchButton, 1);
    topLayout->addStretch();

    todoList = new QListWidget(this);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(todoList, 1);

    // 🔍 검색 버튼 → 메인윈도우로 신호 전달
    connect(searchButton, &QPushButton::clicked, this, [=]() {
        emit openSearch();
    });
}

LeftList::~LeftList()
{
}

void LeftList::highlightDate(const QDate &date)
{
    highlightedDate = date;
    // LeftList 내부에는 달력이 없으므로,
    // 여기서는 나중에 검색 결과 표시할 때 활용할 수 있음.
}


void LeftList::updateAllTodos(const QMap<QDate, QStringList> &data)
{
    todoList->clear();

    QList<QDate> keys = data.keys();
    std::sort(keys.begin(), keys.end());

    for(const QDate &date : keys)
    {
        const QStringList &items = data.value(date);

        for(const QString &text : items)
        {
            QString line = QString("%1  |  %2")
            .arg(date.toString("yyyy-MM-dd"))
                .arg(text);

            todoList->addItem(line);
        }
    }
}


