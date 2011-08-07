/****************************************************************************
**
** Implementation of cached Qt SQL result classes
**
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the sql module of the Qt GUI Toolkit.
**
** This file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the files LICENSE.GPL2
** and LICENSE.GPL3 included in the packaging of this file.
** Alternatively you may (at your option) use any later version
** of the GNU General Public License if such license has been
** publicly approved by Trolltech ASA (or its successors, if any)
** and the KDE Free Qt Foundation.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/.
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** This file may be used under the terms of the Q Public License as
** defined by Trolltech ASA and appearing in the file LICENSE.QPL
** included in the packaging of this file.  Licensees holding valid Qt
** Commercial licenses may use this file in accordance with the Qt
** Commercial License Agreement provided with the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#include "qsqlcachedresult.h"
#include <qdatetime.h>

#ifndef QT_NO_SQL
static const uint initial_cache_size = 128;

class QtSqlCachedResultPrivate
{
public:
    QtSqlCachedResultPrivate();
    bool seek(int i);
    void init(int count, bool fo);
    void cleanup();
    QtSqlCachedResult::RowCache* next();
    void revertLast();

    QtSqlCachedResult::RowsetCache *cache;
    QtSqlCachedResult::RowCache *current;
    int rowCacheEnd;
    int colCount;
    bool forwardOnly;
};

QtSqlCachedResultPrivate::QtSqlCachedResultPrivate():
    cache(0), current(0), rowCacheEnd(0), colCount(0), forwardOnly(FALSE)
{
}

void QtSqlCachedResultPrivate::cleanup()
{
    if (cache) {
	for (int i = 0; i < rowCacheEnd; ++i)
	    delete (*cache)[i];
	delete cache;
	cache = 0;
    }
    if (forwardOnly)
	delete current;
    current = 0;
    forwardOnly = FALSE;
    colCount = 0;
    rowCacheEnd = 0;
}

void QtSqlCachedResultPrivate::init(int count, bool fo)
{
    cleanup();
    forwardOnly = fo;
    colCount = count;
    if (fo)
	current = new QtSqlCachedResult::RowCache(count);
    else
	cache = new QtSqlCachedResult::RowsetCache(initial_cache_size);
}

QtSqlCachedResult::RowCache *QtSqlCachedResultPrivate::next()
{
    if (forwardOnly)
	return current;

    Q_ASSERT(cache);
    current = new QtSqlCachedResult::RowCache(colCount);
    if (rowCacheEnd == (int)cache->size())
	cache->resize(cache->size() * 2);
    cache->insert(rowCacheEnd++, current);
    return current;
}

bool QtSqlCachedResultPrivate::seek(int i)
{
    if (forwardOnly || i < 0)
	return FALSE;
    if (i >= rowCacheEnd)
	return FALSE;
    current = (*cache)[i];
    return TRUE;
}

void QtSqlCachedResultPrivate::revertLast()
{
    if (forwardOnly)
	return;
    --rowCacheEnd;
    delete current;
    current = 0;
}

//////////////

QtSqlCachedResult::QtSqlCachedResult(const QSqlDriver * db ): QSqlResult ( db )
{
    d = new QtSqlCachedResultPrivate();
}

QtSqlCachedResult::~QtSqlCachedResult()
{
    delete d;
}

void QtSqlCachedResult::init(int colCount)
{
    d->init(colCount, isForwardOnly());
}

bool QtSqlCachedResult::fetch(int i)
{
    if ((!isActive()) || (i < 0))
	return FALSE;
    if (at() == i)
	return TRUE;
    if (d->forwardOnly) {
	// speed hack - do not copy values if not needed
	if (at() > i || at() == QSql::AfterLast)
	    return FALSE;
	while(at() < i - 1) {
	    if (!gotoNext(0))
		return FALSE;
	    setAt(at() + 1);
	}
	if (!gotoNext(d->current))
	    return FALSE;
	setAt(at() + 1);
	return TRUE;
    }
    if (d->seek(i)) {
	setAt(i);
	return TRUE;
    }
    setAt(d->rowCacheEnd - 1);
    while (at() < i) {
	if (!cacheNext())
	    return FALSE;
    }
    return TRUE;
}

bool QtSqlCachedResult::fetchNext()
{
    if (d->seek(at() + 1)) {
	setAt(at() + 1);
	return TRUE;
    }
    return cacheNext();
}

bool QtSqlCachedResult::fetchPrev()
{
    return fetch(at() - 1);
}

bool QtSqlCachedResult::fetchFirst()
{
    if (d->forwardOnly && at() != QSql::BeforeFirst) {
	return FALSE;
    }
    if (d->seek(0)) {
	setAt(0);
	return TRUE;
    }
    return cacheNext();
}

bool QtSqlCachedResult::fetchLast()
{
    if (at() == QSql::AfterLast) {
	if (d->forwardOnly)
	    return FALSE;
	else
	    return fetch(d->rowCacheEnd - 1);
    }

    int i = at();
    while (fetchNext())
	i++; /* brute force */
    if (d->forwardOnly && at() == QSql::AfterLast) {
	setAt(i);
	return TRUE;
    } else {
	return fetch(d->rowCacheEnd - 1);
    }
}

QVariant QtSqlCachedResult::data(int i)
{
    if (!d->current || i >= (int)d->current->size() || i < 0)
	return QVariant();

    return (*d->current)[i];
}

bool QtSqlCachedResult::isNull(int i)
{
    if (!d->current || i >= (int)d->current->size() || i < 0)
	return TRUE;

    return (*d->current)[i].isNull();
}

void QtSqlCachedResult::cleanup()
{
    setAt(QSql::BeforeFirst);
    setActive(FALSE);
    d->cleanup();
}

bool QtSqlCachedResult::cacheNext()
{
    if (!gotoNext(d->next())) {
	d->revertLast();
	return FALSE;
    }
    setAt(at() + 1);
    return TRUE;
}

int QtSqlCachedResult::colCount() const
{
    return d->colCount;
}
#endif // QT_NO_SQL
