#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <QWidget>

namespace Ui {
class searchengine;
}

class searchengine : public QWidget
{
    Q_OBJECT

public:
    explicit searchengine(QWidget *parent = nullptr);
    ~searchengine();

public slots:
    void textChanged(QString str);
    void searchButton();


private:
    Ui::searchengine *ui;
};

#endif // SEARCHENGINE_H
