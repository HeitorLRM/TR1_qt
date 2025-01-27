QT       += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Media.cpp \
    R_App.cpp \
    R_Decoder.cpp \
    R_Demodulator.cpp \
    R_WorkerThread.cpp \
    R_main.cpp \
    Sync.cpp \
    T_App.cpp \
    T_Encoder.cpp \
    T_Modulator.cpp \
    T_WorkerThread.cpp \
    T_main.cpp \
    main.cpp

HEADERS += \
    Media.hpp \
    R_App.hpp \
    R_Decoder.hpp \
    R_Demodulator.hpp \
    R_WorkerThread.hpp \
    Sync.hpp \
    T_App.hpp \
    T_Encoder.hpp \
    T_Modulator.hpp \
    T_WorkerThread.hpp

FORMS += \
    R_App.ui \
    T_App.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TR1.pro.user
