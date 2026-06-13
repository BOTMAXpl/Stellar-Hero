TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        atak_obszarowy.cpp \
        boss.cpp \
        dzialko.cpp \
        gracz.cpp \
        laser_wroga.cpp \
        main.cpp \
        obiekt.cpp \
        ostrzezenie.cpp \
        pocisk.cpp \
        pocisk_wroga.cpp \
        portal.cpp \
        wrog.cpp

INCLUDEPATH += "C:/SFML-2.5.1/include"
LIBS += -L"C:/SFML-2.5.1/lib"

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

HEADERS += \
    atak_obszarowy.h \
    boss.h \
    dzialko.h \
    gracz.h \
    laser_wroga.h \
    obiekt.h \
    ostrzezenie.h \
    pocisk.h \
    pocisk_wroga.h \
    portal.h \
    portal_koncowy.h \
    wrog.h
