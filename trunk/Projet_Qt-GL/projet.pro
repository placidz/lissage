HEADERS = glwidget.h \
    window.h \
    OutilsPGM.h
SOURCES = glwidget.cpp \
    main.cpp \
    window.cpp \
    OutilsPGM.cpp
QT += opengl

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
sources.files = $$SOURCES \
    $$HEADERS \
    $$RESOURCES \
    $$FORMS \
    hellogl.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/opengl/hellogl
INSTALLS += target \
    sources
