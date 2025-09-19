QT       += core gui sql
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addbus.cpp \
    adminlogin.cpp \
    adminpage.cpp \
    confirmpage.cpp \
    editbus.cpp \
    getdetails.cpp \
    historypage.cpp \
    main.cpp \
    userlogin.cpp \
    userpage.cpp

HEADERS += \
    addbus.h \
    adminlogin.h \
    adminpage.h \
    confirmpage.h \
    editbus.h \
    getdetails.h \
    historypage.h \
    userlogin.h \
    userpage.h

FORMS += \
    addbus.ui \
    adminlogin.ui \
    adminpage.ui \
    confirmpage.ui \
    editbus.ui \
    getdetails.ui \
    historypage.ui \
    userlogin.ui \
    userpage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
