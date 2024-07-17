QT += core gui widgets

CONFIG += c++11

TARGET = CANAlgorithmUpdater
TEMPLATE = app

SOURCES += main.cpp \
           mainwindow.cpp

HEADERS += mainwindow.h

FORMS += mainwindow.ui

# Укажите путь к библиотеке и имя библиотеки
LIBS += -L"C:/Prog/PcanBacic/x64/VC_LIB" -lPCANBasic
