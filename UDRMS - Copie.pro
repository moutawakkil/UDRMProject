QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    UDRMS.cpp \
    addmenuitemdialog.cpp \
    addstudentdialog.cpp \
    addworkerdialog.cpp \
    assignstudentdialog.cpp \
    block.cpp \
    datamanage.cpp \
    dormitery.cpp \
    dormitoryworker.cpp \
    floor.cpp \
    main.cpp \
    person.cpp \
    reservemealdialog.cpp \
    restaurent.cpp \
    rooms.cpp \
    setupdialog.cpp \
    students.cpp \
    takemealdialog.cpp \
    transferstudentdialog.cpp

HEADERS += \
    Mealperiodutils.h \
    UDRMS.h \
    Workertypeutils.h \
    addmenuitemdialog.h \
    addstudentdialog.h \
    addworkerdialog.h \
    assignstudentdialog.h \
    block.h \
    datamanage.h \
    dormitery.h \
    dormitoryConfig.h \
    dormitoryworker.h \
    floor.h \
    person.h \
    reservemealdialog.h \
    restaurent.h \
    rooms.h \
    setupdialog.h \
    students.h \
    takemealdialog.h \
    transferstudentdialog.h

FORMS += \
    UDRMS.ui \
    setupdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
