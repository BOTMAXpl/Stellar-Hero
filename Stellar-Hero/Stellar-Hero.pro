TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        gracz.cpp \
        main.cpp \
        obiekt.cpp \
        pocisk.cpp

INCLUDEPATH += "C:/SFML-2.5.1/include"
LIBS += -L"C:/SFML-2.5.1/lib"

# Linkowanie modułów
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network

HEADERS += \
    gracz.h \
    obiekt.h \
    pocisk.h
