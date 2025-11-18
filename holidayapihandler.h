#ifndef HOLIDAYAPIHANDLER_H
#define HOLIDAYAPIHANDLER_H

#include <QObject>
#include <QDate>
#include <QMap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

class HolidayApiHandler : public QObject
{
    Q_OBJECT
public:
    explicit HolidayApiHandler(QObject *parent = nullptr);

    void requestHolidays(int year, int month);

signals:
    void holidaysReady(const QMap<QDate, QString> &holidays);
    void requestFailed(const QString &errorMsg);

private slots:
    void onHolidaysReply();


private:
    QNetworkAccessManager *networkManager;
    const QString SERVICE_KEY ="ef02eaf39820e04c4f756124f4750fbf7a209ef54448173620c080bdc144aab4";
    const QString API_URL ="https://apis.data.go.kr/B090041/openapi/service/SpcdeInfoService/getHoliDeInfo";
};

#endif // HOLIDAYAPIHANDLER_H
