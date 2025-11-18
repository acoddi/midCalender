// #include "calendarwidget.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QDate>
// #include <QTextEdit>
// #include <QHeaderView>
// #include <QDebug>

// CalendarWidget::CalendarWidget(QWidget *parent)
//     : QWidget(parent)
// {
//     QDate today = QDate::currentDate();
//     year = today.year();
//     month = today.month();

//     setupUI();
//     populateCalendar();
// }

// void CalendarWidget::setupUI()
// {
//     QVBoxLayout *layout = new QVBoxLayout(this);
//     QHBoxLayout *topLayout = new QHBoxLayout();

//     prevBtn = new QPushButton("이전 달", this);
//     nextBtn = new QPushButton("다음 달", this);
//     monthLabel = new QLabel(QDate(year, month, 1).toString("yyyy-MM"), this);

//     prevBtn->setFixedWidth(60);
//     nextBtn->setFixedWidth(60);

//     topLayout->addWidget(prevBtn);
//     topLayout->addWidget(monthLabel, 1, Qt::AlignCenter);
//     topLayout->addWidget(nextBtn);

//     layout->addLayout(topLayout);

//     table = new QTableWidget(6, 7, this);

//     QStringList weekDays = {"일", "월", "화", "수", "목", "금", "토"};
//     table->setHorizontalHeaderLabels(weekDays);
//     table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//     table->verticalHeader()->setVisible(false);

//     // 행 높이 충분히
//     for(int i = 0; i < 6; ++i)
//         table->setRowHeight(i, 80);

//     layout->addWidget(table, 0, Qt::AlignCenter);


//     // 시그널 연결
//     connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
//     connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
//     connect(table, &QTableWidget::cellDoubleClicked, this, &CalendarWidget::onCellDoubleClicked);
// }

// void CalendarWidget::populateCalendar()
// {
//     table->clearContents();

//     QDate firstDay(year, month, 1);
//     int startColumn = firstDay.dayOfWeek() % 7; // 일요일=0
//     int daysInMonth = firstDay.daysInMonth();

//     int day = 1;

//     for(int row = 0; row < 6; ++row)
//     {
//         for(int col = 0; col < 7; ++col)
//         {
//             if(row == 0 && col < startColumn)
//             {
//                 table->setCellWidget(row, col, nullptr);
//                 continue;
//             }

//             if(day <= daysInMonth)
//             {
//                 QWidget *cellWidget = new QWidget();
//                 QVBoxLayout *cellLayout = new QVBoxLayout(cellWidget);
//                 cellLayout->setContentsMargins(2,2,2,2);
//                 cellLayout->setSpacing(2);

//                 QLabel *dateLabel = new QLabel(QString::number(day));
//                 dateLabel->setStyleSheet("font-weight:bold;");
//                 cellLayout->addWidget(dateLabel);

//                 QLabel *scheduleContent = new QLabel();
//                 scheduleContent->setFixedHeight(50);
//                 cellLayout->addWidget(scheduleContent);

//                 table->setCellWidget(row, col, cellWidget);

//                 day++;
//             }
//             else
//             {
//                 table->setCellWidget(row, col, nullptr);
//             }
//         }
//     }
// }

// void CalendarWidget::showPrevMonth()
// {
//     month--;
//     if(month < 1)
//     {
//         month = 12;
//         year--;
//     }
//     monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
//     populateCalendar();
// }

// void CalendarWidget::showNextMonth()
// {
//     month++;
//     if(month > 12)
//     {
//         month = 1;
//         year++;
//     }
//     monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
//     populateCalendar();
// }

// void CalendarWidget::onCellDoubleClicked(int row, int column)
// {
//     QDate clickedDate(year, month, 1);
//     int startColumn = QDate(year, month, 1).dayOfWeek() % 7;
//     int day = row*7 + column - startColumn + 1;

//     if(day < 1 || day > QDate(year, month, 1).daysInMonth())
//         return;

//     clickedDate.setDate(year, month, day);
//     qDebug()<<"clicked";

//     emit dateDoubleClicked(clickedDate); // MainWindow에서 연결
// }
//
//
// void CalendarWidget::resizeEvent(QResizeEvent *event)
// {
//     QWidget::resizeEvent(event);

//     if (!table) return;

//     // 현재 CalendarWidget 전체 크기
//     int w = width();
//     int h = height();

//     // 정사각형 한 변 길이 = 작은 쪽
//     int side = qMin(w, h - 50);
//     // -50 은 상단 버튼 영역(이전/다음/년월) 높이를 고려한 값 → 필요시 조절 가능

//     // table 위젯을 정사각형으로 유지
//     table->setFixedSize(side, side);
// }



// // void CalendarWidget::resizeEvent(QResizeEvent *event)
// // {
// //     QWidget::resizeEvent(event);

// //     // CalendarWidget 전체 너비/높이
// //     int w = width();
// //     int h = height();

// //     // 정사각형 한 변 = 더 작은 쪽
// //     int side = qMin(w, h);

// //     // 부모 레이아웃이 CalendarWidget 크기를 조절하도록 sizeHint 적용
// //     setMinimumSize(side, side);
// //     setMaximumSize(side, side);
// // }

//////////////////////////////////////////////////////////////////////////수정 전

// #include "calendarwidget.h"
// #include <QVBoxLayout>
// #include <QHBoxLayout>
// #include <QHeaderView>
// #include <QDebug>

// CalendarWidget::CalendarWidget(QWidget *parent)
//     : QWidget(parent)
// {
//     QDate today = QDate::currentDate();
//     year = today.year();
//     month = today.month();

//     setupUI();
//     populateCalendar();
// }

// void CalendarWidget::setupUI()
// {
//     QVBoxLayout *layout = new QVBoxLayout(this);
//     layout->setContentsMargins(0,0,0,0);
//     layout->setSpacing(0);

//     QHBoxLayout *topLayout = new QHBoxLayout();
//     topLayout->setContentsMargins(0,0,0,0);

//     prevBtn = new QPushButton("이전 달", this);
//     nextBtn = new QPushButton("다음 달", this);
//     monthLabel = new QLabel(QDate(year, month, 1).toString("yyyy-MM"), this);
//     monthLabel->setAlignment(Qt::AlignCenter);

//     topLayout->addWidget(prevBtn);
//     topLayout->addStretch();
//     topLayout->addWidget(monthLabel);
//     topLayout->addStretch();
//     topLayout->addWidget(nextBtn);



//     layout->addLayout(topLayout);

//     table = new QTableWidget(6,7,this);
//     table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//     table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//     table->verticalHeader()->setVisible(false);
//     table->horizontalHeader()->setVisible(true);
//     table->setEditTriggers(QAbstractItemView::NoEditTriggers);
//     table->setSelectionMode(QAbstractItemView::NoSelection);

//     QStringList weekDays = {"일","월","화","수","목","금","토"};
//     table->setHorizontalHeaderLabels(weekDays);

//     layout->addWidget(table, 1);

//     connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
//     connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
//     connect(table, &QTableWidget::cellDoubleClicked, this, [=](int row,int col){
//         QDate firstDay(year,month,1);
//         int startCol = firstDay.dayOfWeek() % 7;
//         int day = row*7 + col - startCol + 1;
//         if(day>=1 && day<=firstDay.daysInMonth()){
//             emit dateDoubleClicked(QDate(year,month,day));
//         }
//     });
// }

// void CalendarWidget::populateCalendar()
// {
//     table->clearContents();
//     QDate firstDay(year, month, 1);
//     int startCol = firstDay.dayOfWeek() % 7;
//     int days = firstDay.daysInMonth();

//     int day = 1;
//     for(int r=0; r<6; ++r){
//         for(int c=0;c<7;++c){
//             if(r==0 && c<startCol){ table->setCellWidget(r,c,nullptr); continue;}
//             if(day<=days){
//                 QWidget *cell = new QWidget();
//                 QVBoxLayout *cellLayout = new QVBoxLayout(cell);
//                 cellLayout->setContentsMargins(2,2,2,2);
//                 QLabel *label = new QLabel(QString::number(day),cell);
//                 label->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
//                 cellLayout->addWidget(label);
//                 table->setCellWidget(r,c,cell);
//                 ++day;
//             } else table->setCellWidget(r,c,nullptr);
//         }
//     }
// }

// void CalendarWidget::showPrevMonth(){ month--; if(month<1){month=12; year--;} monthLabel->setText(QDate(year,month,1).toString("yyyy-MM")); populateCalendar();}
// void CalendarWidget::showNextMonth(){ month++; if(month>12){month=1; year++;} monthLabel->setText(QDate(year,month,1).toString("yyyy-MM")); populateCalendar(); }

// void CalendarWidget::resizeEvent(QResizeEvent *event){
//     QWidget::resizeEvent(event);

//     int size = qMin(width(), height() - 40); // 캘린더 사이즈
//     table->setFixedSize(size, size);

//     // 캘린더 기준 좌표 계산
//     int tableX = (width() - size)/2;
//     int tableY = 40; // 상단 마진
//     table->move(tableX, tableY);

//     // 버튼/레이블 위치 캘린더 기준
//     int btnWidth = 80;
//     int btnHeight = 30;
//     int labelWidth = 100;
//     int labelHeight = 30;

//     prevBtn->setFixedSize(btnWidth, btnHeight);
//     nextBtn->setFixedSize(btnWidth, btnHeight);
//     monthLabel->setFixedSize(labelWidth, labelHeight);

//     int topY = tableY - 35; // 캘린더 바로 위

//     prevBtn->move(tableX, topY);
//     nextBtn->move(tableX + size - btnWidth, topY);
//     monthLabel->move(tableX + (size - labelWidth)/2, topY);
// }
//////////////////////////////////////////////////////////////////////수정본 2
#include "calendarwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
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
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0,0,0,0);

    prevBtn = new QPushButton("이전 달", this);
    nextBtn = new QPushButton("다음 달", this);
    monthLabel = new QLabel(QDate(year, month, 1).toString("yyyy-MM"), this);
    monthLabel->setAlignment(Qt::AlignCenter);

    topLayout->addWidget(prevBtn);
    topLayout->addStretch();
    topLayout->addWidget(monthLabel);
    topLayout->addStretch();
    topLayout->addWidget(nextBtn);

    layout->addLayout(topLayout);

    table = new QTableWidget(6, 7, this);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);

    QStringList weekDays = {"일","월","화","수","목","금","토"};
    table->setHorizontalHeaderLabels(weekDays);

    layout->addWidget(table, 1);

    connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
    connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
    connect(table, &QTableWidget::cellDoubleClicked, this, &CalendarWidget::onCellDoubleClicked);
}

void CalendarWidget::populateCalendar()
{
    table->clearContents();
    QDate firstDay(year, month, 1);
    int startCol = firstDay.dayOfWeek() % 7;
    int daysInMonth = firstDay.daysInMonth();

    int day = 1;
    for(int r = 0; r < 6; ++r)
    {
        for(int c = 0; c < 7; ++c)
        {
            if(r == 0 && c < startCol)
            {
                table->setCellWidget(r, c, nullptr);
                continue;
            }

            if(day <= daysInMonth)
            {
                QWidget *cell = new QWidget(this);
                table->setCellWidget(r, c, cell);

                QVBoxLayout *cellLayout = new QVBoxLayout(cell);
                cellLayout->setContentsMargins(2,2,2,2);
                cellLayout->setSpacing(2);

                QLabel *dateLabel = new QLabel(QString::number(day), cell);
                dateLabel->setStyleSheet("font-weight:bold;");
                dateLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
                cellLayout->addWidget(dateLabel);

                QFrame *line = new QFrame(cell);
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                cellLayout->addWidget(line);

                QLabel *scheduleContent = new QLabel(cell);
                scheduleContent->setMinimumHeight(100);
                scheduleContent->setWordWrap(true);
                scheduleContent->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                scheduleContent->setObjectName("scheduleContent");
                cellLayout->addWidget(scheduleContent);

                QDate cellDate(year, month, day);


                if (cellDate == highlightedDate)
                {
                    cell->setStyleSheet("background-color: #4A90E2; color: white; border-radius: 4px;");
                }
                else
                {
                    cell->setStyleSheet("");
                }


                updateCellSchedules(cellDate);

                ++day;
            }
            else
            {
                table->setCellWidget(r, c, nullptr);
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
    // QDate firstDay(year, month, 1);
    // int startCol = firstDay.dayOfWeek() % 7;
    // int day = row*7 + column - startCol + 1;

    // if(day < 1 || day > firstDay.daysInMonth())
    //     return;

    // QDate clickedDate(year, month, day);
    // emit dateDoubleClicked(clickedDate);

    QDate first = QDate(year, month, 1);
    int startCol = first.dayOfWeek() % 7;

    int day = row * 7 + column - startCol + 1;
    if(day < 1 || day > first.daysInMonth())
        return;

    QDate clicked(year, month, day);

    highlightedDate = clicked;       // 강조 저장
    emit dateDoubleClicked(clicked); // LeftList 등으로 signal 전달

    populateCalendar();              // 강조 표시 반영
}

void CalendarWidget::addSchedule(const QDate &date, const QString &text)
{
    QString trimmed = text.trimmed();
    if(!date.isValid() || trimmed.isEmpty())
        return;

    scheduleData[date].append(trimmed);

    if(date.year() == year && date.month() == month)
        updateCellSchedules(date);
}

void CalendarWidget::removeSchedule(const QDate &date, const QString &text)
{
    if(!date.isValid() || !scheduleData.contains(date))
        return;

    QStringList &list = scheduleData[date];
    int idx = list.indexOf(text.trimmed());
    if(idx != -1)
        list.removeAt(idx);

    if(list.isEmpty())
        scheduleData.remove(date);

    if(date.year() == year && date.month() == month)
        updateCellSchedules(date);
}

void CalendarWidget::updateCellSchedules(const QDate &date)
{
    if(date.year() != year || date.month() != month)
        return;

    QDate firstDay(year, month, 1);
    int startCol = firstDay.dayOfWeek() % 7;
    int day = date.day();
    int index = day + startCol - 1;
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
    const int maxVisible = 2;
    int visibleCount = qMin(entries.size(), maxVisible);
    for(int i = 0; i < visibleCount; ++i)
        visibleLines << QStringLiteral("• %1").arg(entries.at(i));

    if(entries.size() > maxVisible)
        visibleLines << QStringLiteral("+%1 더보기").arg(entries.size() - maxVisible);

    scheduleContent->setText(visibleLines.join("\n"));
}

void CalendarWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int size = qMin(width(), height() - 40);
    table->setFixedSize(size, size);

    int tableX = (width() - size) / 2;
    int tableY = 40;

    table->move(tableX, tableY);

    int btnWidth = 80;
    int btnHeight = 30;
    int labelWidth = 100;
    int labelHeight = 30;

    prevBtn->setFixedSize(btnWidth, btnHeight);
    nextBtn->setFixedSize(btnWidth, btnHeight);
    monthLabel->setFixedSize(labelWidth, labelHeight);

    int topY = tableY - 35;
    prevBtn->move(tableX, topY);
    nextBtn->move(tableX + size - btnWidth, topY);
    monthLabel->move(tableX + (size - labelWidth)/2, topY);
}


void CalendarWidget::setHighlight(const QDate &date)
{
    if(!date.isValid()) return;

    year = date.year();
    month = date.month();
    highlightedDate = date;

    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar();
}

QMap<QDate, QStringList> CalendarWidget::getAllSchedules() const
{
    return scheduleData;
}


