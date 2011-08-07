/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qwidget.h>
class QStyle;
class QListBox;
class QListBoxItem;
class QWidgetStack;

class GraphWidgetPrivate;

class GraphWidget : public QWidget 
{
    Q_OBJECT
public:
    GraphWidget( QWidget *parent=0, const char *name=0 );
    ~GraphWidget();

private slots:
    void shuffle();
    void setSpeed(int);
    
private:
    GraphWidgetPrivate* d;
};
