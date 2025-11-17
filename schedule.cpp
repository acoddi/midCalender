#include "schedule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QDateEdit>
#include <QTextEdit>
#include <QTabWidget>
#include <QDate>
#include <QListWidget>
#include <algorithm>

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
{
    selectedDate = QDate::currentDate();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    todoWidget = new QTabWidget(this);
    mainLayout->addWidget(todoWidget);

    QWidget *tab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);

    // 할 일 텍스트 입력
    QLabel *todoLabel = new QLabel("할 일 내용:", this);
    todoEdit = new QTextEdit(this);
    todoEdit->setPlaceholderText("할 일을 입력하세요");
    todoEdit->setFixedHeight(60);

    //할 일 리스트 목록
    QLabel *todoLabelWill = new QLabel("오늘의 할일", this);
    todoListWidget = new QListWidget(this);

    // 기간 입력
    QHBoxLayout *dateLayout = new QHBoxLayout();
    fromLabel = new QLabel("시작일:", this);
    startDateEdit = new QDateEdit(selectedDate, this);
    startDateEdit->setCalendarPopup(true);

    QLabel *toLabel = new QLabel("종료일:", this);
    endDateEdit = new QDateEdit(selectedDate, this);
    endDateEdit->setCalendarPopup(true);

    dateLayout->addWidget(fromLabel);
    dateLayout->addWidget(startDateEdit);
    dateLayout->addWidget(toLabel);
    dateLayout->addWidget(endDateEdit);

    // 버튼 구현
    QPushButton *addBtn = new QPushButton("추가", this);
    QPushButton *deleteBtn = new QPushButton("취소", this);

    tabLayout->addWidget(todoLabel);
    tabLayout->addWidget(todoEdit);
    tabLayout->addWidget(todoLabelWill);
    tabLayout->addWidget(todoListWidget);
    tabLayout->addLayout(dateLayout);
    tabLayout->addWidget(addBtn);
    tabLayout->addWidget(deleteBtn);

    todoWidget->addTab(tab, "새 할 일");

    // 버튼 클릭 시 시그널 연결
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        QString todoText = todoEdit->toPlainText().trimmed();
        if(todoText.isEmpty()) return;

        QDate start = startDateEdit->date();
        QDate end = endDateEdit->date();
        if(end < start)
            std::swap(start, end);

        addTodoItem(todoText);
        emit todoAdded(todoText, start, end);

        // 입력 초기화
        todoEdit->clear();
        startDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
        endDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
    });

    connect(deleteBtn, &QPushButton::clicked, this, &Schedule::hide);
}

void Schedule::addTodoItem(const QString &text)
{
    if(!todoListWidget || text.trimmed().isEmpty())
        return;

    todoListWidget->addItem(text.trimmed());
}

void Schedule::setSelectedDate(const QDate &date)
{
    selectedDate = date.isValid() ? date : QDate::currentDate();  // 선택된 날짜를 저장
    if(startDateEdit)
        startDateEdit->setDate(selectedDate);
    if(endDateEdit)
        endDateEdit->setDate(selectedDate);
}
