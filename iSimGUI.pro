#-------------------------------------------------
#
# Project created by QtCreator 2014-02-07T23:55:45
#
#-------------------------------------------------

QT       += core gui sql
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
    geospatial/g_segment.cpp \
    geospatial/g_uninode.cpp \
    geospatial/g_multinode.cpp \
    temporal/mesoscopicdata.cpp \
    temporal/microscopicdata.cpp \
    temporal/mesoscopicdatamem.cpp \
    temporal/mesoscopicdatadb.cpp \
    temporal/microscopicdatamem.cpp \
    temporal/microscopicdatadb.cpp \
    rstart_tree/agenttree.cpp \
    temporal/g_agent.cpp \
    temporal/g_driver.cpp \
    temporal/agentlist.cpp \
    temporal/g_pedestrian.cpp \
    temporal/g_busdriver.cpp \
    geospatial/crossing.cpp \
    geospatial/g_crossing.cpp \
    geospatial/trafficsignal.cpp \
    geospatial/g_trafficsignal.cpp \
    geospatial/laneconnector.cpp \
    geospatial/g_laneconnector.cpp \
    io/dbhandler.cpp

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
    geospatial/g_segment.h \
    geospatial/g_uninode.h \
    geospatial/g_multinode.h \
    temporal/mesoscopicdata.h \
    temporal/microscopicdata.h \
    temporal/mesoscopicdatamem.h \
    temporal/mesoscopicdatadb.h \
    temporal/microscopicdatamem.h \
    temporal/microscopicdatadb.h \
    rstart_tree/RStarBoundingBox.h \
    rstart_tree/RStarTree.h \
    rstart_tree/RStarVisitor.h \
    rstart_tree/agenttree.h \
    temporal/g_agent.h \
    temporal/g_driver.h \
    temporal/agentlist.h \
    temporal/g_pedestrian.h \
    temporal/g_busdriver.h \
    geospatial/crossing.h \
    geospatial/g_crossing.h \
    geospatial/trafficsignal.h \
    geospatial/g_trafficsignal.h \
    temporal/phasedata.h \
    geospatial/laneconnector.h \
    geospatial/g_laneconnector.h \
    io/dbhandler.h

FORMS    += mainwindow.ui \
    dialog/preferencedialog.ui

RESOURCES += \
    icons.qrc
