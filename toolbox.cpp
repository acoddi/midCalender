#include "toolbox.h"

ToolBox::ToolBox(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    btnClearAll = new QPushButton("전체 초기화", this);
    btnClearMonth = new QPushButton("월 초기화", this);
    btnExit = new QPushButton("종료", this);

    btnClearAll->setMinimumHeight(32);
    btnClearMonth->setMinimumHeight(32);
    btnExit->setMinimumHeight(32);

    //layout->addStretch();

    // 버튼 스타일 적용
    QString style =
        "QPushButton {"
        "background-color: #4A90E2;"
        "border-radius: 6px;"
        "color: white;"
        "padding: 6px 12px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #357ABD;"
        "}";

    btnClearAll->setStyleSheet(style);
    btnClearMonth->setStyleSheet(style);
    btnExit->setStyleSheet(style);

    layout->addWidget(btnClearAll);
    layout->addWidget(btnClearMonth);
    layout->addWidget(btnExit);
    layout->addStretch();

    // 시그널 커넥트
    connect(btnClearAll, &QPushButton::clicked, this, &ToolBox::clearAllRequested);
    connect(btnClearMonth, &QPushButton::clicked, this, &ToolBox::clearMonthRequested);
    connect(btnExit, &QPushButton::clicked, this, &ToolBox::exitRequested);
}
