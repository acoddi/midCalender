// #include "leftlist.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QPixmap>

// LeftList::LeftList(QWidget *parent)
//     : QWidget(parent)
// {
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->setContentsMargins(0,0,0,0); // 전체 여백 제거
//     mainLayout->setSpacing(0);               // 위젯 사이 spacing 제거

//     // --- 상단: Search 버튼 + 아이콘 ---
//     QHBoxLayout *searchLayout = new QHBoxLayout();
//     searchLayout->setContentsMargins(5,5,5,5); // 버튼-아이콘 약간 여백
//     searchLayout->setSpacing(5);

//     iconLabel = new QLabel(this);
//     QPixmap icon(":/new/prefix1/search_2356562.png");
//     iconLabel->setPixmap(icon.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//     searchLayout->addWidget(iconLabel);

//     searchButton = new QPushButton("Search", this);
//     searchButton->setFixedHeight(30);
//     searchLayout->addWidget(searchButton, 1); // 버튼 stretch

//     mainLayout->addLayout(searchLayout);

//     // --- 하단: 할일 리스트 ---
//     taskList = new QListWidget(this);
//     taskList->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//     mainLayout->addWidget(taskList, 1); // stretch 1

//     // --- 시그널 연결 ---
//     connect(searchButton, &QPushButton::clicked, this, &LeftList::clickedSearch);
// }


// LeftList::~LeftList()
// {
//     // 부모가 자식 위젯 삭제하므로 별도 delete 불필요
// }

// void LeftList::clickedSearch()
// {
//     searchengine *search = new searchengine(this);
//     search->setAttribute(Qt::WA_DeleteOnClose);
//     search->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
//     search->setWindowModality(Qt::ApplicationModal);
//     search->show();
//     search->raise();
//     search->activateWindow();
// }



#include "leftlist.h"
#include "searchengine.h"

LeftList::LeftList(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0,0,0,0);
    topLayout->setSpacing(5);

    iconLabel = new QLabel(this);
    QPixmap pixmap(":/new/prefix1/search_2356562.png");
    iconLabel->setPixmap(pixmap.scaled(24,24, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    searchButton = new QPushButton("Search", this);
    searchButton->setFixedHeight(30);


    topLayout->addWidget(iconLabel);
    topLayout->addWidget(searchButton, 1);  // stretch 1로 todoList와 비슷하게
    topLayout->addStretch();

    searchButton->setStyleSheet(
        "background-color: rgba(0,0,0,0);"
        "border: none;"
        );

    todoList = new QListWidget(this);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(todoList, 1);

    connect(searchButton, &QPushButton::clicked, this, [=](){
        searchengine *search = new searchengine(this);
        search->setAttribute(Qt::WA_DeleteOnClose);
        search->setWindowFlags(Qt::Dialog | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
        search->setWindowModality(Qt::ApplicationModal);
        search->show();
        search->raise();
        search->activateWindow();
    });
}

LeftList::~LeftList() {}
