#include "holidayapihandler.h"
#include <QUrlQuery>
#include <QXmlStreamReader>

HolidayApiHandler::HolidayApiHandler(QObject *parent)
    : QObject{parent}
{
    networkManager = new QNetworkAccessManager(this);
}

void HolidayApiHandler::requestHolidays(int year, int month)
{

    if (SERVICE_KEY.isEmpty()) {
        emit requestFailed("Service Key가 설정되지 않았습니다. holidayapihandler.h 파일의 SERVICE_KEY를 확인하세요.");
        return;
    }

    QUrl url(API_URL);

    // 쿼리 파라미터 설정
    QUrlQuery query;
    // serviceKey는 API에서 요구하는 형태로 URL 인코딩됩니다.
    query.addQueryItem("serviceKey", SERVICE_KEY);
    query.addQueryItem("solYear", QString::number(year));
    query.addQueryItem("solMonth", QString("%1").arg(month, 2, 10, QChar('0')));
    query.addQueryItem("numOfRows", "35"); // 한 달 최대 31일이므로 충분히 설정

    url.setQuery(query);

    QNetworkRequest request(url);
    // GET 요청 및 응답 연결
    QNetworkReply *reply = networkManager->get(request);

    // 응답이 완료되면 onHolidaysReply 슬롯 실행
    connect(reply, &QNetworkReply::finished, this, &HolidayApiHandler::onHolidaysReply);
}

void HolidayApiHandler::onHolidaysReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() != QNetworkReply::NoError) {
        emit requestFailed(QString("네트워크 오류: %1").arg(reply->errorString()));
        reply->deleteLater();
        return;
    }

    QMap<QDate, QString> holidays;
    QByteArray responseData = reply->readAll();
    QXmlStreamReader xml(responseData);

    QDate currentDate;
    QString currentName;
    bool inItem = false;

    // XML 스트림 파싱
    while (!xml.atEnd() && !xml.hasError())
    {
        xml.readNext();

        if (xml.isStartElement()) {
            if (xml.name() == "item") {
                inItem = true;
                currentDate = QDate();
                currentName = QString();
            }
            else if (inItem) {
                if (xml.name() == "locdate") { // 날짜 태그 (예: 20251118)
                    xml.readNext();
                    QString dateStr = xml.text().toString();
                    currentDate = QDate::fromString(dateStr, "yyyyMMdd");
                }
                else if (xml.name() == "dateName") { // 공휴일 이름 태그
                    xml.readNext();
                    currentName = xml.text().toString();
                }
            }
        }
        else if (xml.isEndElement() && xml.name() == "item" && inItem) {
            // 항목 끝: 유효한 데이터 저장
            if (currentDate.isValid() && !currentName.isEmpty()) {
                holidays[currentDate] = currentName;
            }
            inItem = false;
        }
    }

    if (xml.hasError()) {
        emit requestFailed(QString("XML 파싱 오류: %1").arg(xml.errorString()));
    } else {
        // 성공: 파싱된 데이터 시그널로 전달
        emit holidaysReady(holidays);
    }

    reply->deleteLater(); // 메모리 해제
}
