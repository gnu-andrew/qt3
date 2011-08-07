/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qmainwindow.h>

class HeaderToolTip;
class TableToolTip;
class QAssistantClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

public slots:
    void assistantSlot();

private:
    HeaderToolTip *horizontalTip;
    HeaderToolTip *verticalTip;    
    TableToolTip *cellTip;
    QAssistantClient *assistant;
};
