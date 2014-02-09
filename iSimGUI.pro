#-------------------------------------------------
#
# Project created by QtCreator 2014-02-07T23:55:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iSimGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog/openfiledialog.cpp \
    io/filereader.cpp \
    util/json.cpp \
    util/UtilityFunctions.cpp \
    temporal/temporalindex.cpp \
    temporal/mesoscopic.cpp \
    preferencemanager.cpp \
    dialog/preferencedialog.cpp \
    io/dbio.cpp \
    temporal/agent.cpp \
    temporal/driver.cpp \
    temporal/busdriver.cpp \
    temporal/pedestrian.cpp \
    geospatial/multinode.cpp \
    geospatial/geospatialindex.cpp \
    viewcontroller.cpp \
    graphicsview/mapgraphicsview.cpp \
    geospatial/node.cpp \
    geospatial/uninode.cpp \
    geospatial/g_node.cpp \
    geospatial/link.cpp \
    geospatial/roadsegment.cpp \
    geospatial/lane.cpp \
    geospatial/busstop.cpp \
    geospatial/g_busstop.cpp \
    geospatial/g_lane.cpp \
    geospatial/g_segment.cpp

HEADERS  += mainwindow.h \
    dialog/openfiledialog.h \
    commondef.h \
    io/filereader.h \
    util/json.h \
    util/UtilityFunctions.h \
    temporal/temporalindex.h \
    temporal/mesoscopic.h \
    preferencemanager.h \
    dialog/preferencedialog.h \
    io/dbio.h \
    temporal/agent.h \
    temporal/driver.h \
    temporal/busdriver.h \
    temporal/pedestrian.h \
    geospatial/multinode.h \
    geospatial/geospatialindex.h \
    viewcontroller.h \
    graphicsview/mapgraphicsview.h \
    geospatial/node.h \
    geospatial/uninode.h \
    geospatial/g_node.h \
    geospatial/link.h \
    geospatial/roadsegment.h \
    geospatial/lane.h \
    geospatial/busstop.h \
    geospatial/g_busstop.h \
    geospatial/g_lane.h \
    geospatial/g_segment.h

FORMS    += mainwindow.ui

RESOURCES += \
    icons.qrc
