#include "modifydialog.h"
#include "ui_modifydialog.h"

ModifyDialog::ModifyDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ModifyDialog)
{
    ui->setupUi(this);
    ui->dateEdit->setCalendarPopup(true);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->textEdit->setPlaceholderText(QStringLiteral("내용을 수정하세요"));
}

ModifyDialog::~ModifyDialog()
{
    delete ui;
}

void ModifyDialog::setTodoText(const QString &text)
{
    ui->textEdit->setPlainText(text);
}

QString ModifyDialog::todoText() const
{
    return ui->textEdit->toPlainText();
}

void ModifyDialog::setDate(const QDate &date)
{
    const QDate targetDate = date.isValid() ? date : QDate::currentDate();
    ui->dateEdit->setDate(targetDate);
}

QDate ModifyDialog::date() const
{
    return ui->dateEdit->date();
}
