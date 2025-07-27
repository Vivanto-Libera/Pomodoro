QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    musiclistsdialog.cpp \
    notewindow.cpp \
    savesettingdialog.cpp \
    setdialog.cpp

HEADERS += \
    mainwindow.h \
    musiclistsdialog.h \
    notewindow.h \
    savesettingdialog.h \
    setdialog.h

FORMS += \
    mainwindow.ui \
    musiclistsdialog.ui \
    notewindow.ui \
    savesettingdialog.ui \
    setdialog.ui

TRANSLATIONS += \
    Pomodoro_zh_CN.ts \
    Pomodoro_zh_TW.ts \
    Pomodoro_en.ts \
    Pomodoro_eo.ts
CONFIG += lrelease
CONFIG += embed_translations

RC_ICONS = pomodoro.ico

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
