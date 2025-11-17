#include "schedule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDateEdit>
#include <QTextEdit>
#include <QTabWidget>
#include <QDate>

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 탭 위젯 생성
    todoWidget = new QTabWidget(this);
    mainLayout->addWidget(todoWidget);

    // 기본 탭 하나 생성
    QWidget *tab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);

    // 할 일 텍스트 입력
    QLabel *todoLabel = new QLabel("할 일 내용:");
    QTextEdit *todoEdit = new QTextEdit();
    todoEdit->setPlaceholderText("할 일을 입력하세요");

    // 기간 입력
    QHBoxLayout *dateLayout = new QHBoxLayout();
    QLabel *fromLabel = new QLabel("시작일:");
    QDateEdit *startDateEdit = new QDateEdit(QDate::currentDate());
    startDateEdit->setCalendarPopup(true);

    QLabel *toLabel = new QLabel("종료일:");
    QDateEdit *endDateEdit = new QDateEdit(QDate::currentDate());
    endDateEdit->setCalendarPopup(true);

    dateLayout->addWidget(fromLabel);
    dateLayout->addWidget(startDateEdit);
    dateLayout->addWidget(toLabel);
    dateLayout->addWidget(endDateEdit);

    // 추가 버튼
    QPushButton *addBtn = new QPushButton("추가");
    QPushButton *deleteBtn = new QPushButton("취소");

    tabLayout->addWidget(todoLabel);
    tabLayout->addWidget(todoEdit);
    tabLayout->addLayout(dateLayout);

    tabLayout->addWidget(addBtn);
    tabLayout->addWidget(deleteBtn);

    todoWidget->addTab(tab, "새 할 일");

    // 버튼 클릭 시 시그널 연결 (달력에 반영하는 기능은 MainWindow에서 구현)
    connect(addBtn, &QPushButton::clicked, [=]() {
        QString todoText = todoEdit->toPlainText();
        QDate start = startDateEdit->date();
        QDate end = endDateEdit->date();

        if(todoText.isEmpty()) return;

        emit todoAdded(todoText, start, end);

        // 입력 초기화
        todoEdit->clear();
        startDateEdit->setDate(QDate::currentDate());
        endDateEdit->setDate(QDate::currentDate());
    });

    connect(deleteBtn, &QPushButton::clicked, this, &Schedule::hide);
}
