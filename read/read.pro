TEMPLATE = app
TARGET = read
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lrt -pthread

SOURCES += \
    read.cpp
