/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#ifndef STRUCTUREPARSER_H
#define STRUCTUREPARSER_H   

#include <qxml.h>

class QString;

class StructureParser : public QXmlDefaultHandler
{
public:
    bool startDocument();
    bool startElement( const QString&, const QString&, const QString& , 
                       const QXmlAttributes& );
    bool endElement( const QString&, const QString&, const QString& );

private:
    QString indent;
};                   

#endif 
