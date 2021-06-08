QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calccalculationsitem.cpp \
    calccalculationsitemmodel.cpp \
    calccalculationsmodelitem.cpp \
    calcitem.cpp \
    calcitem2delegate.cpp \
    calctablewidget.cpp \
    calculatoracceptdatadialog.cpp \
    calculatorcalculationsdialog.cpp \
    calculatorcalculationstreemodel.cpp \
    calculatordatabase.cpp \
    calculatorhistoryitem.cpp \
    calculatorhistorymodel.cpp \
    calculatormaterialsdelegate.cpp \
    calculatormaterialsdialog.cpp \
    calculatormaterialshistorydialog.cpp \
    calculatormaterialsitem.cpp \
    calculatormaterialstreemodel.cpp \
    calculatortreeitem.cpp \
    calculatortreemodel.cpp \
    calcultatorunitstablemodel.cpp \
    calcwidget.cpp \
    cellwidget.cpp \
    directorywidget.cpp \
    diritem.cpp \
    main.cpp \
    mainwindow.cpp \
    mydatabase.cpp \
    mydoublevalidator.cpp \
    myitemdelegate.cpp \
    myprintwidget.cpp \
    myprintwidgetdelegate.cpp \
    myprintwidgettable.cpp \
    mytreeitem.cpp \
    mytreemodel.cpp \
    printwidget.cpp

HEADERS += \
    calccalculationsitem.h \
    calccalculationsitemmodel.h \
    calccalculationsmodelitem.h \
    calcitem.h \
    calcitem2delegate.h \
    calctablewidget.h \
    calculatoracceptdatadialog.h \
    calculatorcalculationsdialog.h \
    calculatorcalculationstreemodel.h \
    calculatordatabase.h \
    calculatorhistoryitem.h \
    calculatorhistorymodel.h \
    calculatormaterialsdelegate.h \
    calculatormaterialsdialog.h \
    calculatormaterialshistorydialog.h \
    calculatormaterialsitem.h \
    calculatormaterialstreemodel.h \
    calculatortreeitem.h \
    calculatortreemodel.h \
    calcultatorunitstablemodel.h \
    calcwidget.h \
    cellwidget.h \
    directorywidget.h \
    diritem.h \
    mainwindow.h \
    mydatabase.h \
    mydoublevalidator.h \
    myitemdelegate.h \
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
