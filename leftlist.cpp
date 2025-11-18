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


