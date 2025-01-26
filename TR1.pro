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
    Rmain.cpp \
    Sync.cpp \
    T_App.cpp \
    T_Encoder.cpp \
    T_Modulator.cpp \
    Tmain.cpp \
    main.cpp

HEADERS += \
    Media.hpp \
    R_App.h \
    R_Decoder.hpp \
    R_Demodulator.hpp \
    Sync.hpp \
    T_App.h \
    T_Encoder.hpp \
    T_Modulator.hpp

FORMS += \
    R_App.ui \
    T_App.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TR1.pro.user
