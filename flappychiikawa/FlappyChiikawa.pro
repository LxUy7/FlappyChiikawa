QT       += core gui
QT      += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp\
    chiikawa.cpp \
    mainwindow.cpp \
    ground.cpp \
    pipe.cpp \
    scoreboard.cpp \
    fbnumlcd.cpp \
    redyboard.cpp

HEADERS  += mainwindow.h \
    chiikawa.h \
    ground.h \
    pipe.h \
    scoreboard.h \
    fbnumlcd.h \
    redyboard.h

FORMS    +=

RESOURCES += \
    flappy.qrc \
    music.qrc

DISTFILES += \
    myicon.rc
RC_FILE += \
    myicon.rc


