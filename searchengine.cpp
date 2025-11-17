#include "searchengine.h"
#include "ui_searchengine.h"

searchengine::searchengine(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::searchengine)
{
    ui->setupUi(this);
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

void searchengine::textChanged(QString str)
{
    QString inputline;
    inputline = ui->lineEdit->text();
}

void searchengine::searchButton()
{
    QString inputline;
    inputline = ui->lineEdit->text();
    if(inputline=="AAA"){
        //ui->label_2->setText("success");
        qDebug()<<"success";
    }else{
        //ui->label_2->setText("Fail");
        qDebug()<<"Fail";

    }
}

