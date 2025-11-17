#include "calendarwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QTextEdit>
#include <QHeaderView>
#include <QDebug>

CalendarWidget::CalendarWidget(QWidget *parent)
    : QWidget(parent)
{
    QDate today = QDate::currentDate();
    year = today.year();
    month = today.month();

    setupUI();
    populateCalendar();
}

void CalendarWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout();

    prevBtn = new QPushButton("이전 달", this);
    nextBtn = new QPushButton("다음 달", this);
    monthLabel = new QLabel(QDate(year, month, 1).toString("yyyy-MM"), this);

    prevBtn->setFixedWidth(60);
    nextBtn->setFixedWidth(60);

    topLayout->addWidget(prevBtn);
    topLayout->addWidget(monthLabel, 1, Qt::AlignCenter);
    topLayout->addWidget(nextBtn);

    layout->addLayout(topLayout);

    table = new QTableWidget(6, 7, this);

    QStringList weekDays = {"일", "월", "화", "수", "목", "금", "토"};
    table->setHorizontalHeaderLabels(weekDays);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    // 행 높이 충분히
    for(int i = 0; i < 6; ++i)
        table->setRowHeight(i, 80);

    layout->addWidget(table);

    // 시그널 연결
    connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
    connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
    connect(table, &QTableWidget::cellDoubleClicked, this, &CalendarWidget::onCellDoubleClicked);
}

void CalendarWidget::populateCalendar()
{
    table->clearContents();

    QDate firstDay(year, month, 1);
    int startColumn = firstDay.dayOfWeek() % 7; // 일요일=0
    int daysInMonth = firstDay.daysInMonth();

    int day = 1;

    for(int row = 0; row < 6; ++row)
    {
        for(int col = 0; col < 7; ++col)
        {
            if(row == 0 && col < startColumn)
            {
                table->setCellWidget(row, col, nullptr);
                continue;
            }

            if(day <= daysInMonth)
            {
                QWidget *cellWidget = new QWidget();
                QVBoxLayout *cellLayout = new QVBoxLayout(cellWidget);
                cellLayout->setContentsMargins(2,2,2,2);
                cellLayout->setSpacing(2);

                QLabel *dateLabel = new QLabel(QString::number(day));
                dateLabel->setStyleSheet("font-weight:bold;");
                cellLayout->addWidget(dateLabel);

                QLabel *scheduleContent = new QLabel();
                scheduleContent->setFixedHeight(50);
                cellLayout->addWidget(scheduleContent);

                table->setCellWidget(row, col, cellWidget);

                day++;
            }
            else
            {
                table->setCellWidget(row, col, nullptr);
            }
        }
    }
}

void CalendarWidget::showPrevMonth()
{
    month--;
    if(month < 1)
    {
        month = 12;
        year--;
    }
    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar();
}

void CalendarWidget::showNextMonth()
{
    month++;
    if(month > 12)
    {
        month = 1;
        year++;
    }
    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar();
}

void CalendarWidget::onCellDoubleClicked(int row, int column)
{
    QDate clickedDate(year, month, 1);
    int startColumn = QDate(year, month, 1).dayOfWeek() % 7;
    int day = row*7 + column - startColumn + 1;

    if(day < 1 || day > QDate(year, month, 1).daysInMonth())
        return;

    clickedDate.setDate(year, month, day);
    qDebug()<<"clicked";

    emit dateDoubleClicked(clickedDate); // MainWindow에서 연결
}

