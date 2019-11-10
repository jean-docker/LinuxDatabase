TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    dbrecord.cpp \
    bplustree.cpp \
    work.cpp

HEADERS += \
    struct.h \
    dbrecord.h \
    bplustree.h \
    work.h
