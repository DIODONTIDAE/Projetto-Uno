TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    ssl_server.cpp

LIBS += -lcrypto -lssl

HEADERS += \
    ssl_server.h
