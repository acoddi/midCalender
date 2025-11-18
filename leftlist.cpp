#include "leftlist.h"

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

    searchButton = new QPushButton("검색", this);
    searchButton->setFixedHeight(30);

    // searchButton->setStyleSheet(
    //     "background-color: rgba(0,0,0,0);"
    //     "border: none;"
    //     );

    topLayout->addWidget(iconLabel);
    topLayout->addWidget(searchButton, 1);
    topLayout->addStretch();

    titleLabel = new QLabel("앞으로 할 일", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 17px; font-weight: bold; padding: 6px;");

    mainLayout->addWidget(titleLabel);

    todoList = new QListWidget(this);

    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(todoList, 1);

    connect(searchButton, &QPushButton::clicked, this, [=]() {
        emit openSearch();
    });


    //////추가한 커넥트
    connect(todoList, &QListWidget::itemClicked,
            this, &LeftList::listItemClicked);
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
    QDate today = QDate::currentDate();
    QList<QDate> keys = data.keys();
    std::sort(keys.begin(), keys.end());

    for (const QDate &date : keys) {

        if (date < today)
            continue;                       //오늘 날짜 스킵하기

        const QStringList &items = data.value(date);

        for (const QString &text : items) {

            QString line = QString("%1  |  %2")
            .arg(date.toString("yyyy - MM - dd"), text);

            QListWidgetItem *item = new QListWidgetItem(line);

            item->setData(Qt::UserRole, date);

            todoList->addItem(item);
        }
    }
}


void LeftList::listItemClicked(QListWidgetItem *item)
{
    QDate d = item->data(Qt::UserRole).toDate();
    emit listdateSelected(d);
}


