/****************************************************************************
**
** Definition of MakefileGenerator class.
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of qmake.
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
#ifndef __MAKEFILE_H__
#define __MAKEFILE_H__

#include "option.h"
#include "project.h"
#include <qtextstream.h>

#ifdef Q_OS_WIN32
#define QT_POPEN _popen
#else
#define QT_POPEN popen
#endif

class MakefileGenerator
{
    QString spec;
    bool init_opath_already, init_already, moc_aware, no_io;
    QStringList createObjectList(const QString &var);
    QString build_args();
    QString dependencyKey(const QString &file) const;
    QMap<QString, bool> depProcessed;
    QMap<QString, QString> depHeuristics, fileFixed;
    QMap<QString, QString> mocablesToMOC, mocablesFromMOC;
    QMap<QString, QStringList> depends;

protected:
    void writeObj(QTextStream &, const QString &obj, const QString &src);
    void writeUicSrc(QTextStream &, const QString &ui);
    void writeMocObj(QTextStream &, const QString &obj, const QString &src);
    void writeMocSrc(QTextStream &, const QString &src);
    void writeLexSrc(QTextStream &, const QString &lex);
    void writeYaccSrc(QTextStream &, const QString &yac);
    void writeInstalls(QTextStream &t, const QString &installs);
    void writeImageObj(QTextStream &t, const QString &obj);
    void writeImageSrc(QTextStream &t, const QString &images);

protected:

    QMakeProject *project;

    class MakefileDependDir {
    public:
	MakefileDependDir(const QString &r, const QString &l) : real_dir(r), local_dir(l) { }
	QString real_dir, local_dir;
    };
    bool generateDependencies(QPtrList<MakefileDependDir> &dirs, const QString &x, bool recurse);

    QString buildArgs();

    QString specdir();
    QString cleanFilePath(const QString &file) const;
    bool generateMocList(const QString &fn);

    QString findMocSource(const QString &moc_file) const;
    QString findMocDestination(const QString &src_file) const;
    virtual QStringList &findDependencies(const QString &file);

    void setNoIO(bool o);
    bool noIO() const;

    void setMocAware(bool o);
    bool mocAware() const;
    void logicWarn(const QString &, const QString &);

    virtual bool doDepends() const { return Option::mkfile::do_deps; }
    bool writeHeader(QTextStream &);
    virtual bool writeMakefile(QTextStream &);
    virtual bool writeMakeQmake(QTextStream &);
    void initOutPaths();
    virtual void init();

    //for cross-platform dependent directories
    virtual void usePlatformDir();

    //for installs
    virtual QString defaultInstall(const QString &);

    //for prl
    bool processPrlFile(QString &);
    virtual void processPrlVariable(const QString &, const QStringList &);
    virtual void processPrlFiles();
    virtual void writePrlFile(QTextStream &);

    //make sure libraries are found
    virtual bool findLibraries();
    virtual QString findDependency(const QString &);

    void setProcessedDependencies(const QString &file, bool b);
    bool processedDependencies(const QString &file);

    virtual QString var(const QString &var);
    QString varGlue(const QString &var, const QString &before, const QString &glue, const QString &after);
    QString varList(const QString &var);
    QString val(const QStringList &varList);
    QString valGlue(const QStringList &varList, const QString &before, const QString &glue, const QString &after);
    QString valList(const QStringList &varList);


    QString fileFixify(const QString& file, const QString &out_dir=QString::null, 
		       const QString &in_dir=QString::null, bool force_fix=FALSE, bool canon=TRUE) const;
    QStringList fileFixify(const QStringList& files, const QString &out_dir=QString::null, 
			   const QString &in_dir=QString::null, bool force_fix=FALSE, bool canon=TRUE) const;
public:
    MakefileGenerator(QMakeProject *p);
    virtual ~MakefileGenerator();

    static MakefileGenerator *create(QMakeProject *);
    virtual bool write();
    virtual bool openOutput(QFile &) const;
};

inline QString MakefileGenerator::findMocSource(const QString &moc_file) const
{
    QString tmp = cleanFilePath(moc_file);
    if (mocablesFromMOC.contains(tmp))
	return mocablesFromMOC[tmp];
    else
	return QString("");
}

inline QString MakefileGenerator::findMocDestination(const QString &src_file) const
{
    QString tmp = cleanFilePath(src_file);
    if (mocablesToMOC.contains(tmp))
	return mocablesToMOC[tmp];
    else
	return QString("");
}

inline void MakefileGenerator::setMocAware(bool o)
{ moc_aware = o; }

inline bool MakefileGenerator::mocAware() const
{ return moc_aware; }

inline void MakefileGenerator::setNoIO(bool o)
{ no_io = o; }

inline bool MakefileGenerator::noIO() const
{ return no_io; }

inline QString MakefileGenerator::defaultInstall(const QString &)
{ return QString(""); }

inline bool MakefileGenerator::findLibraries()
{ return TRUE; }

inline QString MakefileGenerator::findDependency(const QString &)
{ return QString(""); }

inline MakefileGenerator::~MakefileGenerator()
{ }

QString mkdir_p_asstring(const QString &dir);

#endif /* __MAKEFILE_H__ */
