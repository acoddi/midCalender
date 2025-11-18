cmake_minimum_required(VERSION 3.16)

project(calenderProject VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 💡 수정: 필요한 모든 Qt 모듈을 한 번에 찾고 'Network' 모듈을 추가합니다.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Network)


set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    calendarwidget.h calendarwidget.cpp
    schedule.h schedule.cpp
    leftlist.cpp leftlist.h
    searchengine.cpp searchengine.h searchengine.ui
    resources/resources.qrc
    holidayapihandler.h holidayapihandler.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(calenderProject
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        toolbox.h toolbox.cpp toolbox.ui
        modifydialog.h modifydialog.cpp modifydialog.ui

    )
else()
    if(ANDROID)
        add_library(calenderProject SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(calenderProject
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 💡 수정: 모든 필요한 라이브러리를 한 번에 링크합니다.
target_link_libraries(calenderProject PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network # 🚨 네트워크 오류 해결을 위해 이 라인 추가
)


# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.calenderProject)
endif()
set_target_properties(calenderProject PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS calenderProject
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(calenderProject)
endif()
