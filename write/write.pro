TEMPLATE = app
TARGET = write
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lrt -pthread

SOURCES += \
    write.cpp



DEFINES += MMAP

#DEFINES += SHM_GET
