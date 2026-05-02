QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/combat/attackbox.cpp \
    src/combat/collision.cpp \
    src/combat/skill.cpp \
    src/core/gamewidget.cpp \
    src/core/mainwindow.cpp \
    src/entity/character.cpp \
    src/entity/enemy.cpp \
    src/entity/entity.cpp \
    src/entity/player.cpp \
    src/ui/hud.cpp \
    src/ui/startmenu.cpp

HEADERS += \
    src/combat/attackbox.h \
    src/combat/collision.h \
    src/combat/skill.h \
    src/core/gameconfig.h \
    src/core/gametypes.h \
    src/core/gamewidget.h \
    src/core/mainwindow.h \
    src/entity/character.h \
    src/entity/enemy.h \
    src/entity/entity.h \
    src/entity/player.h \
    src/ui/hud.h \
    src/ui/startmenu.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rescource.qrc
