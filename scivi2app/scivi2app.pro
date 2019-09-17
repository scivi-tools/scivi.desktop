QT += quick charts datavisualization
CONFIG += c++14
CONFIG -= debug_and_release_target

INCLUDEPATH += src/
TEMPLATE = app
TARGET = scivi2
PRECOMPILED_HEADER += src/precompiled_header.h
CONFIG += precompile_header

cross_compile: DEFINES += QT_EXTRA_FILE_SELECTOR=\\\"touch\\\"

android {
    QT += androidextras
}

DEFINES += QT_DEPRECATED_WARNINGS

include(../lib/ontparser.pri)

SOURCES += \
    src/main.cpp \
    src/editor/connectionhandler.cpp \
    src/editor/editor.cpp \
    src/editor/nodefactory.cpp \
    src/editor/typesstore.cpp \
    src/interpreter/interpreter.cpp \
    src/io/diagramwriter.cpp \
    src/knowledge/concept.cpp \
    src/knowledge/knowledgeservice.cpp \
    src/knowledge/ontology.cpp \
    src/knowledge/reader.cpp \
    src/knowledge/relation.cpp \
    src/palette/palettebuilder.cpp \
    src/palette/palettefilterproxy.cpp \
    src/palette/paletteitem.cpp \
    src/palette/palettemodel.cpp \
    src/plugin/pluginmodel.cpp \
    src/pluginmanager.cpp \
    src/sciviapplication.cpp \
    src/settings.cpp \
    src/statusbarlogger.cpp \
    src/widgets/zoompanview.cpp


HEADERS += \
    src/plugin/pluginmodel.h \
    src/pluginmanager.h \
    src/precompiled_header.h \
    src/editor/connectionhandler.h \
    src/editor/editor.h \
    src/editor/nodefactory.h \
    src/editor/typesstore.h \
    src/interpreter/interpreter.h \
    src/io/diagramwriter.h \
    src/knowledge/concept.h \
    src/knowledge/knowledgeservice.h \
    src/knowledge/ontology.h \
    src/knowledge/reader.h \
    src/knowledge/relation.h \
    src/knowledge/triplet.h \
    src/palette/palettebuilder.h \
    src/palette/palettefilterproxy.h \
    src/palette/paletteitem.h \
    src/palette/palettemodel.h \
    src/sciviapplication.h \
    src/settings.h \
    src/singleton.h \
    src/statusbarlogger.h \
    src/widgets/zoompanview.h

RESOURCES += resources/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += $$PWD/../scivi2core/

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#    copylibs.commands = $(COPY_DIR) $$shell_path($$PWD/../../../CppFilters) $$shell_path($$PWD/../../../Server/es/lib) $$OUT_PWD
#    first.depends = $(first) copylibs
#    export(first.depends)
#    export(copylibs.commands)
#    QMAKE_EXTRA_TARGETS += first copylibs

contains(ANDROID_TARGET_ARCH,armeabi-v7a)
{
    ANDROID_EXTRA_LIBS = \
        $$PWD/../scivi2core/lib/libscivi2core.so
    ANDROID_EXTRA_PLUGINS = \
        $$PWD/../../../CppFilters/
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    resources/qml/EdgeDelegate.qml \
    resources/qml/EditorPage.qml \
    resources/qml/main.qml \
    resources/qml/NodeDelegate.qml \
    resources/qml/NodeInspector.qml \
    resources/qml/Palette.qml \
    resources/qml/Parameter.qml \
    resources/qml/Socket.qml \
    resources/qml/VisualizationPage.qml \
    resources/icons/add_black.png \
    resources/icons/delete_black.png \
    resources/icons/ic_folder_open_black_48dp.png \
    resources/icons/ic_insert_drive_file_black_48dp.png \
    resources/icons/play_black.png \
    resources/icons/save_black.png \
    resources/icons/scivi_logo_website.png \
    resources/icons/settings_black.png


contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

unix|win32: LIBS += -L$$PWD/../scivi2core/lib -lscivi2core

INCLUDEPATH += $$PWD/../scivi2core/src
DEPENDPATH += $$PWD/../scivi2core/src

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../scivi2core/lib/scivi2core.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../scivi2core/lib/libscivi2core.a
