QT += core
QT -= gui
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle


SOURCES += main.cpp \
    ssl_server.cpp

LIBS += -lcrypto -lssl -lGL

HEADERS += \
    ssl_server.h
