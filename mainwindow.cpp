#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CalendarWidget.h"
#include "schedule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(10,10,10,10);

    calender = new CalendarWidget(this);
    mainLayout->addWidget(calender, 1);

    schedule = new Schedule(this);
    schedule->setMinimumWidth(250);
    schedule->hide();
    mainLayout->addWidget(schedule, 0);

    connect(calender, &CalendarWidget::dateDoubleClicked, this, [=](const QDate &date){
        schedule->show();
        qDebug() << "Selected date:" << date;
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}
