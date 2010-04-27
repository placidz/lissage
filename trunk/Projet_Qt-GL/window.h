/****************************************************************************
 **
 ** Copyright (C) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 **
 ** This file is part of the documentation of the Qt Toolkit.
 **
 ** Licensees holding a valid Qt License Agreement may use this file in
 ** accordance with the rights, responsibilities and obligations
 ** contained therein.  Please consult your licensing agreement or
 ** contact qt-sales@nokia.com if any conditions of this licensing
 ** agreement are not clear to you.
 **
 ** Further information about Qt licensing is available at:
 ** http://trolltech.com/products/appdev/licensing.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGLWidget>

extern "C"
{
#include "OutilsPGM.h"
}

class QSlider;
class QComboBox;
class QPushButton;
class QLabel;

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

signals:
    void signal_EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n);

public slots:
    void slot_clicked();

private:
    GLWidget *glWidgetOriginale;
    GLWidget *glWidgetResultat;
    QComboBox *combobox;
    QPushButton *button;
    QLabel *labelImgOriginale;
    QLabel *labelImgFiltree;

    FiltreLineaire fLaplacien;
};

#endif
