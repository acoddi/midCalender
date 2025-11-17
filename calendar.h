#ifndef CALENDAR_H
#define CALENDAR_H

#include <QWidget>
#include<QTableWidget>
#include<QPushButton>

class calendar : public QWidget
{
    Q_OBJECT
public:
    explicit calendar(QWidget *parent = nullptr);

signals:
private slots:
    void showNextMonth();
    void showPrevMonth();

private:
    void setupUI();
    void populateCalendar();

    int year;
    int month;
    QTableWidget *table;
    QPushButton *nextBtn;
    QPushButton *prevBtn;
};

#endif // CALENDAR_H
