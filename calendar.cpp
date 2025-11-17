#include "calendar.h"
#include <QVBoxLayout>
#include <QDate>

calendar::calendar(QWidget *parent)
    : QWidget{parent}
{
    setupUI();
    populaterCalender();
}

void Calender::
