#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

class ToolBox : public QWidget
{
    Q_OBJECT
public:
    explicit ToolBox(QWidget *parent = nullptr);

signals:
    void clearAllRequested();   // 전체 초기화 요청
    void clearMonthRequested(); // 해당 월 초기화 요청
    void exitRequested();       // 종료 요청

private:
    QPushButton *btnClearAll;
    QPushButton *btnClearMonth;
    QPushButton *btnExit;
};

#endif // TOOLBOX_H
