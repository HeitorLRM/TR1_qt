QT       += core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CamadaFisica.cpp \
    CamadeEnlace.cpp \
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
    error_window.cpp \
    freq_window.cpp \
    main.cpp \
    preview_window.cpp \
    resolution_window.cpp

HEADERS += \
    CamadaEnlace.hpp \
    CamadaFisica.hpp \
    Media.hpp \
    R_App.hpp \
    R_Decoder.hpp \
    R_Demodulator.hpp \
    R_Settings.hpp \
    R_WorkerThread.hpp \
    Sync.hpp \
    T_App.hpp \
    T_Encoder.hpp \
    T_Modulator.hpp \
    T_Settings.hpp \
    T_WorkerThread.hpp \
    error_window.h \
    freq_window.h \
    preview_window.h \
    resolution_window.h

FORMS += \
    R_App.ui \
    T_App.ui \
    error_window.ui \
    freq_window.ui \
    preview_window.ui \
    resolution_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    TR1.pro.user
