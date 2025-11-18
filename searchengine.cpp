// #include "searchengine.h"
// #include "ui_searchengine.h"

// searchengine::searchengine(QWidget *parent)
//     : QWidget(parent)
//     , ui(new Ui::searchengine)
// {
//     ui->setupUi(this);
//     QImage image;

//     image = QImage(":/new/prefix1/search_2356562.png");
//     ui->label->setPixmap(QPixmap::fromImage(image).scaled(
//         ui->label->size(),          // label 크기
//         Qt::KeepAspectRatio,        // 비율 유지
//         Qt::SmoothTransformation    // 부드럽게 변환
//         ));   // 이미지를 부드럽게 변환하는 코드
// }

// searchengine::~searchengine()
// {
//     delete ui;
// }

// void searchengine::textChanged(QString str)
// {
//     QString inputline;
//     inputline = ui->lineEdit->text();
// }

// void searchengine::searchButton()
// {
//     QString inputline;
//     inputline = ui->lineEdit->text();
//     if(inputline=="AAA"){
//         //ui->label_2->setText("success");
//         qDebug()<<"success";
//     }else{
//         //ui->label_2->setText("Fail");
//         qDebug()<<"Fail";

//     }
// }

///////////////////////////////////////////////////////////////////////////////
#include "searchengine.h"
#include "ui_searchengine.h"
#include <QDate>
#include <QDebug>

searchengine::searchengine(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::searchengine)
{
    ui->setupUi(this);

    resultList = ui->listWidget;  // UI에 결과 리스트 위젯 있다고 가정

    // 검색 버튼 연결
    connect(ui->pushButton, &QPushButton::clicked,
            this, &searchengine::onSearchButtonClicked);

    // 리스트 클릭 → 날짜 전송
    connect(resultList, &QListWidget::itemClicked,
            this, &searchengine::onResultItemClicked);

    QImage image;

         image = QImage(":/new/prefix1/search_2356562.png");
         ui->label->setPixmap(QPixmap::fromImage(image).scaled(
             ui->label->size(),          // label 크기
             Qt::KeepAspectRatio,        // 비율 유지
             Qt::SmoothTransformation    // 부드럽게 변환
             ));   // 이미지를 부드럽게 변환하는 코드
}

searchengine::~searchengine()
{
    delete ui;
}

//  검색 버튼 눌림
void searchengine::onSearchButtonClicked()
{
    QString keyword = ui->lineEdit->text().trimmed();
    if(keyword.isEmpty()) return;

    emit searchRequested(keyword);   // MainWindow로 검색어 보냄
}

//  MainWindow → 검색 결과 전달 받음
void searchengine::showSearchResults(const QList<QPair<QDate, QString>> &results)
{
    resultList->clear();

    for (auto &item : results)
    {
        QDate d = item.first;
        QString text = item.second;

        QListWidgetItem *listItem = new QListWidgetItem(
            QString("%1  |  %2")
                .arg(d.toString("yyyy-MM-dd"))
                .arg(text)
            );

        // 날짜 저장
        listItem->setData(Qt::UserRole, d);

        resultList->addItem(listItem);
    }
}

//  검색 결과 리스트 클릭 → 해당 날짜를 MainWindow로 전달
void searchengine::onResultItemClicked(QListWidgetItem *item)
{
    QDate d = item->data(Qt::UserRole).toDate();
    emit dateSelected(d);
}

