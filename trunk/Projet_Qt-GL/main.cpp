#include <QApplication>
#include <QTextCodec>

#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    Window window;
    window.show();
    return app.exec();
}
