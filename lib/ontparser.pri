unix|win32: LIBS += -L$$OUT_PWD/../lib/ontparser/ -lontparser

INCLUDEPATH += $$PWD/../lib/ontparser
DEPENDPATH += $$PWD/../lib/ontparser

win32:!win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../lib/ontparser/ontparser.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$OUT_PWD/../lib/ontparser/libontparser.a

