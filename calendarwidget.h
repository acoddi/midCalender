#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QDate>
#include <QMap>
#include <QStringList>
#include <QResizeEvent>
#include "HolidayApiHandler.h" // 💡 API 핸들러 클래스 포함

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);

    void addSchedule(const QDate &date, const QString &text);
    void removeSchedule(const QDate &date, const QString &text);

    void setHighlight(const QDate &date);
    QMap<QDate, QStringList> getAllSchedules() const;

    int year;
    int month;

    void populateCalendar();

    void clearAllSchedules();   // 전체 일정 삭제

    int currentYear() const { return year; }        //년 정보 주는함수
    int currentMonth() const { return month; }      //월 정보 주는 함수

    void clearMonth(int y, int m);                  //월 삭제 함수




private slots:
    void showNextMonth();
    void showPrevMonth();
    void onCellDoubleClicked(int row, int column);

    //  1. API 핸들러로부터 데이터를 받았을 때 실행될 슬롯
    void onHolidaysReceived(const QMap<QDate, QString> &holidays);

    //  2. API 요청 실패 시 실행될 슬롯
    void onHolidayRequestFailed(const QString &errorMsg);


private:
    void setupUI();
    void updateCellSchedules(const QDate &date);

    //  3. 공휴일 데이터를 기반으로 달력 스타일을 적용하는 함수
    void applyHolidayStyles();


    QTableWidget *table;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QLabel *monthLabel;
    QMap<QDate, QStringList> scheduleData;

    QDate highlightedDate;       // 강조 표시할 날짜

    //  4. API 핸들러 객체
    HolidayApiHandler *apiHandler;

    //  5. 공휴일 데이터를 저장할 맵
    QMap<QDate, QString> m_holidays;


signals:
    void dateDoubleClicked(const QDate &date);

protected:
    void resizeEvent(QResizeEvent *event) override; // 캘린더 크기/위치 조정용
};

#endif // CALENDARWIDGET_H
