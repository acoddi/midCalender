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

        // 내부적으로 날짜별로 저장
        addTodoItem(todoText, start, end);
        // 캘린더 위젯에도 전달
        emit todoAdded(todoText, start, end);

        // 입력 초기화
        todoEdit->clear();
        startDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
        endDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());
    });

    connect(deleteBtn, &QPushButton::clicked, this, &Schedule::hide);
}

void Schedule::addTodoItem(const QString &text, const QDate &start, const QDate &end)
{
    QString trimmed = text.trimmed();
    if(!todoListWidget || trimmed.isEmpty())
        return;

    QDate from = start;
    QDate to = end;
    if(!from.isValid() || !to.isValid())
        from = to = selectedDate.isValid() ? selectedDate : QDate::currentDate();
    if(to < from)
        std::swap(from, to);

    // 선택된 기간의 모든 날짜에 이 할 일을 저장
    for(QDate day = from; day <= to; day = day.addDays(1)) {
        todosByDate[day].append(trimmed);
    }

    // 현재 선택된 날짜 리스트를 다시 그림
    refreshTodoList();
}

void Schedule::setSelectedDate(const QDate &date)
{
    selectedDate = date.isValid() ? date : QDate::currentDate();  // 선택된 날짜를 저장
    if(startDateEdit)
        startDateEdit->setDate(selectedDate);
    if(endDateEdit)
        endDateEdit->setDate(selectedDate);

    // 날짜 변경 시 해당 날짜의 할 일 목록 표시
    refreshTodoList();
}

void Schedule::refreshTodoList()
{
    if(!todoListWidget)
        return;

    todoListWidget->clear();

    if(!selectedDate.isValid())
        return;

    const QStringList items = todosByDate.value(selectedDate);

    for(int i = 0; i < items.size(); ++i) {
        const QString &text = items.at(i);

        // 리스트 아이템(데이터용)
        QListWidgetItem *item = new QListWidgetItem(todoListWidget);
        item->setSizeHint(QSize(0, 30));

        // 아이템에 표시될 위젯 (텍스트 + 삭제 버튼)
        QWidget *rowWidget = new QWidget(todoListWidget);
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(2, 2, 2, 2);
        rowLayout->setSpacing(4);

        QLabel *label = new QLabel(text, rowWidget);
        QPushButton *deleteBtn = new QPushButton("삭제", rowWidget);
        deleteBtn->setFixedWidth(40);

        rowLayout->addWidget(label);
        rowLayout->addStretch();
        rowLayout->addWidget(deleteBtn);

        todoListWidget->setItemWidget(item, rowWidget);

        // 삭제 버튼 클릭 시 해당 인덱스의 할 일 삭제
        connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            if(!selectedDate.isValid())
                return;

            QStringList &listRef = todosByDate[selectedDate];
            int row = todoListWidget->row(item);
            if(row >= 0 && row < listRef.size()) {
                const QString removedText = listRef.at(row);
                listRef.removeAt(row);
                // 캘린더에도 삭제 알림
                emit todoRemoved(removedText, selectedDate);
            }

            refreshTodoList();
        });
    }
}
