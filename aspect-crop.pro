QT += widgets

PREFIX = /usr/local
export(PREFIX)

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS_RELEASE += -O2
CONFIG += c++17 release
HEADERS = imagewindow.h imageitem.h system.h
SOURCES = main.C imagewindow.C imageitem.C system.C

man.path = $$PREFIX/man/man1
man.files = aspect-crop.1
target.path = $$PREFIX/bin

INSTALLS += target man
