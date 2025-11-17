#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "calendarwidget.h"
#include "schedule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    calender = new CalendarWidget(this);
    mainLayout->addWidget(calender, 1);

    schedule = new Schedule(this);
    schedule->setMinimumWidth(250);
    schedule->hide();
    mainLayout->addWidget(schedule, 0);

    connect(calender, &CalendarWidget::dateDoubleClicked, this, [=](const QDate &date) {
        currentSelectedDate = date;
        schedule->setSelectedDate(currentSelectedDate);
        schedule->show();
    });

    connect(schedule, &Schedule::todoAdded, this, [=](const QString &text, const QDate &start, const QDate &end) {
        if (!start.isValid() || !end.isValid())
            return;

        QDate from = start;
        QDate to = end;
        if(to < from)
            std::swap(from, to);

        for(QDate day = from; day <= to; day = day.addDays(1))
        {
            calender->addSchedule(day, text);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
