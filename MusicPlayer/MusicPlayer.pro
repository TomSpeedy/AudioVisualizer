QT       += core gui multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioVisualizer.cpp \
    main.cpp \
    mainwindow.cpp \
    musicplayer.cpp \
    tracks.cpp

HEADERS += \
    audioVisualizer.h \
    fftw3.h \
    mainwindow.h \
    musicplayer.h \
    tracks.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    MusicPlayer_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64/' -lOpenAL32
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64/' -lOpenAL32
else:unix: LIBS += -L'C:/Program Files (x86)/OpenAL 1.1 SDK/libs/Win64/' -lOpenAL32
LIBS += "$$PWD/libfftw3-3.dll"

INCLUDEPATH += 'C:/Program Files (x86)/OpenAL 1.1 SDK/include'
DEPENDPATH += 'C:/Program Files (x86)/OpenAL 1.1 SDK/include'

