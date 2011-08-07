/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <qmainwindow.h>
#include <qstring.h>

class MyWidget : public QMainWindow
{
    Q_OBJECT

public:
    MyWidget( QWidget* parent=0, const char* name = 0 );

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent*);

private:
    static void initChoices(QWidget* parent);
};

#endif
