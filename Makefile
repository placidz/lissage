#############################################################################
# Makefile for building: projet_lissage
# Generated by qmake (2.01a) (Qt 4.6.2) on: lun. mai 3 10:54:17 2010
# Project:  projet_lissage.pro
# Template: app
# Command: /usr/bin/qmake -unix -o Makefile projet_lissage.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG -DQT_OPENGL_LIB -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/share/qt4/mkspecs/linux-g++ -I. -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui -I/usr/include/qt4/QtOpenGL -I/usr/include/qt4 -I. -Iinclude -I/usr/X11R6/include -I.
LINK          = g++
LFLAGS        = -Wl,-O1
LIBS          = $(SUBLIBS)  -L/usr/lib -L/usr/X11R6/lib -lQtOpenGL -lQtGui -lQtCore -lGLU -lGL -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		source/glgraphique.cpp \
		source/glwidget.cpp \
		source/OutilsPGM.cpp \
		source/window.cpp moc_glgraphique.cpp \
		moc_glwidget.cpp \
		moc_window.cpp
OBJECTS       = main.o \
		glgraphique.o \
		glwidget.o \
		OutilsPGM.o \
		window.o \
		moc_glgraphique.o \
		moc_glwidget.o \
		moc_window.o
DIST          = /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		projet_lissage.pro
QMAKE_TARGET  = projet_lissage
DESTDIR       = 
TARGET        = projet_lissage

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: projet_lissage.pro  /usr/share/qt4/mkspecs/linux-g++/qmake.conf /usr/share/qt4/mkspecs/common/g++.conf \
		/usr/share/qt4/mkspecs/common/unix.conf \
		/usr/share/qt4/mkspecs/common/linux.conf \
		/usr/share/qt4/mkspecs/qconfig.pri \
		/usr/share/qt4/mkspecs/features/qt_functions.prf \
		/usr/share/qt4/mkspecs/features/qt_config.prf \
		/usr/share/qt4/mkspecs/features/exclusive_builds.prf \
		/usr/share/qt4/mkspecs/features/default_pre.prf \
		/usr/share/qt4/mkspecs/features/release.prf \
		/usr/share/qt4/mkspecs/features/default_post.prf \
		/usr/share/qt4/mkspecs/features/warn_on.prf \
		/usr/share/qt4/mkspecs/features/qt.prf \
		/usr/share/qt4/mkspecs/features/unix/opengl.prf \
		/usr/share/qt4/mkspecs/features/unix/thread.prf \
		/usr/share/qt4/mkspecs/features/moc.prf \
		/usr/share/qt4/mkspecs/features/resources.prf \
		/usr/share/qt4/mkspecs/features/uic.prf \
		/usr/share/qt4/mkspecs/features/yacc.prf \
		/usr/share/qt4/mkspecs/features/lex.prf \
		/usr/share/qt4/mkspecs/features/include_source_dir.prf \
		/usr/lib/libQtOpenGL.prl \
		/usr/lib/libQtGui.prl \
		/usr/lib/libQtCore.prl
	$(QMAKE) -unix -o Makefile projet_lissage.pro
/usr/share/qt4/mkspecs/common/g++.conf:
/usr/share/qt4/mkspecs/common/unix.conf:
/usr/share/qt4/mkspecs/common/linux.conf:
/usr/share/qt4/mkspecs/qconfig.pri:
/usr/share/qt4/mkspecs/features/qt_functions.prf:
/usr/share/qt4/mkspecs/features/qt_config.prf:
/usr/share/qt4/mkspecs/features/exclusive_builds.prf:
/usr/share/qt4/mkspecs/features/default_pre.prf:
/usr/share/qt4/mkspecs/features/release.prf:
/usr/share/qt4/mkspecs/features/default_post.prf:
/usr/share/qt4/mkspecs/features/warn_on.prf:
/usr/share/qt4/mkspecs/features/qt.prf:
/usr/share/qt4/mkspecs/features/unix/opengl.prf:
/usr/share/qt4/mkspecs/features/unix/thread.prf:
/usr/share/qt4/mkspecs/features/moc.prf:
/usr/share/qt4/mkspecs/features/resources.prf:
/usr/share/qt4/mkspecs/features/uic.prf:
/usr/share/qt4/mkspecs/features/yacc.prf:
/usr/share/qt4/mkspecs/features/lex.prf:
/usr/share/qt4/mkspecs/features/include_source_dir.prf:
/usr/lib/libQtOpenGL.prl:
/usr/lib/libQtGui.prl:
/usr/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -unix -o Makefile projet_lissage.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/projet_lissage1.0.0 || $(MKDIR) .tmp/projet_lissage1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/projet_lissage1.0.0/ && $(COPY_FILE) --parents include/glgraphique.h include/glwidget.h include/OutilsPGM.h include/window.h .tmp/projet_lissage1.0.0/ && $(COPY_FILE) --parents main.cpp source/glgraphique.cpp source/glwidget.cpp source/OutilsPGM.cpp source/window.cpp .tmp/projet_lissage1.0.0/ && (cd `dirname .tmp/projet_lissage1.0.0` && $(TAR) projet_lissage1.0.0.tar projet_lissage1.0.0 && $(COMPRESS) projet_lissage1.0.0.tar) && $(MOVE) `dirname .tmp/projet_lissage1.0.0`/projet_lissage1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/projet_lissage1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_glgraphique.cpp moc_glwidget.cpp moc_window.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_glgraphique.cpp moc_glwidget.cpp moc_window.cpp
moc_glgraphique.cpp: include/glwidget.h \
		include/glgraphique.h \
		include/OutilsPGM.h \
		include/glgraphique.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) include/glgraphique.h -o moc_glgraphique.cpp

moc_glwidget.cpp: include/glgraphique.h \
		include/glwidget.h \
		include/OutilsPGM.h \
		include/glwidget.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) include/glwidget.h -o moc_glwidget.cpp

moc_window.cpp: include/OutilsPGM.h \
		include/window.h
	/usr/bin/moc-qt4 $(DEFINES) $(INCPATH) include/window.h -o moc_window.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp include/window.h \
		include/OutilsPGM.h \
		include/glwidget.h \
		include/glgraphique.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

glgraphique.o: source/glgraphique.cpp include/glgraphique.h \
		include/glwidget.h \
		include/OutilsPGM.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o glgraphique.o source/glgraphique.cpp

glwidget.o: source/glwidget.cpp include/glwidget.h \
		include/glgraphique.h \
		include/OutilsPGM.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o glwidget.o source/glwidget.cpp

OutilsPGM.o: source/OutilsPGM.cpp include/OutilsPGM.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o OutilsPGM.o source/OutilsPGM.cpp

window.o: source/window.cpp include/glwidget.h \
		include/glgraphique.h \
		include/OutilsPGM.h \
		include/window.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o window.o source/window.cpp

moc_glgraphique.o: moc_glgraphique.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_glgraphique.o moc_glgraphique.cpp

moc_glwidget.o: moc_glwidget.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_glwidget.o moc_glwidget.cpp

moc_window.o: moc_window.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_window.o moc_window.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
