QT += core
QT -= gui
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += \
    main.cpp

LIBS += -lcrypto -lssl
