#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T18:36:00
#
#-------------------------------------------------
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pygmy
TEMPLATE = app
CONFIG += c++11

RC_ICONS = resources/images/pygmy.ico

QMAKE_INFO_PLIST = $$_PRO_FILE_PWD_/resources/osx/Info.plist
ICON = resources/images/pygmy.icns

INCLUDEPATH += /usr/local/include /usr/local/include/freetype2

LIBS += -L/usr/local/lib -lftgl

SOURCES +=\
    src/gui/GlWidget.cpp \
    src/main.cpp \
    src/gui/MainWindow.cpp \
    src/core/NewickIO.cpp \
    src/utils/Colour.cpp \
    src/utils/Node.cpp \
    src/utils/Point.cpp \
    src/core/VisualLine.cpp \
    src/core/VisualMarker.cpp \
    src/core/VisualObject.cpp \
    src/core/VisualRect.cpp \
    src/core/VisualTree.cpp \
    src/glUtils/Font.cpp \
    src/core/State.cpp \
    src/core/MetadataInfo.cpp \
    src/utils/ParsimonyCalculator.cpp \
    src/gui/GlScrollWrapper.cpp \
    src/gui/GlWidgetBase.cpp \
    src/gui/GlWidgetOverview.cpp \
    src/gui/SimpleSearch.cpp \
    #src/gui/PreferencesDialog.cpp

HEADERS  += \
    src/gui/MainWindow.hpp \
    src/gui/GlWidget.hpp \
    src/core/NewickIO.hpp \
    src/utils/Tree.hpp \
    src/utils/TreeTools.hpp \
    src/core/NodePhylo.hpp \
    src/utils/Colour.hpp \
    src/utils/Node.hpp \
    src/utils/Point.hpp \
    src/core/DataTypes.hpp \
    src/utils/Common.hpp \
    src/utils/Error.hpp \
    src/core/VisualLine.hpp \
    src/core/VisualMarker.hpp \
    src/core/VisualObject.hpp \
    src/core/VisualRect.hpp \
    src/core/VisualTree.hpp \
    src/glUtils/ErrorGL.hpp \
    src/glUtils/Font.hpp \
    src/core/State.hpp \
    src/core/MetadataInfo.hpp \
    src/core/Filter.hpp \
    src/utils/ParsimonyCalculator.hpp \
    src/gui/GlScrollWrapper.hpp \
    src/gui/GlWidgetBase.hpp \
    src/gui/GlWidgetOverview.hpp \
    src/gui/SimpleSearch.hpp \
    src/core/TextSearch.hpp \
    #src/gui/PreferencesDialog.hpp

RESOURCES += \
    resources.qrc

FORMS += \
    src/gui/SimpleSearch.ui \
    #src/gui/PreferencesDialog.ui

