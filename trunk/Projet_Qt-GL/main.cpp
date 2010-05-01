#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QGLWidget>
#include <QTextCodec>

#include "window.h"
#include "glwidget.h"

class GLWidget;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Window wTools;
    wTools.show();
    return app.exec();
}
