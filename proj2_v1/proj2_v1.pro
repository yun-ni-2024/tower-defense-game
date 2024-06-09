QT       += core gui multimediawidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17  # Use an appropriate C++ standard

# Emit warnings for deprecated Qt features
DEFINES += QT_DEPRECATED_WARNINGS

# Uncomment to disable deprecated APIs before Qt 6.0.0
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    main.cpp \
    Map.cpp \
    Enemy.cpp \
    Simulation.cpp \
    Defense.cpp \
    widget.cpp

HEADERS += \
    Function.h \
    Map.h \
    Enemy.h \
    Simulation.h \
    Defense.h \
    widget.h

FORMS += \
    widget.ui

# Deployment rules
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon/resource.qrc

CONFIG += resources_big
