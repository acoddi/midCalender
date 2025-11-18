#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>
#include <QDate>
#include <QString>

namespace Ui {
class ModifyDialog;
}

class ModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyDialog(QWidget *parent = nullptr);
    ~ModifyDialog();

    void setTodoText(const QString &text);
    QString todoText() const;

    void setDate(const QDate &date);
    QDate date() const;

private:
    Ui::ModifyDialog *ui;
};

#endif // MODIFYDIALOG_H
