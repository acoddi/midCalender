#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include <QLabel>
#include <QDate>
#include <QMap>
#include <QStringList>

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);

    void addSchedule(const QDate &date, const QString &text);
    void removeSchedule(const QDate &date, const QString &text);

private slots:
    void showNextMonth();
    void showPrevMonth();
    void onCellDoubleClicked(int row, int column);

private:
    void setupUI();
    void populateCalendar();
    void updateCellSchedules(const QDate &date);

    int year;
    int month;
    QTableWidget *table;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QLabel *monthLabel;
    QMap<QDate, QStringList> scheduleData;

signals:
    void dateDoubleClicked(const QDate &date);
};

#endif // CALENDARWIDGET_H
