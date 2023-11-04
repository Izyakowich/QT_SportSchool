QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminpanel.cpp \
    indtra.cpp \
    intru.cpp \
    main.cpp \
    mainwindow.cpp \
    placea.cpp \
    placeu.cpp \
    sectiona.cpp \
    sectionu.cpp \
    studenta.cpp \
    studentu.cpp \
    trainer.cpp \
    traineru.cpp \
    userpanel.cpp \
    visitinga.cpp \
    visitingu.cpp

HEADERS += \
    adminpanel.h \
    indtra.h \
    intru.h \
    mainwindow.h \
    placea.h \
    placeu.h \
    sectiona.h \
    sectionu.h \
    studenta.h \
    studentu.h \
    trainer.h \
    traineru.h \
    userpanel.h \
    visitinga.h \
    visitingu.h

FORMS += \
    adminpanel.ui \
    indtra.ui \
    intru.ui \
    mainwindow.ui \
    placea.ui \
    placeu.ui \
    sectiona.ui \
    sectionu.ui \
    studenta.ui \
    studentu.ui \
    trainer.ui \
    traineru.ui \
    userpanel.ui \
    visitinga.ui \
    visitingu.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
