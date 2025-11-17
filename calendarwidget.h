#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>
#include <QLabel>

class CalendarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CalendarWidget(QWidget *parent = nullptr);
private slots:
    void showNextMonth();
    void showPrevMonth();
    void onCellDoubleClicked(int row, int column);

private:
    void setupUI();
    void populateCalendar();

    int year;
    int month;
    QTableWidget *table;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
    QLabel *monthLabel;

signals:
    void dateDoubleClicked(const QDate &date);
};

#endif // CALENDARWIDGET_H
