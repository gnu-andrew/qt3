/****************************************************************************
**
** Declaration of the QLocalePrivate class
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QLOCALE_P_H
#define QLOCALE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//
//

#include <qstring.h>

struct QLocalePrivate
{
public:
    const QChar &decimal() const { return (QChar&)m_decimal; }
    const QChar &group() const { return (QChar&)m_group; }
    const QChar &list() const { return (QChar&)m_list; }
    const QChar &percent() const { return (QChar&)m_percent; }
    const QChar &zero() const { return (QChar&)m_zero; }
    QChar plus() const { return QChar('+'); }
    const QChar &minus() const { return (QChar&)m_minus; }
    const QChar &exponential() const { return (QChar&)m_exponential; }
    QString infinity() const;
    QString nan() const;

    Q_UINT32 languageId() const { return m_language_id; }
    Q_UINT32 countryId() const { return m_country_id; }

    bool isDigit(QChar d) const;

    enum GroupSeparatorMode {
    	FailOnGroupSeparators,
	ParseGroupSeparators
    };

    enum DoubleForm {
	DFExponent = 0,     	    	// %e or %E
	DFDecimal,  	    	    	// %f or %F
	DFSignificantDigits,	    	// %g or %G
	_DFMax = DFSignificantDigits
    };

    enum Flags {
	NoFlags	    	= 0,
	
	// These correspond to the options in a printf format string
	Alternate           = 0x01,
	ZeroPadded          = 0x02,
	LeftAdjusted        = 0x04,
	BlankBeforePositive = 0x08,
	AlwaysShowSign      = 0x10,
	ThousandsGroup      = 0x20,
	CapitalEorX	    = 0x40 // %x, %e, %f, %g vs. %X, %E, %F, %G
    };

    QString doubleToString(double d,
			   int precision = -1,
			   DoubleForm form = DFSignificantDigits,
			   int width = -1,
			   unsigned flags = NoFlags) const;
    QString longLongToString(Q_LLONG l, int precision = -1,
			     int base = 10,
			     int width = -1,
			     unsigned flags = NoFlags) const;
    QString unsLongLongToString(Q_ULLONG l, int precision = -1,
				int base = 10,
				int width = -1,
				unsigned flags = NoFlags) const;
    double stringToDouble(QString num, bool *ok, GroupSeparatorMode group_sep_mode) const;
    Q_LLONG stringToLongLong(QString num, int base, bool *ok, GroupSeparatorMode group_sep_mode) const;
    Q_ULLONG stringToUnsLongLong(QString num, int base, bool *ok, GroupSeparatorMode group_sep_mode) const;
    bool removeGroupSeparators(QString &num_str) const;
    bool numberToCLocale(QString &locale_num, GroupSeparatorMode group_sep_mode) const;

    Q_UINT32 m_language_id, m_country_id;

    Q_UINT16 m_decimal, m_group, m_list, m_percent,
	m_zero, m_minus, m_exponential;

    static const QString m_infinity;
    static const QString m_nan;
    static const QChar m_plus;

    static const char *systemLocaleName();
};

#endif
