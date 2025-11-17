#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include"CalendarWidget.h"
#include"schedule.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CalendarWidget *calender;
    Schedule *schedule;
};
#endif // MAINWINDOW_H
