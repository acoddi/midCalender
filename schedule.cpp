#include "schedule.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include "modifydialog.h"

Schedule::Schedule(QWidget *parent)
    : QWidget(parent)
{
    // 기본 선택 날짜 설정: 현재 날짜
    selectedDate = QDate::currentDate();

    // 메인 레이아웃 설정 (세로 방향 레이아웃)
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);  // 마진 설정
    mainLayout->setSpacing(5);  // 아이템 간의 간격 설정

    // 할 일 탭 위젯 생성
    todoWidget = new QTabWidget(this);
    mainLayout->addWidget(todoWidget);  // 탭 위젯을 메인 레이아웃에 추가

    // 새 탭 추가
    QWidget *tab = new QWidget();
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);  // 세로 레이아웃
    tabLayout->setContentsMargins(5,5,5,5);  // 마진 설정
    tabLayout->setSpacing(5);  // 아이템 간의 간격 설정

    // 상단: 할 일 입력 부분
    todoLabel = new QLabel(this);  // 라벨 생성
    todoLabel->setText(QString("할 일 내용: (%1)").arg(selectedDate.toString("yyyy-MM-dd")));  // 날짜 표시
    todoEdit = new QTextEdit(this);  // 텍스트 입력 필드
    todoEdit->setPlaceholderText("할 일을 입력하세요");  // placeholder 텍스트 설정
    todoEdit->setFixedHeight(50);  // 고정 높이 설정

    tabLayout->addWidget(todoLabel);  // 라벨 추가
    tabLayout->addWidget(todoEdit);   // 텍스트 편집기 추가

    // 중간: 오늘의 할 일 리스트 표시
    QLabel *todoLabelWill = new QLabel("오늘의 할 일", this);  // 할 일 목록을 보여주는 라벨
    todoListWidget = new QListWidget(this);  // 할 일 목록 위젯

    tabLayout->addWidget(todoLabelWill);  // 라벨 추가
    tabLayout->addWidget(todoListWidget, 1);  // 리스트 위젯 추가, 공간을 최대한 차지하도록 설정

    // 하단: 날짜 입력 부분
    QHBoxLayout *dateLayout = new QHBoxLayout();  // 날짜 입력을 위한 가로 레이아웃

    fromLabel = new QLabel("시작일:", this);  // 시작일 라벨
    startDateEdit = new QDateEdit(selectedDate, this);  // 시작일 날짜 선택 위젯
    startDateEdit->setCalendarPopup(true);  // 날짜 선택 팝업 활성화

    QLabel *toLabel = new QLabel("종료일:", this);  // 종료일 라벨
    endDateEdit = new QDateEdit(selectedDate, this);  // 종료일 날짜 선택 위젯
    endDateEdit->setCalendarPopup(true);  // 날짜 선택 팝업 활성화

    dateLayout->addWidget(fromLabel);  // 시작일 라벨 추가
    dateLayout->addWidget(startDateEdit);  // 시작일 위젯 추가

    dateLayout->addWidget(toLabel);  // 종료일 라벨 추가
    dateLayout->addWidget(endDateEdit);  // 종료일 위젯 추가

    tabLayout->addLayout(dateLayout);  // 날짜 입력 레이아웃 추가

    // 하단: 버튼 추가 (추가 및 취소)
    QHBoxLayout *btnLayout = new QHBoxLayout();  // 버튼을 위한 가로 레이아웃
    QPushButton *addBtn = new QPushButton("추가", this);  // 추가 버튼
    QPushButton *deleteBtn = new QPushButton("취소", this);  // 취소 버튼

    btnLayout->addStretch();  // 버튼들 사이에 여백 추가
    btnLayout->addWidget(addBtn);  // 추가 버튼 추가
    btnLayout->addWidget(deleteBtn);  // 취소 버튼 추가

    tabLayout->addLayout(btnLayout);  // 버튼 레이아웃 추가

    todoWidget->addTab(tab, "새 할 일");  // 탭 위젯에 "새 할 일" 탭 추가

    // 버튼 클릭 시 동작 연결
    connect(addBtn, &QPushButton::clicked, this, [=]() {
        QString todoText = todoEdit->toPlainText().trimmed();  // 입력된 할 일 텍스트
        if(todoText.isEmpty()) return;  // 텍스트가 비어 있으면 추가하지 않음

        // 시작일과 종료일 설정
        QDate start = startDateEdit->date();
        QDate end = endDateEdit->date();
        if (end < start) {
            QMessageBox::warning(this, "잘못된 날짜", "종료일이 시작일보다 이전일 수 없습니다.");
            return;  // 종료일이 잘못되었으면 추가 작업을 하지 않음
        }

        addTodoItem(todoText, start, end);  // 할 일 추가 함수 호출
        emit todoAdded(todoText, start, end);  // 할 일이 추가되었음을 신호로 전달

        todoEdit->clear();  // 입력 필드 비우기
        startDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());  // 날짜 리셋
        endDateEdit->setDate(selectedDate.isValid() ? selectedDate : QDate::currentDate());  // 날짜 리셋
    });

    // 취소 버튼 클릭 시 동작 연결
    connect(deleteBtn, &QPushButton::clicked, this, &Schedule::hide);  // 창 숨기기
}

void Schedule::addTodoItem(const QString &text, const QDate &start, const QDate &end)
{
    QString trimmed = text.trimmed();  // 텍스트 공백 제거
    if(!todoListWidget || trimmed.isEmpty())  // 텍스트가 비어 있거나 할 일 목록 위젯이 없다면 리턴
        return;

    QDate from = start;
    QDate to = end;
    if(!from.isValid() || !to.isValid())  // 날짜가 유효하지 않으면 기본 날짜로 설정
        from = to = selectedDate.isValid() ? selectedDate : QDate::currentDate();
    if(to < from)  // 종료일이 시작일보다 이전이면 경고 후 리턴
    {
        QMessageBox::warning(this, "잘못된 날짜", "종료일이 시작일보다 이전일 수 없습니다.");
        return;
    }

    // 지정된 날짜 범위에 대해 할 일 추가
    for(QDate day = from; day <= to; day = day.addDays(1))
        todosByDate[day].append(trimmed);  // 날짜별로 할 일 추가

    refreshTodoList();  // 할 일 목록 갱신
}

void Schedule::setSelectedDate(const QDate &date)
{
    selectedDate = date.isValid() ? date : QDate::currentDate();  // 유효한 날짜 설정

    // 날짜 선택기에 현재 선택된 날짜 설정
    if(startDateEdit) startDateEdit->setDate(selectedDate);
    if(endDateEdit) endDateEdit->setDate(selectedDate);

    // 할 일 내용 옆에 날짜 업데이트
    if(todoLabel)
    {
        todoLabel->setText(QString("할 일 내용: (%1)").arg(selectedDate.toString("yyyy-MM-dd")));
    }

    refreshTodoList();  // 할 일 목록 갱신
}

void Schedule::refreshTodoList()
{
    if(!todoListWidget) return;  // 할 일 목록 위젯이 없다면 리턴
    todoListWidget->clear();  // 기존 목록을 지운다
    if(!selectedDate.isValid()) return;  // 날짜가 유효하지 않으면 리턴

    const QStringList items = todosByDate.value(selectedDate);  // 선택된 날짜에 해당하는 할 일 목록

    // 할 일 항목들 추가
    for(int i = 0; i < items.size(); ++i) {
        const QString &text = items.at(i);

        QListWidgetItem *item = new QListWidgetItem(todoListWidget);  // 할 일 항목 추가
        item->setSizeHint(QSize(0, 30));  // 항목 크기 설정

        QWidget *rowWidget = new QWidget(todoListWidget);  // 항목을 담을 위젯 생성
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);  // 수평 레이아웃
        rowLayout->setContentsMargins(2, 2, 2, 2);  // 마진 설정
        rowLayout->setSpacing(4);  // 간격 설정

        QLabel *label = new QLabel(text, rowWidget);  // 할 일 텍스트 라벨
        QPushButton *deleteBtn = new QPushButton("삭제", rowWidget);  // 삭제 버튼
        deleteBtn->setFixedWidth(40);  // 삭제 버튼 크기 고정
        QPushButton *modifyBtn = new QPushButton("변경", rowWidget);
        modifyBtn->setFixedWidth(40);  // 변경 버튼 크기 고정

        rowLayout->addWidget(label);  // 라벨 추가
        rowLayout->addStretch();  // 여백 추가
        rowLayout->addWidget(modifyBtn);  // 변경 버튼 추가
        rowLayout->addWidget(deleteBtn);  // 삭제 버튼 추가

        todoListWidget->setItemWidget(item, rowWidget);  // 항목에 위젯 추가

        // 삭제 버튼 클릭 시 할 일 삭제
        connect(deleteBtn, &QPushButton::clicked, this, [=]() {
            if(!selectedDate.isValid()) return;  // 날짜가 유효하지 않으면 리턴
            QStringList &listRef = todosByDate[selectedDate];  // 해당 날짜의 할 일 목록
            int row = todoListWidget->row(item);  // 항목의 인덱스
            if(row >= 0 && row < listRef.size()) {  // 인덱스가 유효한지 확인
                QString removedText = listRef.at(row);  // 삭제할 텍스트
                listRef.removeAt(row);  // 항목 삭제
                emit todoRemoved(removedText, selectedDate);  // 삭제 신호 발생
            }
            refreshTodoList();  // 목록 갱신
        });

        // 변경 버튼 클릭 시 할 일 수정 다이얼로그 표시
        connect(modifyBtn, &QPushButton::clicked, this, [=]() {
            const QDate originalDate = selectedDate.isValid() ? selectedDate : QDate::currentDate();
            QStringList &listRef = todosByDate[originalDate];
            const int row = todoListWidget->row(item);
            if(row < 0 || row >= listRef.size())
                return;

            const QString originalText = listRef.at(row);
            ModifyDialog dialog(this);
            dialog.setTodoText(originalText);
            dialog.setDate(originalDate);

            if(dialog.exec() != QDialog::Accepted)
                return;

            const QString updatedText = dialog.todoText().trimmed();
            if(updatedText.isEmpty()) {
                QMessageBox::warning(this, "잘못된 입력", "변경할 내용을 입력하세요.");
                return;
            }

            QDate updatedDate = dialog.date();
            if(!updatedDate.isValid())
                updatedDate = originalDate;

            if(updatedDate == originalDate) {
                if(updatedText == originalText) {
                    refreshTodoList();
                    return;
                }
                listRef[row] = updatedText;
                emit todoModified(originalText, originalDate, updatedText, updatedDate);
                refreshTodoList();
                return;
            }

            listRef.removeAt(row);
            if(listRef.isEmpty())
                todosByDate.remove(originalDate);

            todosByDate[updatedDate].append(updatedText);
            emit todoModified(originalText, originalDate, updatedText, updatedDate);
            setSelectedDate(updatedDate);
        });
    }
}

bool Schedule::saveToJson(const QString &filename) const
{
    QJsonObject rootObject;
    QJsonArray todosArray;
    
    // todosByDate 맵의 모든 항목을 JSON 배열로 변환
    for(auto it = todosByDate.begin(); it != todosByDate.end(); ++it)
    {
        QDate date = it.key();
        QStringList todos = it.value();
        
        // 각 날짜에 대한 JSON 객체 생성
        QJsonObject dateObject;
        dateObject["date"] = date.toString(Qt::ISODate);  // ISO 형식으로 날짜 저장 (yyyy-MM-dd)
        
        // 해당 날짜의 할 일 목록을 JSON 배열로 변환
        QJsonArray todosList;
        for(const QString &todo : todos)
        {
            todosList.append(todo);
        }
        dateObject["todos"] = todosList;
        
        todosArray.append(dateObject);
    }
    
    rootObject["todos"] = todosArray;
    
    // JSON 문서 생성
    QJsonDocument doc(rootObject);
    
    // 파일 저장
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly))
    {
        return false;  // 파일 열기 실패
    }
    
    file.write(doc.toJson());
    file.close();
    
    return true;
}

bool Schedule::loadFromJson(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;  // 파일이 없거나 열 수 없음 (처음 실행 시 정상)
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    // JSON 문서 파싱
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if(error.error != QJsonParseError::NoError)
    {
        return false;  // JSON 파싱 오류
    }
    
    if(!doc.isObject())
    {
        return false;  // 루트가 객체가 아님
    }
    
    QJsonObject rootObject = doc.object();
    if(!rootObject.contains("todos") || !rootObject["todos"].isArray())
    {
        return false;  // todos 배열이 없음
    }
    
    // 기존 할 일 목록 초기화
    todosByDate.clear();
    
    // JSON 배열에서 데이터 로드
    QJsonArray todosArray = rootObject["todos"].toArray();
    for(const QJsonValue &value : todosArray)
    {
        if(!value.isObject())
            continue;
        
        QJsonObject dateObject = value.toObject();
        if(!dateObject.contains("date") || !dateObject.contains("todos"))
            continue;
        
        // 날짜 파싱
        QString dateString = dateObject["date"].toString();
        QDate date = QDate::fromString(dateString, Qt::ISODate);
        if(!date.isValid())
            continue;
        
        // 할 일 목록 파싱
        if(!dateObject["todos"].isArray())
            continue;
        
        QJsonArray todosList = dateObject["todos"].toArray();
        QStringList todos;
        for(const QJsonValue &todoValue : todosList)
        {
            if(todoValue.isString())
            {
                todos.append(todoValue.toString());
            }
        }
        
        // 데이터 복원
        if(!todos.isEmpty())
        {
            todosByDate[date] = todos;
        }
    }
    
    // UI 갱신
    refreshTodoList();
    
    return true;
}
