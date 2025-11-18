#include "calendarwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QFrame>
#include <QResizeEvent>
#include <QColor>
#include <QBrush>
#include <QPalette>
#include <QDate>
#include <QTableWidgetItem>
#include <QMessageBox>

// 생성자: 초기화 및 UI 설정
CalendarWidget::CalendarWidget(QWidget *parent)
    : QWidget(parent)
{
    QDate today = QDate::currentDate();
    year = today.year(); // 현재 연도 설정
    month = today.month(); // 현재 월 설정

    apiHandler = new HolidayApiHandler(this); // API 핸들러 객체 생성
    // 공휴일 데이터 수신 시 슬롯 연결
    connect(apiHandler, &HolidayApiHandler::holidaysReady,
            this, &CalendarWidget::onHolidaysReceived);
    // API 요청 실패 시 슬롯 연결
    connect(apiHandler, &HolidayApiHandler::requestFailed,
            this, &CalendarWidget::onHolidayRequestFailed);

    setupUI(); // UI 구성
    populateCalendar(); // 달력 내용 채우기 (API 호출 포함)
}

// UI 구성 요소 설정
void CalendarWidget::setupUI()
{
    QVBoxLayout *layout = new QVBoxLayout(this); // 주 레이아웃
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);

    QHBoxLayout *topLayout = new QHBoxLayout(); // 상단 레이아웃 (버튼/라벨)

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

    table = new QTableWidget(6, 7, this); // 6주 x 7일 테이블 생성
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 가로 헤더 늘리기
    table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 세로 헤더 늘리기
    table->verticalHeader()->setVisible(false); // 행 번호 숨기기
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 편집 불가 설정
    table->setSelectionMode(QAbstractItemView::NoSelection); // 선택 불가 설정

    QStringList weekDays = {"일","월","화","수","목","금","토"};
    table->setHorizontalHeaderLabels(weekDays); // 요일 설정

    layout->addWidget(table, 1);

    // 버튼 클릭 슬롯 연결
    connect(prevBtn, &QPushButton::clicked, this, &CalendarWidget::showPrevMonth);
    connect(nextBtn, &QPushButton::clicked, this, &CalendarWidget::showNextMonth);
    // 셀 더블 클릭 슬롯 연결
    connect(table, &QTableWidget::cellDoubleClicked, this, &CalendarWidget::onCellDoubleClicked);
}

// 현재 월의 달력을 테이블 위젯에 채우는 함수
void CalendarWidget::populateCalendar()
{
    table->clearContents(); // 기존 내용 초기화
    m_holidays.clear(); // 공휴일 데이터 초기화

    QDate firstDay(year, month, 1);
    int startCol = firstDay.dayOfWeek() % 7; // 1일이 시작하는 열 위치 계산 (일=0)
    int daysInMonth = firstDay.daysInMonth(); // 해당 월의 일수

    int day = 1;
    for(int r = 0; r < 6; ++r) // 6주 순회
    {
        for(int c = 0; c < 7; ++c) // 7일 순회
        {
            // 월 시작 전 빈 셀 처리
            if(r == 0 && c < startCol)
            {
                if (table->item(r, c) == nullptr) {
                    table->setItem(r, c, new QTableWidgetItem(""));
                }
                table->item(r, c)->setBackground(QBrush(QColor(240, 240, 240))); // 회색 배경
                table->setCellWidget(r, c, nullptr);
                continue;
            }

            // 해당 월의 날짜인 경우
            if(day <= daysInMonth)
            {
                QWidget *cell = new QWidget(this);
                cell->setStyleSheet("background-color: transparent; color: black; border: none;");
                table->setCellWidget(r, c, cell);

                QVBoxLayout *cellLayout = new QVBoxLayout(cell);
                cellLayout->setContentsMargins(2,2,2,2);
                cellLayout->setSpacing(2);

                QLabel *dateLabel = new QLabel(QString::number(day), cell);
                dateLabel->setStyleSheet("font-weight:bold; color: black;");
                dateLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

                // 요일에 따른 기본 색상 적용
                if (c == 0) { // 일요일
                    dateLabel->setStyleSheet("font-weight:bold; color: red;");
                } else if (c == 6) { // 토요일
                    dateLabel->setStyleSheet("font-weight:bold; color: blue;");
                }

                dateLabel->setObjectName("dateLabel");
                cellLayout->addWidget(dateLabel);

                QFrame *line = new QFrame(cell); // 구분선
                line->setFrameShape(QFrame::HLine);
                line->setFrameShadow(QFrame::Sunken);
                cellLayout->addWidget(line);

                QLabel *scheduleContent = new QLabel(cell); // 일정 표시 라벨
                // 💡 HTML 콘텐츠를 올바르게 처리하기 위해 RichText 포맷 설정
                scheduleContent->setTextFormat(Qt::RichText);
                scheduleContent->setMinimumHeight(100);
                scheduleContent->setWordWrap(true);
                scheduleContent->setAlignment(Qt::AlignTop | Qt::AlignLeft);
                scheduleContent->setObjectName("scheduleContent");
                cellLayout->addWidget(scheduleContent);

                QDate cellDate(year, month, day);

                // 강조 날짜 표시 (선택 날짜)
                if (cellDate == highlightedDate)
                {
                    cell->setStyleSheet("background-color: #4A90E2; color: white; border-radius: 4px;");
                    dateLabel->setStyleSheet(dateLabel->styleSheet() + " color: white;");
                }

                updateCellSchedules(cellDate); // 저장된 일정 로드

                ++day;
            }
            // 월 종료 후 빈 셀 처리
            else
            {
                if (table->item(r, c) == nullptr) {
                    table->setItem(r, c, new QTableWidgetItem(""));
                }
                table->item(r, c)->setBackground(QBrush(QColor(240, 240, 240)));
                table->setCellWidget(r, c, nullptr);
            }
        }
    }

    apiHandler->requestHolidays(year, month); // 공휴일 데이터 API 호출
}

// 이전 달로 이동
void CalendarWidget::showPrevMonth()
{
    month--;
    if(month < 1)
    {
        month = 12;
        year--;
    }
    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar(); // 달력 갱신
}

// 다음 달로 이동
void CalendarWidget::showNextMonth()
{
    month++;
    if(month > 12)
    {
        month = 1;
        year++;
    }
    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar(); // 달력 갱신
}

// 셀 더블 클릭 이벤트 처리
void CalendarWidget::onCellDoubleClicked(int row, int column)
{
    QDate first = QDate(year, month, 1);
    int startCol = first.dayOfWeek() % 7;

    int day = row * 7 + column - startCol + 1;
    if(day < 1 || day > first.daysInMonth()) return; // 유효하지 않은 날짜 클릭 무시

    QDate clicked(year, month, day);

    highlightedDate = clicked;
    emit dateDoubleClicked(clicked); // 외부로 시그널 전송

    populateCalendar(); // UI 갱신 (강조 표시 반영)
}

// 일정 추가 로직
void CalendarWidget::addSchedule(const QDate &date, const QString &text)
{
    QString trimmed = text.trimmed();
    if(!date.isValid() || trimmed.isEmpty()) return;

    scheduleData[date].append(trimmed); // 데이터에 저장

    if(date.year() == year && date.month() == month)
        updateCellSchedules(date); // 현재 월이면 UI 갱신
}

// 일정 제거 로직
void CalendarWidget::removeSchedule(const QDate &date, const QString &text)
{
    if(!date.isValid() || !scheduleData.contains(date)) return;

    QStringList &list = scheduleData[date];
    int idx = list.indexOf(text.trimmed());
    if(idx != -1) list.removeAt(idx); // 리스트에서 제거

    if(list.isEmpty()) scheduleData.remove(date); // 빈 리스트면 맵에서 키 제거

    if(date.year() == year && date.month() == month)
        updateCellSchedules(date); // 현재 월이면 UI 갱신
}

// 특정 날짜 셀의 일정 UI 갱신
void CalendarWidget::updateCellSchedules(const QDate &date)
{
    if(date.year() != year || date.month() != month) return;

    QDate firstDay(year, month, 1);
    int startCol = firstDay.dayOfWeek() % 7;
    int day = date.day();
    int index = day + startCol - 1;
    int row = index / 7;
    int col = index % 7;

    QWidget *cellWidget = table->cellWidget(row, col);
    if(!cellWidget) return;

    QLabel *scheduleContent = cellWidget->findChild<QLabel*>("scheduleContent");
    if(!scheduleContent) return;

    const QStringList entries = scheduleData.value(date);

    QStringList lines;

    // 1) 공휴일 이름 한 번만 맨 위에 표시
    if (m_holidays.contains(date)) {
        const QString holidayName = m_holidays.value(date);
        lines << QString("<font color='red'>%1</font>").arg(holidayName);
    }

    // 2) 사용자 일정 표시 (최대 2개 + "외 +N")
    if(!entries.isEmpty())
    {
        const int maxVisible = 2;
        int visibleCount = qMin(entries.size(), maxVisible);
        for(int i = 0; i < visibleCount; ++i)
            lines << QStringLiteral("• %1").arg(entries.at(i));

        if(entries.size() > maxVisible)
            lines << QStringLiteral("외 +%1").arg(entries.size() - maxVisible);
    }

    // RichText 포맷으로 라인들을 <br>로 연결해서 설정
    scheduleContent->setText(lines.join("<br>"));
}

// 위젯 크기 변경 이벤트 (테이블 중앙 정렬 및 크기 조정)
void CalendarWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    int size = qMin(width(), height() - 40); // 정사각형 크기 계산
    table->setFixedSize(size, size);

    int tableX = (width() - size) / 2; // 테이블 중앙 X 좌표
    int tableY = 40;

    table->move(tableX, tableY); // 테이블 이동

    // 상단 버튼/라벨 크기 정의 및 이동
    int btnWidth = 80;
    int btnHeight = 30;
    int labelWidth = 100;

    prevBtn->setFixedSize(btnWidth, btnHeight);
    nextBtn->setFixedSize(btnWidth, btnHeight);
    monthLabel->setFixedSize(labelWidth, btnHeight);

    int topY = tableY - 35;
    prevBtn->move(tableX, topY);
    nextBtn->move(tableX + size - btnWidth, topY);
    monthLabel->move(tableX + (size - labelWidth)/2, topY);
}


// 특정 날짜로 이동 및 강조 표시
void CalendarWidget::setHighlight(const QDate &date)
{
    if(!date.isValid()) return;

    year = date.year();
    month = date.month();
    highlightedDate = date;

    monthLabel->setText(QDate(year, month, 1).toString("yyyy-MM"));
    populateCalendar(); // 갱신
}

// 전체 일정 데이터 반환
QMap<QDate, QStringList> CalendarWidget::getAllSchedules() const
{
    return scheduleData;
}


// 공휴일 데이터 수신 슬롯
void CalendarWidget::onHolidaysReceived(const QMap<QDate, QString> &holidays)
{
    m_holidays.clear();
    m_holidays = holidays; // 데이터 저장

    applyHolidayStyles(); // 스타일 적용 및 공휴일 명칭 표시
}

// API 요청 실패 슬롯
void CalendarWidget::onHolidayRequestFailed(const QString &errorMsg)
{
    qWarning() << "공휴일 API 요청 실패:" << errorMsg;
    QMessageBox::warning(this, "API 오류", "공휴일 정보를 가져오는 데 실패했습니다: " + errorMsg);
}


// 공휴일 스타일 적용 및 명칭 표시 로직
void CalendarWidget::applyHolidayStyles()
{
    QDate firstDay(year, month, 1);
    int startCol = firstDay.dayOfWeek() % 7;
    int daysInMonth = firstDay.daysInMonth();

    for (int day = 1; day <= daysInMonth; ++day) // 일자별 순회
    {
        QDate cellDate(year, month, day);

        if (m_holidays.contains(cellDate)) // 공휴일인 경우
        {
            int index = day + startCol - 1;
            int row = index / 7;
            int col = index % 7;

            QWidget *cellWidget = table->cellWidget(row, col);
            if (cellWidget) {
                QLabel *dateLabel = cellWidget->findChild<QLabel*>("dateLabel");

                const QString holidayName = m_holidays.value(cellDate);

                if (dateLabel) {
                    dateLabel->setStyleSheet("font-weight: bold; color: red;"); // 날짜 텍스트 빨간색 변경

                    if (cellDate != highlightedDate) {
                        cellWidget->setToolTip(holidayName); // 툴팁 설정
                    }
                }

                // 내용 텍스트는 updateCellSchedules()에서 공휴일/일정을 한 번에 구성하도록 함
                updateCellSchedules(cellDate);
            }
        }
    }
}

void CalendarWidget::clearAllSchedules()
{
    scheduleData.clear();      // 모든 일정 데이터 삭제
    highlightedDate = QDate(); // 강조 날짜 초기화 (선택 사항)

    populateCalendar();        // UI 그래픽 리셋
}

void CalendarWidget::clearMonth(int y, int m)
{
    QList<QDate> keys = scheduleData.keys();
    for (const QDate &d : keys) {
        if (d.year() == y && d.month() == m)
            scheduleData.remove(d);
    }

    populateCalendar();
}

