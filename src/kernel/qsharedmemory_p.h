/****************************************************************************
**
** Includes system files for shared memory
**
** Created : 020124
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QSHAREDMEMORY_P_H
#define QSHAREDMEMORY_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qapplication_qws.cpp and qgfxvnc_qws.cpp.  This header file may 
// change from version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

#if !defined (QT_QWS_NO_SHM)

#include <sys/types.h>
#include <sys/ipc.h>

class QSharedMemory {
public:
	QSharedMemory(){};
	QSharedMemory(int, QString, char c = 'Q');
	~QSharedMemory(){};

	bool create();
	void destroy();

	bool attach();
	void detach();

	void setPermissions(mode_t mode);
	int size();
	void * base() { return shmBase; };

private:
	void *shmBase;
	int shmSize;
	QString shmFile;
	char character;
#if defined(QT_POSIX_QSHM)
	int shmFD;
#else
	int shmId;
	key_t key;
	int idInitted;
#endif
};

#endif

#endif
