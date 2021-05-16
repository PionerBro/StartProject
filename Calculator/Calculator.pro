QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calcitem.cpp \
    calcitem2delegate.cpp \
    calctablewidget.cpp \
    cellwidget.cpp \
    directorywidget.cpp \
    diritem.cpp \
    main.cpp \
    mainwindow.cpp \
    mydatabase.cpp \
    mydoublevalidator.cpp \
    myprintwidget.cpp \
    myprintwidgetdelegate.cpp \
    myprintwidgettable.cpp \
    mytreeitem.cpp \
    mytreemodel.cpp \
    printwidget.cpp

HEADERS += \
    calcitem.h \
    calcitem2delegate.h \
    calctablewidget.h \
    cellwidget.h \
    directorywidget.h \
    diritem.h \
    mainwindow.h \
    mydatabase.h \
    mydoublevalidator.h \
    myprintwidget.h \
    myprintwidgetdelegate.h \
    myprintwidgettable.h \
    mytreeitem.h \
    mytreemodel.h \
    printwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
