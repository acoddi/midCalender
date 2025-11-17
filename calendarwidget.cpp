#include "calendarwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QHeaderView>
#include <QDebug>
#include <QFrame>

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

    prevBtn = new QPushButton("<", this);
    nextBtn = new QPushButton(">", this);
    monthLabel = new QLabel(QDate(year, month, 1).toString("yyyy-MM"), this);

    prevBtn->setFixedWidth(70);
    nextBtn->setFixedWidth(70);

    topLayout->addWidget(prevBtn);
    topLayout->addWidget(monthLabel, 1, Qt::AlignCenter);
    topLayout->addWidget(nextBtn);

    layout->addLayout(topLayout);

    table = new QTableWidget(6, 7, this);

    QStringList weekDays = {"일", "월", "화", "수", "목", "금", "토"};
    table->setHorizontalHeaderLabels(weekDays);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //행 높이 설정
    for(int i = 0; i < 6; ++i)
        table->setRowHeight(i, 80);

    //레이아웃에 테이블 추가
    layout->addWidget(table);

    // 시그널 연결
    connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
    connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
    connect(table, &QTableWidget::cellDoubleClicked, this, &CalendarWidget::onCellDoubleClicked);

}

void CalendarWidget::populateCalendar()
{
    //테이블 초기화
    table->clearContents();

    //테이블 날짜 입력
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
                QWidget *cellWidget = new QWidget(this);
                table->setCellWidget(row, col, cellWidget);

                QVBoxLayout *cellLayout = new QVBoxLayout(cellWidget);
                cellLayout->setContentsMargins(2,2,2,2);
                cellLayout->setSpacing(2);

                QLabel *dateLabel = new QLabel(QString::number(day), cellWidget);
                dateLabel->setStyleSheet("font-weight:bold;");
                cellLayout->addWidget(dateLabel);

                QFrame *line = new QFrame(cellWidget);
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                cellLayout->addWidget(line);

                QLabel *scheduleContent = new QLabel(cellWidget);
                scheduleContent->setMinimumHeight(100);
                scheduleContent->setWordWrap(true);
                scheduleContent->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                scheduleContent->setObjectName("scheduleContent");
                cellLayout->addWidget(scheduleContent);

                QDate cellDate(year, month, day);
                updateCellSchedules(cellDate);

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
    qDebug()<<"clicked"<<clickedDate;

    emit dateDoubleClicked(clickedDate);
}

void CalendarWidget::addSchedule(const QDate &date, const QString &text)
{
    QString trimmed = text.trimmed();
    if(!date.isValid() || trimmed.isEmpty())
        return;

    scheduleData[date].append(trimmed);

    if (date.year() == year && date.month() == month)
    {
        updateCellSchedules(date);
    }
}

void CalendarWidget::removeSchedule(const QDate &date, const QString &text)
{
    if(!date.isValid())
        return;

    if(!scheduleData.contains(date))
        return;

    QStringList &list = scheduleData[date];
    QString trimmed = text.trimmed();

    // 동일한 텍스트의 첫 번째 항목만 제거
    int idx = list.indexOf(trimmed);
    if(idx != -1)
        list.removeAt(idx);

    if(list.isEmpty())
        scheduleData.remove(date);

    if (date.year() == year && date.month() == month)
    {
        updateCellSchedules(date);
    }
}

void CalendarWidget::updateCellSchedules(const QDate &date)
{
    if(date.year() != year || date.month() != month)
        return;

    QDate firstDay(year, month, 1);
    int startColumn = firstDay.dayOfWeek() % 7;
    int day = date.day();
    int index = day + startColumn - 1;
    int row = index / 7;
    int col = index % 7;

    QWidget *cellWidget = table->cellWidget(row, col);
    if(!cellWidget)
        return;

    QLabel *scheduleContent = cellWidget->findChild<QLabel*>("scheduleContent");
    if(!scheduleContent)
        return;

    const QStringList entries = scheduleData.value(date);
    if(entries.isEmpty())
    {
        scheduleContent->clear();
        return;
    }

    QStringList visibleLines;
    // 화면에 직접 보여줄 최대 일정 개수 (나머지는 +N 으로 표시)
    const int maxVisible = 2;
    const int visibleCount = qMin(entries.size(), maxVisible);
    for(int i = 0; i < visibleCount; ++i)
    {
        visibleLines << QStringLiteral("• %1").arg(entries.at(i));
    }

    if(entries.size() > maxVisible)
    {
        visibleLines << QStringLiteral("+%1 더보기").arg(entries.size() - maxVisible);
    }

    scheduleContent->setText(visibleLines.join("\n"));
}



