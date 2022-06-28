QT       += core gui network
 QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ErrorWindow.cpp \
    clientwindow.cpp \
    connectwindow.cpp \
    firstwindow.cpp \
    location.cpp \
    main.cpp \
    mainscene.cpp \
    mainwindow.cpp \
    networkdata.cpp \
    networkserver.cpp \
    networksocket.cpp \
    rankinglist.cpp \
    roomwindow.cpp \
    secondwindow.cpp \
    serverwindow.cpp \
    stone.cpp \
    winwindow.cpp

HEADERS += \
    ErrorWindow.h \
    clientwindow.h \
    connectwindow.h \
    firstwindow.h \
    location.h \
    mainscene.h \
    mainwindow.h \
    networkdata.h \
    networkserver.h \
    networksocket.h \
    rankinglist.h \
    roomwindow.h \
    secondwindow.h \
    serverwindow.h \
    stone.h \
    winwindow.h

FORMS += \
    ErrorWindow.ui \
    clientwindow.ui \
    connectwindow.ui \
    firstwindow.ui \
    mainscene.ui \
    roomwindow.ui \
    secondwindow.ui \
    serverwindow.ui \
    winwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc \
    res.qrc
