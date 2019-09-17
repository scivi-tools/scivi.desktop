TEMPLATE = subdirs

SUBDIRS += \
    lib/ontparser \
    scivi2core \
    scivi2app \

CONFIG += ordered

scivi2app.depends = ontparser scivi2core
