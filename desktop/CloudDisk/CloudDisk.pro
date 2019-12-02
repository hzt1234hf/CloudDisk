#-------------------------------------------------
#
# Project created by QtCreator 2019-11-12T17:01:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CloudDisk
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        class_obj/obj_base.cpp \
        class_obj/obj_file.cpp \
        class_obj/obj_folder.cpp \
        class_obj/obj_frame.cpp \
        class_obj/obj_transfer.cpp \
        class_obj/showpanel.cpp \
        class_obj/transferitem.cpp \
        class_obj/transferitemprocessingdelegate.cpp \
        class_obj/transferpanel.cpp \
        login.cpp \
        main.cpp \
        clouddiskwindow.cpp \
        serverconnect.cpp

HEADERS += \
        class_obj/comdef.h \
        class_obj/obj_base.h \
        class_obj/obj_file.h \
        class_obj/obj_folder.h \
        class_obj/obj_frame.h \
        class_obj/obj_transfer.h \
        class_obj/showpanel.h \
        class_obj/transferitem.h \
        class_obj/transferitemprocessingdelegate.h \
        class_obj/transferpanel.h \
        clouddiskwindow.h \
        login.h \
        serverconnect.h

FORMS += \
        clouddiskwindow.ui \
        login.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
