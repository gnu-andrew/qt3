/****************************************************************************
**
** Implementation of MakefileGenerator class.
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

#include "makefile.h"
#include "option.h"
#include "meta.h"
#include <qdir.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qdict.h>
#if defined(Q_OS_UNIX)
#include <unistd.h>
#else
#include <io.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

// Well, Windows doesn't have this, so here's the macro
#ifndef S_ISDIR
#define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#endif
#define QMAKE_EOL(x) (x == '\r' || x == '\n')

QString mkdir_p_asstring(const QString &dir)
{
    QString ret =  "@$(CHK_DIR_EXISTS) \"" + dir + "\" ";
    if(Option::target_mode == Option::TARG_WIN_MODE)
	ret += "$(MKDIR)";
    else
	ret += "|| $(MKDIR)";
    ret += " \"" + dir + "\"";
    return ret;
}

static bool createDir(const QString& fullPath)
{
    if(QFile::exists(fullPath))
	return FALSE;
    QDir dirTmp;
    bool ret = TRUE;
    QString pathComponent, tmpPath;
    QStringList hierarchy = QStringList::split(QString(Option::dir_sep), fullPath, TRUE);
    for(QStringList::Iterator it = hierarchy.begin(); it != hierarchy.end(); ++it) {
	pathComponent = *it + QDir::separator();
	tmpPath += pathComponent;
	if(!dirTmp.mkdir(tmpPath)) {
	    ret = FALSE;
//	    break;
	}
    }
    return ret;
}


MakefileGenerator::MakefileGenerator(QMakeProject *p) : init_opath_already(FALSE),
							init_already(FALSE), moc_aware(FALSE),
							no_io(FALSE), project(p)
{
}

static char *gimme_buffer(off_t s)
{
    static char *big_buffer = NULL;
    static int big_buffer_size = 0;
    if(!big_buffer || big_buffer_size < s)
	big_buffer = (char *)realloc(big_buffer, s);
    return big_buffer;
}

bool
MakefileGenerator::generateMocList(const QString &fn_target)
{
    if(!findMocDestination(fn_target).isEmpty())
	return TRUE;

    QString fn_local = Option::fixPathToLocalOS(fileFixify(fn_target, QDir::currentDirPath(), Option::output_dir));

    int file = open(fn_local.latin1(), O_RDONLY);
    if(file == -1)
	return FALSE;

    struct stat fst;
    if(fstat(file, &fst) || S_ISDIR(fst.st_mode))
	return FALSE; //shouldn't happen
    char *big_buffer = gimme_buffer(fst.st_size);

    int total_size_read;
    for(int have_read = total_size_read = 0;
	(have_read = read(file, big_buffer + total_size_read,
			  fst.st_size - total_size_read));
	total_size_read += have_read);
    close(file);

    bool ignore_qobject = FALSE;
    int line_count = 1;
 /* qmake ignore Q_OBJECT */
#define COMP_LEN 8 //strlen("Q_OBJECT")
#define OBJ_LEN 8 //strlen("Q_OBJECT")
#define DIS_LEN 10 //strlen("Q_DISPATCH")
    int x;
    for(x = 0; x < (total_size_read-COMP_LEN); x++) {
	if(*(big_buffer + x) == '/') {
	    x++;
	    if(total_size_read >= x) {
		if(*(big_buffer + x) == '/') { //c++ style comment
		    for( ;x < total_size_read && !QMAKE_EOL(*(big_buffer + x)); x++);
		    line_count++;
		} else if(*(big_buffer + x) == '*') { //c style comment
		    for( ;x < total_size_read; x++) {
			if(*(big_buffer + x) == 't' || *(big_buffer + x) == 'q') { //ignore
			    if(total_size_read >= (x + 20)) {
				if(!strncmp(big_buffer + x + 1, "make ignore Q_OBJECT", 20)) {
				    debug_msg(2, "Mocgen: %s:%d Found \"qmake ignore Q_OBJECT\"",
					      fn_target.latin1(), line_count);
				    x += 20;
				    ignore_qobject = TRUE;
				}
			    }
			} else if(*(big_buffer + x) == '*') {
			    if(total_size_read >= (x+1) && *(big_buffer + (x+1)) == '/') {
				x += 2;
				break;
			    }
			} else if(QMAKE_EOL(*(big_buffer + x))) {
			    line_count++;
			}
		    }
		}
	    }
	}
#define SYMBOL_CHAR(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z') || \
			(x <= '0' && x >= '9') || x == '_')

	bool interesting = *(big_buffer+x) == 'Q' && (!strncmp(big_buffer+x, "Q_OBJECT", OBJ_LEN) ||
						      !strncmp(big_buffer+x, "Q_DISPATCH", DIS_LEN));
	if(interesting) {
	    int len = 0;
	    if(!strncmp(big_buffer+x, "Q_OBJECT", OBJ_LEN)) {
		if(ignore_qobject) {
		    debug_msg(2, "Mocgen: %s:%d Ignoring Q_OBJECT", fn_target.latin1(), line_count);
		    interesting = FALSE;
		}
		len=OBJ_LEN;
	    } else if(!strncmp(big_buffer+x, "Q_DISPATCH", DIS_LEN)) {
		len=DIS_LEN;
	    }
	    if(SYMBOL_CHAR(*(big_buffer+x+len)))
		interesting = FALSE;
	    if(interesting) {
		*(big_buffer+x+len) = '\0';
		debug_msg(2, "Mocgen: %s:%d Found MOC symbol %s", fn_target.latin1(),
			  line_count, big_buffer+x);

		int ext_pos = fn_target.findRev('.');
		int ext_len = fn_target.length() - ext_pos;
		int dir_pos =  fn_target.findRev(Option::dir_sep, ext_pos);
		QString mocFile;
		if(!project->isEmpty("MOC_DIR"))
		    mocFile = project->first("MOC_DIR");
		else if(dir_pos != -1)
		    mocFile = fn_target.left(dir_pos+1);

		bool cpp_ext = FALSE;
		for(QStringList::Iterator cppit = Option::cpp_ext.begin();
		    cppit != Option::cpp_ext.end(); ++cppit) {
		    if((cpp_ext = (fn_target.right(ext_len) == (*cppit))))
			break;
		}
		if(cpp_ext) {
		    mocFile += Option::cpp_moc_mod + fn_target.mid(dir_pos+1, ext_pos - dir_pos-1) + Option::cpp_moc_ext;
		    project->variables()["_SRCMOC"].append(mocFile);
		} else if(project->variables()["HEADERS"].findIndex(fn_target) != -1) {
		    for(QStringList::Iterator hit = Option::h_ext.begin();
			hit != Option::h_ext.end(); ++hit) {
			if((fn_target.right(ext_len) == (*hit))) {
			    mocFile += Option::h_moc_mod + fn_target.mid(dir_pos+1, ext_pos - dir_pos-1) +
				       Option::h_moc_ext;
			    logicWarn(mocFile, "SOURCES");
			    project->variables()["_HDRMOC"].append(mocFile);
			    break;
			}
		    }
		}

		if(!mocFile.isEmpty()) {
		    mocFile = Option::fixPathToTargetOS(mocFile);
		    mocablesToMOC[cleanFilePath(fn_target)] = mocFile;
		    mocablesFromMOC[cleanFilePath(mocFile)] = fn_target;
		}
		break;
	    }
	}

	while(x < total_size_read && SYMBOL_CHAR(*(big_buffer+x)))
	    x++;
	if(QMAKE_EOL(*(big_buffer+x)))
	    line_count++;
    }
#undef OBJ_LEN
#undef DIS_LEN
    return TRUE;
}

bool
MakefileGenerator::generateDependencies(QPtrList<MakefileDependDir> &dirs, const QString &f, bool recurse)
{
    if(processedDependencies(f))
	return TRUE;
    setProcessedDependencies(f, TRUE);

    QStringList &fndeps = findDependencies(f);
    QString fn = fileFixify(f, QDir::currentDirPath(), Option::output_dir);
    fn = Option::fixPathToLocalOS(fn, FALSE);
    QString fix_env_fn = Option::fixPathToLocalOS(fn);
    int file = open(fix_env_fn.latin1(), O_RDONLY);
    if(file == -1)
	return FALSE;
    struct stat fst;
    if(fstat(file, &fst) || S_ISDIR(fst.st_mode))
	return FALSE; //shouldn't happen

    QString fndir, fix_env_fndir;
    int dl = fn.findRev(Option::dir_sep);
    if(dl != -1)
	fndir = fn.left(dl+1);
    dl = fix_env_fn.findRev(Option::dir_sep);
    if(dl != -1)
	fix_env_fndir = fix_env_fn.left(dl + 1);

    int line_count = 1;
    char *big_buffer = gimme_buffer(fst.st_size);

    int total_size_read;
    for(int have_read = total_size_read = 0;
	(have_read = read(file, big_buffer + total_size_read,
			  fst.st_size - total_size_read));
	total_size_read += have_read);
    close(file);

    bool ui_file = fn.endsWith(Option::ui_ext);
    for(int x = 0; x < total_size_read; x++) {
	QStringList *outdeps=&fndeps;
	QString inc;
	if(!ui_file) {
	    if(*(big_buffer + x) == '/') {
		x++;
		if(total_size_read >= x) {
		    if(*(big_buffer + x) == '/') { //c++ style comment
			for( ; x < total_size_read && !QMAKE_EOL(*(big_buffer + x)); x++);
		    } else if(*(big_buffer + x) == '*') { //c style comment
			for( ; x < total_size_read; x++) {
			    if(*(big_buffer + x) == '*') {
				if(total_size_read >= (x+1) && *(big_buffer + (x+1)) == '/') {
				    x += 2;
				    break;
				}
			    } else if(QMAKE_EOL(*(big_buffer + x))) {
				line_count++;
			    }
			}
		    }
		}
	    }
	    while(x < total_size_read && //Skip spaces
		  (*(big_buffer+x) == ' ' || *(big_buffer+x) == '\t'))
		x++;
	    if(*(big_buffer + x) == '#') {
		x++;
		while(x < total_size_read && //Skip spaces after hash
		      (*(big_buffer+x) == ' ' || *(big_buffer+x) == '\t'))
		    x++;
		if(total_size_read >= x + 8 && !strncmp(big_buffer + x, "include", 7) &&
		   (*(big_buffer + x + 7) == ' ' || *(big_buffer + x + 7) == '\t' ||
		    *(big_buffer + x + 7) == '<' || *(big_buffer + x + 7) == '"')) {
		    for(x+=7; //skip spaces after keyword
			x < total_size_read && (*(big_buffer+x) == ' ' || *(big_buffer+x) == '\t');
			x++);
		    char term = *(big_buffer + x);
		    if(term == '"');
		    else if(term == '<')
			term = '>';
		    else
			continue; //wtf?
		    x++;

		    int inc_len;
		    for(inc_len = 0; *(big_buffer + x + inc_len) != term &&
                                     !QMAKE_EOL(*(big_buffer + x + inc_len)); inc_len++);
		    *(big_buffer + x + inc_len) = '\0';
		    inc = big_buffer + x;
		} else if(total_size_read >= x + 14 && !strncmp(big_buffer + x,  "qmake_warning ", 14)) {
		    for(x+=14; //skip spaces after keyword
			x < total_size_read && (*(big_buffer+x) == ' ' || *(big_buffer+x) == '\t');
			x++);
		    char term = 0;
		    if(*(big_buffer + x) == '"')
			term = '"';
		    if(*(big_buffer + x) == '\'')
			term = '\'';
		    if(term)
			x++;

		    int msg_len;
		    for(msg_len = 0; (term && *(big_buffer + x + msg_len) != term) &&
                                     !QMAKE_EOL(*(big_buffer + x + msg_len)); msg_len++);
                    const char saved_term = *(big_buffer + x + msg_len);
		    *(big_buffer + x + msg_len) = '\0';
		    QString msg = big_buffer + x;
		    debug_msg(0, "%s:%d qmake_warning -- %s", fix_env_fn.latin1(),
			      line_count, msg.latin1());
		    *(big_buffer + x + msg_len) = saved_term; //put it back
		}
	    }
	} else if(ui_file) {
	    // skip whitespaces
	    while(x < total_size_read &&
		  (*(big_buffer+x) == ' ' || *(big_buffer+x) == '\t'))
		x++;
	    if(*(big_buffer + x) == '<') {
		x++;
		if(total_size_read >= x + 12 && !strncmp(big_buffer + x, "includehint", 11) &&
		    (*(big_buffer + x + 11) == ' ' || *(big_buffer + x + 11) == '>')) {
		    for(x += 11; *(big_buffer + x) != '>'; x++);
		    int inc_len = 0;
		    for(x += 1 ; *(big_buffer + x + inc_len) != '<'; inc_len++);
		    *(big_buffer + x + inc_len) = '\0';
		    inc = big_buffer + x;
		} else if(total_size_read >= x + 13 && !strncmp(big_buffer + x, "customwidget", 12) &&
			  (*(big_buffer + x + 12) == ' ' || *(big_buffer + x + 12) == '>')) {
		    for(x += 13; *(big_buffer + x) != '>'; x++); //skip up to >
		    while(x < total_size_read) {
			for(x++; *(big_buffer + x) != '<'; x++); //skip up to <
			x++;
			if(total_size_read >= x + 7 && !strncmp(big_buffer+x, "header", 6) &&
			   (*(big_buffer + x + 6) == ' ' || *(big_buffer + x + 6) == '>')) {
			    for(x += 7; *(big_buffer + x) != '>'; x++); //skip up to >
			    int inc_len = 0;
			    for(x += 1 ; *(big_buffer + x + inc_len) != '<'; inc_len++);
			    *(big_buffer + x + inc_len) = '\0';
			    inc = big_buffer + x;
			    break;
			} else if(total_size_read >= x + 14 && !strncmp(big_buffer+x, "/customwidget", 13) &&
				  (*(big_buffer + x + 13) == ' ' || *(big_buffer + x + 13) == '>')) {
			    x += 14;
			    break;
			}
		    }
		} else if(total_size_read >= x + 8 && !strncmp(big_buffer + x, "include", 7) &&
		    (*(big_buffer + x + 7) == ' ' || *(big_buffer + x + 7) == '>')) {
		    for(x += 8; *(big_buffer + x) != '>'; x++) {
			if(total_size_read >= x + 9 && *(big_buffer + x) == 'i' &&
			   !strncmp(big_buffer + x, "impldecl", 8)) {
			    for(x += 8; *(big_buffer + x) != '='; x++);
			    if(*(big_buffer + x) != '=')
				continue;
			    for(x++; *(big_buffer+x) == '\t' || *(big_buffer+x) == ' '; x++);
			    char quote = 0;
			    if(*(big_buffer+x) == '\'' || *(big_buffer+x) == '"') {
				quote = *(big_buffer + x);
				x++;
			    }
			    int val_len;
			    for(val_len = 0; TRUE; val_len++) {
				if(quote) {
				    if(*(big_buffer+x+val_len) == quote)
					break;
				} else if(*(big_buffer + x + val_len) == '>' ||
					  *(big_buffer + x + val_len) == ' ') {
				    break;
				}
			    }
			    char saved = *(big_buffer + x + val_len);
			    *(big_buffer + x + val_len) = '\0';
			    QString where = big_buffer + x;
			    *(big_buffer + x + val_len) = saved;
			    if(where == "in implementation") {
				QString cpp = fn.left(fn.length() - Option::ui_ext.length()) +
						      Option::cpp_ext.first();
				outdeps = &findDependencies(cpp);
			    }
			}
		    }
		    int inc_len = 0;
		    for(x += 1 ; *(big_buffer + x + inc_len) != '<'; inc_len++);
		    *(big_buffer + x + inc_len) = '\0';
		    inc = big_buffer + x;
		}
	    }
	}

	if(!inc.isEmpty()) {
	    bool from_source_dir = TRUE;
	    debug_msg(5, "%s:%d Found dependency to %s", fix_env_fn.latin1(),
		      line_count, inc.latin1());
	    if(!project->isEmpty("SKIP_DEPENDS")) {
		bool found = FALSE;
		QStringList &nodeplist = project->values("SKIP_DEPENDS");
		for(QStringList::Iterator it = nodeplist.begin();
		    it != nodeplist.end(); ++it) {
		    QRegExp regx((*it));
		    if(regx.search(inc) != -1) {
			found = TRUE;
			break;
		    }
		}
		if(found)
		    continue;
	    }
	    QString fqn;
	    if(project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH") &&
	       !stat(fix_env_fndir + inc, &fst) && !S_ISDIR(fst.st_mode)) {
		fqn = fndir + inc;
		goto handle_fqn;
	    } else if(project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH") &&
	       !stat(inc, &fst) && !S_ISDIR(fst.st_mode)) {
		fqn = inc;
		goto handle_fqn;
	    } else {
		if((Option::target_mode == Option::TARG_MAC9_MODE && inc.find(':')) ||
		   (Option::target_mode == Option::TARG_WIN_MODE && inc[1] != ':') ||
		   ((Option::target_mode == Option::TARG_UNIX_MODE ||
		     Option::target_mode == Option::TARG_QNX6_MODE ||
		     Option::target_mode == Option::TARG_MACX_MODE) &&
		    inc[0] != '/')) {
		    for(MakefileDependDir *mdd = dirs.first(); mdd; mdd = dirs.next() ) {
			if(!stat(mdd->local_dir + QDir::separator() + inc, &fst) &&
			   !S_ISDIR(fst.st_mode)) {
			    fqn = mdd->real_dir + QDir::separator() + inc;
			    goto handle_fqn;
			}
		    }
		}
	    }
	    if(fqn.isEmpty() && Option::mkfile::do_dep_heuristics) {
		//these are some hacky heuristics it will try to do on an include
		//however these can be turned off at runtime, I'm not sure how
		//reliable these will be, most likely when problems arise turn it off
		//and see if they go away..
		if(depHeuristics.contains(inc)) {
		    fqn = depHeuristics[inc];
		    from_source_dir = FALSE;
		} else if(Option::mkfile::do_dep_heuristics) { //some heuristics..
		    //is it a file from a .ui?
		    QString inc_file = inc.section(Option::dir_sep, -1);
		    int extn = inc_file.findRev('.');
		    if(extn != -1 &&
		       (inc_file.right(inc_file.length()-extn) == Option::cpp_ext.first() ||
			inc_file.right(inc_file.length()-extn) == Option::h_ext.first())) {
			QString uip = inc_file.left(extn) + Option::ui_ext;
			QStringList uil = project->variables()["FORMS"];
			for(QStringList::Iterator it = uil.begin(); it != uil.end(); ++it) {
			    if((*it).section(Option::dir_sep, -1) == uip) {
				if(!project->isEmpty("UI_DIR"))
				    fqn = project->first("UI_DIR");
				else if(!project->isEmpty("UI_HEADERS_DIR"))
				    fqn = project->first("UI_HEADERS_DIR");
				else
				    fqn = (*it).section(Option::dir_sep, 0, -2);
				if(!fqn.isEmpty() && !fqn.endsWith(Option::dir_sep))
				    fqn += Option::dir_sep;
				fqn += inc_file;
				from_source_dir = FALSE; //uics go in the output_dir (so don't fix them)
				fqn = fileFixify(fqn, QDir::currentDirPath(), Option::output_dir);
				goto cache_fqn;
			    }
			}
		    }
		    if(project->isActiveConfig("lex_included")) { //is this the lex file?
			QString rhs = Option::lex_mod + Option::cpp_ext.first();
			if(inc.endsWith(rhs)) {
			    QString lhs = inc.left(inc.length() - rhs.length()) + Option::lex_ext;
			    QStringList ll = project->variables()["LEXSOURCES"];
			    for(QStringList::Iterator it = ll.begin(); it != ll.end(); ++it) {
				QString s = (*it), d;
				int slsh = s.findRev(Option::dir_sep);
				if(slsh != -1) {
				    d = s.left(slsh + 1);
				    s = s.right(s.length() - slsh - 1);
				}
				if(!project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH"))
				    d = project->first("QMAKE_ABSOLUTE_SOURCE_PATH");
				if(s == lhs) {
				    fqn = d + inc;
				    from_source_dir = FALSE;  //uics go in the output_dir (so don't fix them)
				    fqn = fileFixify(fqn, QDir::currentDirPath(), Option::output_dir);
				    goto cache_fqn;
				}
			    }
			}
		    }
		    { //is it from a .y?
			QString rhs = Option::yacc_mod + Option::h_ext.first();
			if(inc.endsWith(rhs)) {
			    QString lhs = inc.left(inc.length() - rhs.length()) + Option::yacc_ext;
			    QStringList yl = project->variables()["YACCSOURCES"];
			    for(QStringList::Iterator it = yl.begin(); it != yl.end(); ++it) {
				QString s = (*it), d;
				int slsh = s.findRev(Option::dir_sep);
				if(slsh != -1) {
				    d = s.left(slsh + 1);
				    s = s.right(s.length() - slsh - 1);
				}
				if(!project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH"))
				    d = project->first("QMAKE_ABSOLUTE_SOURCE_PATH");
				if(s == lhs) {
				    fqn = d + inc;
				    from_source_dir = FALSE;  //uics go in the output_dir (so don't fix them)
				    fqn = fileFixify(fqn, QDir::currentDirPath(), Option::output_dir);
				    goto cache_fqn;
				}
			    }
			}
		    }
		    if( mocAware() &&		    //is it a moc file?
		       ( inc.endsWith(Option::cpp_ext.first()) || inc.endsWith(Option::cpp_moc_ext) )
		       || ( (Option::cpp_ext.first() != Option::h_moc_ext) && inc.endsWith(Option::h_moc_ext) )) {
			QString mocs[] = { QString("_HDRMOC"), QString("_SRCMOC"), QString::null };
			for(int moc = 0; !mocs[moc].isNull(); moc++) {
			    QStringList &l = project->variables()[mocs[moc]];
			    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
				QString file = Option::fixPathToTargetOS((*it));
				if(file.section(Option::dir_sep, -(inc.contains('/')+1)) == inc) {
				    fqn = (*it);
				    if(mocs[moc] == "_HDRMOC") {
					//Since it is include, no need to link it in as well
					project->variables()["_SRCMOC"].append((*it));
					l.remove(it);
				    } else if(!findMocSource(fqn).endsWith(fileFixify(fn))) {
					/* Not really a very good test, but this will at least avoid
					   confusion if it really does happen (since tmake/qmake
					   previously didn't even allow this the test is mostly accurate) */
					warn_msg(WarnLogic,
						 "Found potential multiple MOC include %s (%s) in '%s'",
						 inc.latin1(), fqn.latin1(), fix_env_fn.latin1());
				    }
				    from_source_dir = FALSE; //mocs go in the output_dir (so don't fix them)
				    goto cache_fqn;
				}
			    }
			}
		    }
		    fqn = findDependency(inc); //all else fails..
		cache_fqn:
		    if(from_source_dir) {
			fqn = fileFixify(fqn);
			from_source_dir = FALSE;
		    }
		    depHeuristics.insert(inc, fqn);
		}
	    }
	handle_fqn:
	    if(fqn.isEmpty()) //I give up
	      continue;
	    fqn = Option::fixPathToTargetOS(fqn, FALSE);
	    if(from_source_dir)
		fqn = fileFixify(fqn);
	    debug_msg(4, "Resolved dependency of %s to %s", inc.latin1(), fqn.latin1());
	    if(outdeps && outdeps->findIndex(fqn) == -1)
		outdeps->append(fqn);
	}
	//read past new line now..
	for( ; x < total_size_read && !QMAKE_EOL(*(big_buffer + x)); x++);
        line_count++;
    }

    if(recurse) {
	for(QStringList::Iterator fnit = fndeps.begin(); fnit != fndeps.end(); ++fnit) {
	    generateDependencies(dirs, (*fnit), recurse);
	    QStringList &deplist = findDependencies((*fnit));
	    for(QStringList::Iterator it = deplist.begin(); it != deplist.end(); ++it) 
		if(fndeps.findIndex((*it)) == -1 && (*it) != fn)
		    fndeps.append((*it));
	}
    }
    debug_msg(2, "Dependencies: %s -> %s", fn.latin1(), fndeps.join(" :: ").latin1());
    return TRUE;
}

void
MakefileGenerator::initOutPaths()
{
    if(init_opath_already)
	return;
    init_opath_already = TRUE;
    QMap<QString, QStringList> &v = project->variables();
	if(!v.contains("QMAKE_ABSOLUTE_SOURCE_PATH")) {
	    if(Option::mkfile::do_cache && !Option::mkfile::cachefile.isEmpty() &&
	       v.contains("QMAKE_ABSOLUTE_SOURCE_ROOT")) {
		QString root = v["QMAKE_ABSOLUTE_SOURCE_ROOT"].first();
		root = Option::fixPathToTargetOS( root );
		if(!root.isEmpty()) {
		    QFileInfo fi(Option::mkfile::cachefile);
		    if(!fi.convertToAbs()) {
			QString cache_r = fi.dirPath(), pwd = Option::output_dir;
			if ( pwd.startsWith(cache_r) && !pwd.startsWith(root) ) {
			    pwd = Option::fixPathToTargetOS(root + pwd.mid(cache_r.length()));
			    if(QFile::exists(pwd))
				v.insert("QMAKE_ABSOLUTE_SOURCE_PATH", pwd);
			}
		    }
		}
	    }
	}
	if(!v["QMAKE_ABSOLUTE_SOURCE_PATH"].isEmpty()) {
	    QString &asp = v["QMAKE_ABSOLUTE_SOURCE_PATH"].first();
	    asp = Option::fixPathToTargetOS( asp );
	    if(asp.isEmpty() || asp == Option::output_dir) //if they're the same, why bother?
		v["QMAKE_ABSOLUTE_SOURCE_PATH"].clear();
	}
	QString currentDir = QDir::currentDirPath();
	QString dirs[] = { QString("OBJECTS_DIR"), QString("MOC_DIR"), QString("UI_HEADERS_DIR"),
			   QString("UI_SOURCES_DIR"), QString("UI_DIR"), QString("DESTDIR"),
			   QString("SUBLIBS_DIR"), QString("DLLDESTDIR"), QString::null };
	for(int x = 0; dirs[x] != QString::null; x++) {
	    if ( !v[dirs[x]].isEmpty() ) {
		QString orig_path = v[dirs[x]].first();
#ifdef Q_WS_WIN
		// We don't want to add a separator for DLLDESTDIR on Windows
		if (!(dirs[x] == "DLLDESTDIR"))
#endif
		{
		    QString &path = v[dirs[x]].first();
		    path = fileFixify(path, Option::output_dir, Option::output_dir);
		    if(path.right(Option::dir_sep.length()) != Option::dir_sep)
			path += Option::dir_sep;
		}
		if(noIO())
		    continue;

		QString path = project->first(dirs[x]); //not to be changed any further
		path = Option::fixPathToTargetOS(fileFixify(path, QDir::currentDirPath(), Option::output_dir));
		debug_msg(3, "Fixed output_dir %s (%s) into %s (%s)", dirs[x].latin1(), orig_path.latin1(),
			  v[dirs[x]].join("::").latin1(), path.latin1());

		QDir d;
		if(path.startsWith(Option::dir_sep)) {
		    d.cd(Option::dir_sep);
		    path = path.right(path.length() - 1);
		}
#ifdef Q_WS_WIN
		bool driveExists = TRUE;
		if ( !QDir::isRelativePath( path ) ) {
		    if ( QFile::exists( path.left( 3 ) ) ) {
			d.cd( path.left( 3 ) );
			path = path.right( path.length() - 3 );
		    } else {
			warn_msg(WarnLogic, "%s: Cannot access drive '%s' (%s)", dirs[x].latin1(),
			    path.left( 3 ).latin1(), path.latin1() );
			driveExists = FALSE;
		    }
		}
		if ( driveExists ) {
#endif
		    QStringList subs = QStringList::split(Option::dir_sep, path);
		    for(QStringList::Iterator subit = subs.begin(); subit != subs.end(); ++subit) {
			if(!d.cd(*subit)) {
			    d.mkdir((*subit));
			    if ( d.exists( (*subit) ) )
				d.cd((*subit));
			    else {
				warn_msg(WarnLogic, "%s: Cannot access directory '%s' (%s)", dirs[x].latin1(),
				    (*subit).latin1(), path.latin1() );
				break;
			    }
			}
		    }
#ifdef Q_WS_WIN
		}
#endif
	    }
	}
	if ( !v["DESTDIR"].isEmpty() ) {
	    QDir d(v["DESTDIR"].first());
	    if(Option::fixPathToLocalOS(d.absPath()) == Option::fixPathToLocalOS(Option::output_dir))
		v.remove("DESTDIR");
	}
	QDir::current().cd( currentDir );
}

void
MakefileGenerator::init()
{
    initOutPaths();
    if(init_already)
	return;
    init_already = TRUE;

    QMap<QString, QStringList> &v = project->variables();
    QString paths[] = { QString("SOURCES"), QString("FORMS"), QString("YACCSOURCES"), QString("INCLUDEPATH"),
			QString("HEADERS"), QString("HEADERS_ORIG"), QString("LEXSOURCES"),
			QString("QMAKE_INTERNAL_INCLUDED_FILES"),
			QString("PRECOMPILED_HEADER"), QString::null };
    for(int y = 0; paths[y] != QString::null; y++) {
	QStringList &l = v[paths[y]];
	for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	    if ((*it).isEmpty())
		continue;
	    if(QFile::exists((*it)))
		(*it) = fileFixify((*it));
	}
    }

    /* get deps and mocables */
    QDict<void> cache_found_files;
    QString cache_file(".qmake.internal.cache");
    if(!project->isEmpty("QMAKE_INTERNAL_CACHE_FILE"))
	cache_file = Option::fixPathToLocalOS(project->first("QMAKE_INTERNAL_CACHE_FILE"));
    if(cache_file.find(QDir::separator()) == -1) //guess they know what they are doing..
	cache_file.prepend(Option::output_dir + QDir::separator());
    if((Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT ||
	Option::mkfile::do_deps || Option::mkfile::do_mocs) && !noIO()) {
	QPtrList<MakefileDependDir> deplist;
	deplist.setAutoDelete(TRUE);
	if((Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT || Option::mkfile::do_deps) &&
	   doDepends()) {
	    QStringList incDirs = v["DEPENDPATH"] + v["QMAKE_ABSOLUTE_SOURCE_PATH"];
	    if(project->isActiveConfig("depend_includepath"))
		incDirs += v["INCLUDEPATH"];
	    for(QStringList::Iterator it = incDirs.begin(); it != incDirs.end(); ++it) {
		QString r = (*it), l = Option::fixPathToLocalOS((*it));
		deplist.append(new MakefileDependDir(r.replace("\"",""),
						     l.replace("\"","")));
	    }
	    debug_msg(1, "Dependency Directories: %s", incDirs.join(" :: ").latin1());
	    if(Option::output.name() != "-" && project->isActiveConfig("qmake_cache")) {
		QFile cachef(cache_file);
		if(cachef.open(IO_ReadOnly | IO_Translate)) {
		    QFileInfo cachefi(cache_file);
		    debug_msg(2, "Trying internal cache information: %s", cache_file.latin1());
		    QTextStream cachet(&cachef);
		    QString line, file;
		    enum { CacheInfo, CacheDepend, CacheMoc } state = CacheInfo;
		    while (!cachet.eof()) {
			line = cachet.readLine().stripWhiteSpace();
			int sep = line.find('=');
			if(line == "[depend]") {
			    state = CacheDepend;
			} else if(line == "[mocable]") {
			    state = CacheMoc;
			} else if(line == "[check]") {
			    state = CacheInfo;
			} else if(!line.isEmpty() && sep != -1) {
			    file = line.left(sep).stripWhiteSpace();
			    line = line.right(line.length() - sep - 1).stripWhiteSpace();
			    if(state == CacheInfo) {
				if(file == "QMAKE_CACHE_VERSION") {
				    if(line != qmake_version())
					break;
				} else {
				    const QStringList &l = project->variables()[file];
				    if(!l.isEmpty() && !line.isEmpty() && l.join(" ") != line)
					break;
				}
			    } else if(state == CacheDepend) {
				bool found = (bool)cache_found_files[file];
				QStringList files = QStringList::split(" ", line);
				if(!found) {
				    QFileInfo fi(fileFixify(file, QDir::currentDirPath(), Option::output_dir));
				    if(fi.exists() && fi.lastModified() < cachefi.lastModified()) {
					cache_found_files.insert(file, (void *)1);
					found = TRUE;
				    }
				}
				if(found) {
				    for(QStringList::Iterator dep_it = files.begin();
					dep_it != files.end(); ++dep_it) {
					if(!cache_found_files[(*dep_it)]) {
					    QFileInfo fi(fileFixify((*dep_it), QDir::currentDirPath(), Option::output_dir));
					    if(fi.exists() &&
					       fi.lastModified() < cachefi.lastModified()) {
						cache_found_files.insert((*dep_it), (void *)1);
					    } else {
						found = FALSE;
						break;
					    }
					}
				    }
				    if(found) {
					debug_msg(2, "Dependencies (cached): %s -> %s", file.latin1(),
						  files.join(" :: ").latin1());
					findDependencies(file) = files;
					setProcessedDependencies(file, TRUE);
				    }
				}
			    } else {
				void *found = cache_found_files[file];
				if(found != (void *)2) {
				    if(found) {
					cache_found_files.replace(file, (void *)2);
				    } else {
					QFileInfo fi(fileFixify(file, QDir::currentDirPath(), Option::output_dir));
					if(fi.exists() && fi.lastModified() < cachefi.lastModified()) {
					    cache_found_files.insert(file, (void *)2);
					    found = (void*)1;
					}
				    }
				}
				if(found && line != "*qmake_ignore*") {
				    int ext_len = file.length() - file.findRev('.');
				    bool cpp_ext = FALSE;
				    for(QStringList::Iterator cppit = Option::cpp_ext.begin();
					cppit != Option::cpp_ext.end(); ++cppit) {
					if((cpp_ext = (file.right(ext_len) == (*cppit))))
					    break;
				    }
				    if(cpp_ext) {
					project->variables()["_SRCMOC"].append(line);
				    } else if(project->variables()["HEADERS"].findIndex(file) != -1) {
					for(QStringList::Iterator hit = Option::h_ext.begin();
					    hit != Option::h_ext.end(); ++hit) {
					    if((file.right(ext_len) == (*hit))) {
						project->variables()["_HDRMOC"].append(line);
						break;
					    }
					}
				    }
				    debug_msg(2, "Mocgen (cached): %s -> %s", file.latin1(),
					      line.latin1());
				    mocablesToMOC[file] = line;
				    mocablesFromMOC[line] = file;
				}
			    }
			}
		    }
		    cachef.close();
		}
	    }
	}
	if(!noIO()) {
	    QString sources[] = { QString("OBJECTS"), QString("LEXSOURCES"), QString("YACCSOURCES"),
				  QString("HEADERS"), QString("SOURCES"), QString("FORMS"), 
				  QString("PRECOMPILED_HEADER"), QString::null };
	    depHeuristics.clear();
	    bool write_cache = FALSE, read_cache = QFile::exists(cache_file);
	    int x;
	    for(x = 0; sources[x] != QString::null; x++) {
		QStringList vpath, &l = v[sources[x]];
		for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
		    if(!(*val_it).isEmpty()) {
			QString file = fileFixify((*val_it), QDir::currentDirPath(), Option::output_dir);
			if(!QFile::exists(file)) {
			    bool found = FALSE;
			    if(QDir::isRelativePath((*val_it))) {
				if(vpath.isEmpty())
				    vpath = v["VPATH_" + sources[x]] + v["VPATH"] +
					    v["QMAKE_ABSOLUTE_SOURCE_PATH"] + v["DEPENDPATH"];

				for(QStringList::Iterator vpath_it = vpath.begin();
				    vpath_it != vpath.end(); ++vpath_it) {
				    QString real_dir = Option::fixPathToLocalOS((*vpath_it));
				    if(QFile::exists(real_dir + QDir::separator() + (*val_it))) {
					QString dir = (*vpath_it);
					if(dir.right(Option::dir_sep.length()) != Option::dir_sep)
					    dir += Option::dir_sep;
					(*val_it) = fileFixify(dir + (*val_it));
					found = TRUE;
					debug_msg(1, "Found file through vpath %s -> %s",
						  file.latin1(), (*val_it).latin1());
					break;
				    }
				}
			    }
			    if(!found) {
				QString dir, regex = (*val_it), real_dir;
				if(regex.findRev(Option::dir_sep) != -1) {
				    dir = regex.left(regex.findRev(Option::dir_sep) + 1);
				    real_dir = fileFixify(Option::fixPathToLocalOS(dir),
							  QDir::currentDirPath(), Option::output_dir);
				    regex = regex.right(regex.length() - dir.length());
				}
				if(real_dir.isEmpty() || QFile::exists(real_dir)) {
				    QDir d(real_dir, regex);
				    if(!d.count()) {
					debug_msg(1, "%s:%d Failure to find %s in vpath (%s)",
						  __FILE__, __LINE__,
						  (*val_it).latin1(), vpath.join("::").latin1());
					warn_msg(WarnLogic, "Failure to find: %s", (*val_it).latin1());
					continue;
				    } else {
					for(int i = 0; i < (int)d.count(); i++) {
					    QString file = fileFixify(dir + d[i]);
					    if(i == (int)d.count() - 1)
						(*val_it) = file;
					    else
						l.insert(val_it, file);
					}
				    }
				} else {
				    debug_msg(1, "%s:%d Cannot match %s%c%s, as %s does not exist.",
					      __FILE__, __LINE__,
					      real_dir.latin1(), QDir::separator(), regex.latin1(),
					      real_dir.latin1());
				    warn_msg(WarnLogic, "Failure to find: %s", (*val_it).latin1());
				}
			    }
			}
		    }
		}
	    }
	    for(x = 0; sources[x] != QString::null; x++) {
	        QStringList &l = v[sources[x]];
	        for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
		    bool found_cache_moc = FALSE, found_cache_dep = FALSE;
		    if(read_cache && Option::output.name() != "-" &&
			    project->isActiveConfig("qmake_cache")) {
			if(processedDependencies((*val_it)))
			    found_cache_dep = TRUE;
			if(cache_found_files[(*val_it)] == (void *)2)
			    found_cache_moc = TRUE;
			if(!found_cache_moc || !found_cache_dep)
			    write_cache = TRUE;
		    }
		    /* Do moc before dependency checking since some includes can come from
		       moc_*.cpp files */
		    if(found_cache_moc) {
			QString fixed_file(fileFixify((*val_it), QDir::currentDirPath(), Option::output_dir));
			QString moc = findMocDestination(fixed_file);
			if(!moc.isEmpty()) {
			    for(QStringList::Iterator cppit = Option::cpp_ext.begin();
				    cppit != Option::cpp_ext.end(); ++cppit) {
				if(fixed_file.endsWith((*cppit))) {
				    QStringList &deps = findDependencies(fixed_file);
				    if(!deps.contains(moc))
					deps.append(moc);
				    break;
				}
			    }
			}
		    } else if(mocAware() && (sources[x] == "SOURCES" || sources[x] == "HEADERS") &&
			    (Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT ||
			     Option::mkfile::do_mocs)) {
			generateMocList((*val_it));
		    }
		    if(!found_cache_dep && sources[x] != "OBJECTS") {
			debug_msg(5, "Looking for dependencies for %s", (*val_it).latin1());
			generateDependencies(deplist, (*val_it), doDepends());
		    }
		}
	    }
	    if(project->isActiveConfig("qmake_cache") && (write_cache || !read_cache)) {
		QFile cachef(cache_file);
		if(cachef.open(IO_WriteOnly | IO_Translate)) {
		    debug_msg(2, "Writing internal cache information: %s", cache_file.latin1());
		    QTextStream cachet(&cachef);
		    cachet << "[check]" << "\n"
			   << "QMAKE_CACHE_VERSION = " << qmake_version() << "\n"
			   << "QMAKE_ABSOLUTE_SOURCE_PATH = " << var("QMAKE_ABSOLUTE_SOURCE_PATH") << "\n"
			   << "MOC_DIR = " << var("MOC_DIR") << "\n"
			   << "UI_DIR = " <<  var("UI_DIR") << "\n"
			   << "UI_HEADERS_DIR = " <<  var("UI_HEADERS_DIR") << "\n"
			   << "UI_SOURCES_DIR = " <<  var("UI_SOURCES_DIR") << "\n";
		    cachet << "[depend]" << endl;
		    for(QMap<QString, QStringList>::Iterator it = depends.begin();
			it != depends.end(); ++it)
			cachet << dependencyKey(it.key()) << " = " << it.data().join(" ") << endl;
		    cachet << "[mocable]" << endl;
		    QString mc, moc_sources[] = { QString("HEADERS"), QString("SOURCES"), QString::null };
		    for(int x = 0; moc_sources[x] != QString::null; x++) {
			QStringList &l = v[moc_sources[x]];
			for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
			    QString f = fileFixify((*val_it));
			    if(!f.isEmpty()) {
				mc = mocablesToMOC[f];
				if(mc.isEmpty())
				    mc = "*qmake_ignore*";
				cachet << f << " = " << mc << endl;
			    }
			}
		    }
		    cachef.close();
		}
	    }
	}
    }
    v["OBJECTS"] = createObjectList("SOURCES") + v["OBJECTS"]; // init variables

    //lex files
    {
	QStringList &impls = v["LEXIMPLS"];
	QStringList &l = v["LEXSOURCES"];
	for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	    QString dir;
	    QFileInfo fi((*it));
	    if(fi.dirPath() != ".")
		dir = fi.dirPath() + Option::dir_sep;
	    dir = fileFixify(dir, QDir::currentDirPath(), Option::output_dir);
	    if(!dir.isEmpty() && dir.right(Option::dir_sep.length()) != Option::dir_sep)
		dir += Option::dir_sep;
	    QString impl = dir + fi.baseName(TRUE) + Option::lex_mod + Option::cpp_ext.first();
	    logicWarn(impl, "SOURCES");
	    logicWarn(impl, "SOURCES");
	    impls.append(impl);
	    if( ! project->isActiveConfig("lex_included")) {
		v["SOURCES"].append(impl);
		// attribute deps of lex file to impl file
		QStringList &lexdeps = findDependencies((*it));
		QStringList &impldeps = findDependencies(impl);
		for(QStringList::ConstIterator d = lexdeps.begin(); d != lexdeps.end(); ++d) {
		    if(!impldeps.contains(*d))
			impldeps.append(*d);
		}
		lexdeps.clear();
	    }
	}
	if( ! project->isActiveConfig("lex_included"))
	    v["OBJECTS"] += (v["LEXOBJECTS"] = createObjectList("LEXIMPLS"));
    }
    //yacc files
    {
	QStringList &decls = v["YACCCDECLS"], &impls = v["YACCIMPLS"];
	QStringList &l = v["YACCSOURCES"];
	for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	    QString dir;
	    QFileInfo fi((*it));
	    if(fi.dirPath() != ".")
		dir = fi.dirPath() + Option::dir_sep;
	    dir = fileFixify(dir, QDir::currentDirPath(), Option::output_dir);
	    if(!dir.isEmpty() && dir.right(Option::dir_sep.length()) != Option::dir_sep)
		dir += Option::dir_sep;
	    QString impl = dir + fi.baseName(TRUE) + Option::yacc_mod + Option::cpp_ext.first();
	    logicWarn(impl, "SOURCES");
	    QString decl = dir + fi.baseName(TRUE) + Option::yacc_mod + Option::h_ext.first();
	    logicWarn(decl, "HEADERS");

	    decls.append(decl);
	    impls.append(impl);
	    v["SOURCES"].append(impl);
	    QStringList &impldeps = findDependencies(impl);
	    impldeps.append(decl);
	    // attribute deps of yacc file to impl file
	    QStringList &yaccdeps = findDependencies((*it));
	    for(QStringList::ConstIterator d = yaccdeps.begin(); d != yaccdeps.end(); ++d) {
		if(!impldeps.contains(*d))
		    impldeps.append(*d);
	    }
	    if( project->isActiveConfig("lex_included")) {
		// is there a matching lex file ? Transfer its dependencies.
		QString lexsrc = fi.baseName(TRUE) + Option::lex_ext;
		if(fi.dirPath() != ".")
		    lexsrc.prepend(fi.dirPath() + Option::dir_sep);
		if(v["LEXSOURCES"].findIndex(lexsrc) != -1) {
		    QString trg = dir + fi.baseName(TRUE) + Option::lex_mod + Option::cpp_ext.first();
		    impldeps.append(trg);
		    impldeps += findDependencies(lexsrc);
		    depends[lexsrc].clear();
		}
	    }
	    yaccdeps.clear();
	}
	v["OBJECTS"] += (v["YACCOBJECTS"] = createObjectList("YACCIMPLS"));
    }

    //UI files
    {
	QStringList &includepath = project->variables()["INCLUDEPATH"];
	if(!project->isEmpty("UI_DIR"))
	    includepath.append(project->first("UI_DIR"));
	else if(!project->isEmpty("UI_HEADERS_DIR"))
	    includepath.append(project->first("UI_HEADERS_DIR"));
	QStringList &decls = v["UICDECLS"], &impls = v["UICIMPLS"];
	QStringList &l = v["FORMS"];
	for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	    QString impl, decl;
	    QFileInfo fi(Option::fixPathToLocalOS((*it)));
	    if ( !project->isEmpty("UI_DIR") ) {
		impl = decl = project->first("UI_DIR");
		QString d = fi.dirPath();
		if( d == ".")
		    d = QDir::currentDirPath();
		d = fileFixify(d, QDir::currentDirPath(), Option::output_dir);
		if(!includepath.contains(d))
		    includepath.append(d);
	    } else {
		if(decl.isEmpty() && !project->isEmpty("UI_HEADERS_DIR"))
		    decl = project->first("UI_HEADERS_DIR");
		if(!decl.isEmpty() || (project->isEmpty("UI_HEADERS_DIR") &&
				       !project->isEmpty("UI_SOURCES_DIR")) ) {
		    QString d = fi.dirPath();
		    if( d == ".")
			d = QDir::currentDirPath();
		    d = fileFixify(d, QDir::currentDirPath(), Option::output_dir);
		    if(!includepath.contains(d))
			includepath.append(d);
		}
		if(impl.isEmpty() && !project->isEmpty("UI_SOURCES_DIR"))
		    impl = project->first("UI_SOURCES_DIR");
		if(fi.dirPath() != ".") {
		    if(impl.isEmpty())
			impl = fi.dirPath() + Option::dir_sep;
		    if(decl.isEmpty())
			decl = fi.dirPath() + Option::dir_sep;
		}
	    }
	    impl = fileFixify(impl, QDir::currentDirPath(), Option::output_dir);
	    if(!impl.isEmpty() && !impl.endsWith(Option::dir_sep))
		impl += Option::dir_sep;
	    impl += fi.baseName(TRUE) + Option::cpp_ext.first();
	    if(Option::output_dir != QDir::currentDirPath() &&
	       project->isEmpty("UI_DIR") && project->isEmpty("UI_HEADERS_DIR")) {
		QString decl_fixed = fileFixify(decl, QDir::currentDirPath(), Option::output_dir);
		if(!includepath.contains(decl_fixed))
		    includepath.append(decl_fixed);
		if(!includepath.contains(decl))
		    project->variables()["INCLUDEPATH"].append(decl);
	    }
	    decl = fileFixify(decl, QDir::currentDirPath(), Option::output_dir);
	    if(!decl.isEmpty() && !decl.endsWith(Option::dir_sep))
		decl += Option::dir_sep;
	    decl += fi.baseName(TRUE) + Option::h_ext.first();
	    logicWarn(impl, "SOURCES");
	    logicWarn(decl, "HEADERS");
	    decls.append(decl);
	    impls.append(impl);
	    findDependencies(impl).append(decl);

	    QString mocable = Option::h_moc_mod + fi.baseName(TRUE) + Option::h_moc_ext;
	    if(!v["MOC_DIR"].isEmpty())
		mocable.prepend(v["MOC_DIR"].first());
	    else if(fi.dirPath() != ".")
		mocable.prepend(fi.dirPath() + Option::dir_sep);
	    logicWarn(mocable, "SOURCES");
	    mocablesToMOC[cleanFilePath(decl)] = mocable;
	    mocablesFromMOC[cleanFilePath(mocable)] = decl;
	    v["_UIMOC"].append(mocable);
	}
	v["OBJECTS"] += (v["UICOBJECTS"] = createObjectList("UICDECLS"));
    }

    //Translation files
    if(!project->isEmpty("TRANSLATIONS")) {
	QStringList &trf = project->variables()["TRANSLATIONS"];
	for(QStringList::Iterator it = trf.begin(); it != trf.end(); ++it) {
	    (*it) = Option::fixPathToLocalOS((*it));
	}
    }

    //Image files
    if(!project->isEmpty("IMAGES")) {
	if(project->isEmpty("QMAKE_IMAGE_COLLECTION"))
	    v["QMAKE_IMAGE_COLLECTION"].append("qmake_image_collection" + Option::cpp_ext.first());
	QString imgfile = project->first("QMAKE_IMAGE_COLLECTION");
	Option::fixPathToTargetOS(imgfile);
	if(!project->isEmpty("UI_DIR") || !project->isEmpty("UI_SOURCES_DIR")) {
	    if(imgfile.find(Option::dir_sep) != -1)
		imgfile = imgfile.right(imgfile.findRev(Option::dir_sep) + 1);
	    imgfile.prepend( (project->isEmpty("UI_DIR") ? project->first("UI_SOURCES_DIR") :
			    project->first("UI_DIR")) );
	    v["QMAKE_IMAGE_COLLECTION"] = QStringList(imgfile);
	}
	logicWarn(imgfile, "SOURCES");
	if(!noIO()) {
	    QStringList &l = v["IMAGES"];
	    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
		if(!QFile::exists((*it))) {
		    warn_msg(WarnLogic, "Failure to open: %s", (*it).latin1());
		    continue;
		}
		findDependencies(imgfile).append(fileFixify((*it)));
	    }
	}
	v["OBJECTS"] += (v["IMAGEOBJECTS"] = createObjectList("QMAKE_IMAGE_COLLECTION"));
    }
    if(Option::output_dir != QDir::currentDirPath())
	project->variables()["INCLUDEPATH"].append(fileFixify(Option::output_dir, Option::output_dir,
							      Option::output_dir));

    //moc files
    if ( mocAware() ) {
	if(!project->isEmpty("MOC_DIR"))
	    project->variables()["INCLUDEPATH"].append(project->first("MOC_DIR"));
	if ( Option::h_moc_ext == Option::cpp_ext.first() )
	    v["OBJMOC"] = createObjectList("_HDRMOC") + createObjectList("_UIMOC");

	QStringList &l = v["SRCMOC"];
	l = v["_HDRMOC"] + v["_UIMOC"] + v["_SRCMOC"];
	for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
	    if(!(*val_it).isEmpty())
		(*val_it) = Option::fixPathToTargetOS((*val_it), FALSE);
	}
    }

    QString fixpaths[] = { QString("PRE_TARGETDEPS"), QString("POST_TARGETDEPS"), QString::null };
    for(int path = 0; !fixpaths[path].isNull(); path++) {
	QStringList &l = v[fixpaths[path]];
	for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
	    if(!(*val_it).isEmpty())
		(*val_it) = Option::fixPathToTargetOS((*val_it), FALSE);
	}
    }

    // Make sure the INCLUDEPATH doesn't contain any empty(/null) entries
    QStringList &ipl = project->variables()["INCLUDEPATH"];
    for(QStringList::Iterator ipl_it = ipl.begin(); ipl_it != ipl.end();) {
	if ((*ipl_it).isEmpty())
	    ipl_it = ipl.remove(ipl_it);
	else
	    ++ipl_it;
    }
}

bool
MakefileGenerator::processPrlFile(QString &file)
{
    bool ret = FALSE, try_replace_file=FALSE;
    QString meta_file, orig_file = file;
    if(QMakeMetaInfo::libExists(file)) {
	try_replace_file = TRUE;
	meta_file = file;
	file = "";
    } else {
	QString tmp = file;
	int ext = tmp.findRev('.');
	if(ext != -1)
	    tmp = tmp.left(ext);
	meta_file = tmp;
    }
    meta_file = fileFixify(meta_file);
    if(!QMakeMetaInfo::libExists(fileFixify(meta_file, QDir::currentDirPath(), Option::output_dir)) &&
       project->isActiveConfig("qt")) {
	QString stem = meta_file, dir, extn;
	int slsh = stem.findRev('/'), hadlib = 0;
	if(slsh != -1) {
	    dir = stem.left(slsh + 1);
	    stem = stem.right(stem.length() - slsh - 1);
	}
	if(stem.startsWith("lib")) {
	    hadlib = 1;
	    stem = stem.right(stem.length() - 3);
	}
	int dot = stem.find('.');
	if(dot != -1) {
	    extn = stem.right(stem.length() - dot);
	    stem = stem.left(dot);
	}
	if(stem == "qt" || stem == "qte" || stem == "qte-mt" || stem == "qt-mt") {
	    if(stem.endsWith("-mt"))
		stem = stem.left(stem.length() - 3); //lose the -mt
	    else
		stem += "-mt"; //try the thread case
	    meta_file = dir;
	    if(hadlib)
		meta_file += "lib";
	    meta_file += stem + extn;
	    try_replace_file = TRUE;
	}
    }
    QString real_meta_file = Option::fixPathToLocalOS(meta_file);
    if(project->variables()["QMAKE_PRL_INTERNAL_FILES"].findIndex(QMakeMetaInfo::findLib(meta_file)) != -1) {
	ret = TRUE;
    } else if(!meta_file.isEmpty()) {
	QString f = fileFixify(real_meta_file, QDir::currentDirPath(), Option::output_dir);
	if(QMakeMetaInfo::libExists(f)) {
	    QMakeMetaInfo libinfo;
	    debug_msg(1, "Processing PRL file: %s", real_meta_file.latin1());
	    if(!libinfo.readLib(f)) {
		fprintf(stderr, "Error processing meta file: %s\n", real_meta_file.latin1());
	    } else if(project->isActiveConfig("no_read_prl_" + libinfo.type().lower())) {
		debug_msg(2, "Ignored meta file %s [%s]", real_meta_file.latin1(), libinfo.type().latin1()); 
	    } else {
		ret = TRUE;
		QMap<QString, QStringList> &vars = libinfo.variables();
		for( QMap<QString, QStringList>::Iterator it = vars.begin(); it != vars.end(); ++it)
		    processPrlVariable(it.key(), it.data());
		if(try_replace_file && !libinfo.isEmpty("QMAKE_PRL_TARGET")) {
		    QString dir;
		    int slsh = real_meta_file.findRev(Option::dir_sep);
		    if(slsh != -1)
			dir = real_meta_file.left(slsh+1);
		    file = libinfo.first("QMAKE_PRL_TARGET");
		    if(QDir::isRelativePath(file))
			file.prepend(dir);
		}
	    }
	}
	if(ret) {
	    QString mf = QMakeMetaInfo::findLib(meta_file);
	    project->variables()["QMAKE_PRL_INTERNAL_FILES"].append(mf);
	    project->variables()["QMAKE_INTERNAL_INCLUDED_FILES"].append(mf);
	}
    }
    if(try_replace_file && file.isEmpty()) {
#if 0
	warn_msg(WarnLogic, "Found prl [%s] file with no target [%s]!", meta_file.latin1(),
		 orig_file.latin1());
#endif
	file = orig_file;
    }
    return ret;
}

void
MakefileGenerator::processPrlVariable(const QString &var, const QStringList &l)
{
    if(var == "QMAKE_PRL_LIBS") {
	QString where = "QMAKE_LIBS";
	if(!project->isEmpty("QMAKE_INTERNAL_PRL_LIBS"))
	    where = project->first("QMAKE_INTERNAL_PRL_LIBS");
	QStringList &out = project->variables()[where];
	for(QStringList::ConstIterator it = l.begin(); it != l.end(); ++it) {
	    if( out.findIndex((*it)) == -1)
		out.append((*it));
	}
    } else if(var == "QMAKE_PRL_DEFINES") {
	QStringList &out = project->variables()["DEFINES"];
	for(QStringList::ConstIterator it = l.begin(); it != l.end(); ++it) {
	    if(out.findIndex((*it)) == -1 &&
	       project->variables()["PRL_EXPORT_DEFINES"].findIndex((*it)) == -1)
		out.append((*it));
	}
    }
}

void
MakefileGenerator::processPrlFiles()
{
    QDict<void> processed;
    for(bool ret = FALSE; TRUE; ret = FALSE) {
	//read in any prl files included..
	QStringList l_out;
	QString where = "QMAKE_LIBS";
	if(!project->isEmpty("QMAKE_INTERNAL_PRL_LIBS"))
	    where = project->first("QMAKE_INTERNAL_PRL_LIBS");
	QStringList &l = project->variables()[where];
	for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	    QString file = (*it);
	    if(!processed[file] && processPrlFile(file)) {
		processed.insert(file, (void*)1);
		ret = TRUE;
	    }
	    if(!file.isEmpty())
		l_out.append(file);
	}
	if(ret)
	    l = l_out;
	else
	    break;
    }
}

void
MakefileGenerator::writePrlFile(QTextStream &t)
{
    QString target = project->first("TARGET");
    int slsh = target.findRev(Option::dir_sep);
    if(slsh != -1)
	target = target.right(target.length() - slsh - 1);
    QString bdir = Option::output_dir;
    if(bdir.isEmpty())
	bdir = QDir::currentDirPath();
    t << "QMAKE_PRL_BUILD_DIR = " << bdir << endl;

    if(!project->projectFile().isEmpty() && project->projectFile() != "-")
	t << "QMAKE_PRO_INPUT = " << project->projectFile().section('/', -1) << endl;

    if(!project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH"))
	t << "QMAKE_PRL_SOURCE_DIR = " << project->first("QMAKE_ABSOLUTE_SOURCE_PATH") << endl;
    t << "QMAKE_PRL_TARGET = " << target << endl;
    if(!project->isEmpty("PRL_EXPORT_DEFINES"))
	t << "QMAKE_PRL_DEFINES = " << project->variables()["PRL_EXPORT_DEFINES"].join(" ") << endl;
    if(!project->isEmpty("PRL_EXPORT_CFLAGS"))
	t << "QMAKE_PRL_CFLAGS = " << project->variables()["PRL_EXPORT_CFLAGS"].join(" ") << endl;
    if(!project->isEmpty("PRL_EXPORT_CXXFLAGS"))
	t << "QMAKE_PRL_CXXFLAGS = " << project->variables()["PRL_EXPORT_CXXFLAGS"].join(" ") << endl;
    if(!project->isEmpty("CONFIG"))
	t << "QMAKE_PRL_CONFIG = " << project->variables()["CONFIG"].join(" ") << endl;
    if(!project->isEmpty("VERSION"))
        t << "QMAKE_PRL_VERSION = " << project->first("VERSION") << endl;
    if(project->isActiveConfig("staticlib") || project->isActiveConfig("explicitlib")) {
	QStringList libs;
	if(!project->isEmpty("QMAKE_INTERNAL_PRL_LIBS"))
	    libs = project->variables()["QMAKE_INTERNAL_PRL_LIBS"];
	else
	    libs << "QMAKE_LIBS"; //obvious one
	t << "QMAKE_PRL_LIBS = ";
	for(QStringList::Iterator it = libs.begin(); it != libs.end(); ++it)
	    t << project->variables()[(*it)].join(" ") << " ";
	t << endl;
    }
}

bool
MakefileGenerator::write()
{
    usePlatformDir();
    init();
    findLibraries();
    if((Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE || //write prl
       Option::qmake_mode == Option::QMAKE_GENERATE_PRL) &&
       project->variables()["QMAKE_FAILED_REQUIREMENTS"].isEmpty() &&
       project->isActiveConfig("create_prl") && project->first("TEMPLATE") == "lib" &&
       !project->isActiveConfig("plugin")) {
	QString prl = var("TARGET");
	int slsh = prl.findRev(Option::dir_sep);
	if(slsh != -1)
	    prl = prl.right(prl.length() - slsh);
	int dot = prl.find('.');
	if(dot != -1)
	    prl = prl.left(dot);
	prl += Option::prl_ext;
	if(!project->isEmpty("DESTDIR"))
	    prl.prepend(var("DESTDIR"));
	QString local_prl = Option::fixPathToLocalOS(fileFixify(prl, QDir::currentDirPath(), Option::output_dir));
	QFile ft(local_prl);
	if(ft.open(IO_WriteOnly)) {
	    project->variables()["ALL_DEPS"].append(prl);
	    project->variables()["QMAKE_INTERNAL_PRL_FILE"].append(prl);
	    QTextStream t(&ft);
	    writePrlFile(t);
	    ft.close();
	}
    }
    if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE &&
       project->isActiveConfig("link_prl")) //load up prl's'
	processPrlFiles();

    if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE || //write prl file
       Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT) {
	QTextStream t(&Option::output);
	writeMakefile(t);
    }
    return TRUE;
}

// Manipulate directories, so it's possible to build
// several cross-platform targets concurrently
void
MakefileGenerator::usePlatformDir()
{
    QString pltDir(project->first("QMAKE_PLATFORM_DIR"));
    if(pltDir.isEmpty())
	return;
    char sep = QDir::separator();
    QString slashPltDir = sep + pltDir;

    QString filePath = project->first("DESTDIR");
    project->variables()["DESTDIR"] = filePath 
				    + (filePath.isEmpty() ? pltDir : slashPltDir);

    filePath = project->first("DLLDESTDIR");
    project->variables()["DLLDESTDIR"] = filePath 
				       + (filePath.isEmpty() ? pltDir : slashPltDir);

    filePath = project->first("OBJECTS_DIR");
    project->variables()["OBJECTS_DIR"] = filePath 
					+ (filePath.isEmpty() ? pltDir : slashPltDir);

    filePath = project->first("QMAKE_LIBDIR_QT");
    project->variables()["QMAKE_LIBDIR_QT"] = filePath 
					    + (filePath.isEmpty() ? pltDir : slashPltDir);

    filePath = project->first("QMAKE_LIBS_QT");
    int fpi = filePath.findRev(sep);
    if (fpi == -1)
	project->variables()["QMAKE_LIBS_QT"].prepend(pltDir + sep);
    else
        project->variables()["QMAKE_LIBS_QT"] = filePath.left(fpi)
					      + slashPltDir
					      + filePath.mid(fpi);
    
    filePath = project->first("QMAKE_LIBS_QT_THREAD");
    fpi = filePath.findRev(sep);
    if (fpi == -1)
	project->variables()["QMAKE_LIBS_QT_THREAD"].prepend(pltDir + sep);
    else
        project->variables()["QMAKE_LIBS_QT_THREAD"] = filePath.left(fpi)
						     + slashPltDir
						     + filePath.mid(fpi);

    filePath = project->first("QMAKE_LIBS_QT_ENTRY");
    fpi = filePath.findRev(sep);
    if (fpi == -1)
	project->variables()["QMAKE_LIBS_QT_ENTRY"].prepend(pltDir + sep);
    else
        project->variables()["QMAKE_LIBS_QT_ENTRY"] = filePath.left(fpi)
						    + slashPltDir
						    + filePath.mid(fpi);
}

void
MakefileGenerator::writeObj(QTextStream &t, const QString &obj, const QString &src)
{
    QStringList &objl = project->variables()[obj];
    QStringList &srcl = project->variables()[src];

    QStringList::Iterator oit = objl.begin();
    QStringList::Iterator sit = srcl.begin();
    QString stringSrc("$src");
    QString stringObj("$obj");
    for( ;sit != srcl.end() && oit != objl.end(); oit++, sit++) {
	if((*sit).isEmpty())
	    continue;

	if(!doDepends()) {
	    QString sdep, odep = (*sit) + " ";
	    QStringList deps = findDependencies((*sit));
	    for(QStringList::Iterator dit = deps.begin(); dit != deps.end(); dit++) {
		if((*dit).endsWith(Option::cpp_moc_ext))
		    odep += (*dit) + " ";
		else
		    sdep += (*dit) + " ";
	    }
	    t << (*sit) << ": " << sdep << endl
	      << (*oit) << ": " << odep ;
	} else {
	    t << (*oit) << ": " << (*sit) << " " << findDependencies((*sit)).join(" \\\n\t\t");
	}

	QString comp, cimp;
	for(QStringList::Iterator cppit = Option::cpp_ext.begin(); cppit != Option::cpp_ext.end(); ++cppit) {
	    if((*sit).endsWith((*cppit))) {
		comp = "QMAKE_RUN_CXX";
		cimp = "QMAKE_RUN_CXX_IMP";
		break;
	    }
	}
	if(comp.isEmpty()) {
	    comp = "QMAKE_RUN_CC";
	    cimp = "QMAKE_RUN_CC_IMP";
	}
	bool use_implicit_rule = !project->isEmpty(cimp);
	if(use_implicit_rule) {
	    if(!project->isEmpty("OBJECTS_DIR")) {
		use_implicit_rule = FALSE;
	    } else {
		int dot = (*sit).findRev('.');
		if(dot == -1 || ((*sit).left(dot) + Option::obj_ext != (*oit)))
		    use_implicit_rule = FALSE;
	    }
	}
	if (!use_implicit_rule && !project->isEmpty(comp)) {
	    QString p = var(comp), srcf(*sit);
	    p.replace(stringSrc, srcf);
	    p.replace(stringObj, (*oit));
	    t << "\n\t" << p;
	}
	t << endl << endl;
    }
}


void
MakefileGenerator::writeUicSrc(QTextStream &t, const QString &ui)
{
    QStringList &uil = project->variables()[ui];
    for(QStringList::Iterator it = uil.begin(); it != uil.end(); it++) {
	QString decl, impl;
	{
	    QString tmp = (*it), impl_dir, decl_dir;
	    decl = tmp.replace(QRegExp("\\" + Option::ui_ext + "$"), Option::h_ext.first());
	    int dlen = decl.findRev(Option::dir_sep) + 1;
	    tmp = (*it);
	    impl = tmp.replace(QRegExp("\\" + Option::ui_ext + "$"), Option::cpp_ext.first());
	    int ilen = decl.findRev(Option::dir_sep) + 1;
	    if(!project->isEmpty("UI_DIR")) {
		impl_dir = project->first("UI_DIR");
		decl = project->first("UI_DIR") + decl.right(decl.length() - dlen);
		impl = project->first("UI_DIR") + impl.right(impl.length() - ilen);
	    } else {
		if(!project->isEmpty("UI_HEADERS_DIR")) {
		    decl_dir = project->first("UI_HEADERS_DIR");
		    decl = project->first("UI_HEADERS_DIR") + decl.right(decl.length() - dlen);
		}
		if(!project->isEmpty("UI_SOURCES_DIR")) {
		    impl_dir = project->first("UI_SOURCES_DIR");
		    impl = project->first("UI_SOURCES_DIR") + impl.right(impl.length() - ilen);
		}
	    }
	    impl = fileFixify(impl, QDir::currentDirPath(), Option::output_dir);
	    decl = fileFixify(decl, QDir::currentDirPath(), Option::output_dir);
	    if(decl_dir.isEmpty())
		decl_dir = decl.section(Option::dir_sep,0,-2);
	    if(impl_dir.isEmpty())
		impl_dir = impl.section(Option::dir_sep,0,-2);
	    if (QDir::isRelativePath(impl_dir))
		impl_dir.prepend(Option::output_dir + Option::dir_sep);
	    if (QDir::isRelativePath(decl_dir))
		decl_dir.prepend(Option::output_dir + Option::dir_sep);
	    createDir(impl_dir);
	    createDir(decl_dir);
	}
	QStringList deps = findDependencies((*it));
	deps.remove(decl); //avoid circular dependencies..
	t << decl << ": " << (*it) << " ";
        t << deps.join(" \\\n\t\t") << "\n\t"
          << "$(UIC) " << (*it) << " -o " << decl << endl << endl;

	QString mildDecl = decl;
	int k = mildDecl.findRev(Option::dir_sep);
	if ( k != -1 )
	    mildDecl = mildDecl.mid( k + 1 );
	t << impl << ": " << decl << " " << (*it) << " ";
        if(QFile::exists((*it) + Option::h_ext.first()))
            t << (*it) << Option::h_ext.first() << " ";
        t << deps.join(" \\\n\t\t") << "\n\t"
	  << "$(UIC) " << (*it) << " -i " << mildDecl << " -o " << impl << endl << endl;
    }
}


void
MakefileGenerator::writeMocObj(QTextStream &t, const QString &obj, const QString &src)
{
    QStringList &objl = project->variables()[obj],
		&srcl = project->variables()[src];
    QStringList::Iterator oit = objl.begin(), sit = srcl.begin();
    QString stringSrc("$src"), stringObj("$obj");
    for( ;sit != srcl.end() && oit != objl.end(); oit++, sit++) {
	QString hdr = findMocSource((*sit));
	t << (*oit) << ": " 
	  << (*sit) << " " << findDependencies((*sit)).join(" \\\n\t\t") << " "
	  << hdr << " " << findDependencies(hdr).join(" \\\n\t\t");
	bool use_implicit_rule = !project->isEmpty("QMAKE_RUN_CXX_IMP");
	if(use_implicit_rule) {
	    if(!project->isEmpty("OBJECTS_DIR") || !project->isEmpty("MOC_DIR")) {
		use_implicit_rule = FALSE;
	    } else {
		int dot = (*sit).findRev('.');
		if(dot == -1 || ((*sit).left(dot) + Option::obj_ext != (*oit)))
		    use_implicit_rule = FALSE;
	    }
	}
	if (!use_implicit_rule && !project->isEmpty("QMAKE_RUN_CXX")) {
	    QString p = var("QMAKE_RUN_CXX"), srcf(*sit);
	    p.replace(stringSrc, srcf);
	    p.replace(stringObj, (*oit));
	    t << "\n\t" << p;
	}
	t << endl << endl;
    }
}


void
MakefileGenerator::writeMocSrc(QTextStream &t, const QString &src)
{
    QStringList &l = project->variables()[src];
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QString m = Option::fixPathToTargetOS(findMocDestination(*it));
	if ( !m.isEmpty()) {
	    QString deps;
	    if(!project->isActiveConfig("no_mocdepend"))
		deps += "$(MOC) ";
	    deps += (*it);
	    t << m << ": " << deps << "\n\t"
	      << "$(MOC)";
	    t << " " << (*it) << " -o " << m << endl << endl;
	}
    }
}

void
MakefileGenerator::writeYaccSrc(QTextStream &t, const QString &src)
{
    QStringList &l = project->variables()[src];
    if(project->isActiveConfig("yacc_no_name_mangle") && l.count() > 1)
	warn_msg(WarnLogic, "yacc_no_name_mangle specified, but multiple parsers expected."
		 "This can lead to link problems.\n");
    QString default_out_h = "y.tab.h", default_out_c = "y.tab.c";
    if(!project->isEmpty("QMAKE_YACC_HEADER"))
	default_out_h = project->first("QMAKE_YACC_HEADER");
    if(!project->isEmpty("QMAKE_YACC_SOURCE"))
	default_out_c = project->first("QMAKE_YACC_SOURCE");
    QString stringBase("$base");
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QFileInfo fi((*it));
	QString dir;
	if(fi.dirPath() != ".")
	    dir = fi.dirPath() + Option::dir_sep;
	dir = fileFixify(dir, QDir::currentDirPath(), Option::output_dir);
	if(!dir.isEmpty() && dir.right(Option::dir_sep.length()) != Option::dir_sep)
	    dir += Option::dir_sep;

	QString impl = dir + fi.baseName(TRUE) + Option::yacc_mod + Option::cpp_ext.first();
	QString decl = dir + fi.baseName(TRUE) + Option::yacc_mod + Option::h_ext.first();

	QString yaccflags = "$(YACCFLAGS)", mangle = "y";
	if(!project->isActiveConfig("yacc_no_name_mangle")) {
	    mangle = fi.baseName(TRUE);
	    if(!project->isEmpty("QMAKE_YACCFLAGS_MANGLE")) 
		yaccflags += " " + var("QMAKE_YACCFLAGS_MANGLE").replace(stringBase, mangle);
	    else
		yaccflags += " -p " + mangle;
	}
	QString out_h = default_out_h, out_c = default_out_c;
	if(!mangle.isEmpty()) {
	    out_h.replace(stringBase, mangle);
	    out_c.replace(stringBase, mangle);
	}

	t << impl << ": " << (*it) << "\n\t"
	  << "$(YACC) " << yaccflags << " " << (*it) << "\n\t"
	  << "-$(DEL_FILE) " << impl << " " << decl << "\n\t"
	  << "-$(MOVE) " << out_h << " " << decl << "\n\t"
	  << "-$(MOVE) " << out_c << " " << impl << endl << endl;
	t << decl << ": " << impl << endl << endl;
    }
}

void
MakefileGenerator::writeLexSrc(QTextStream &t, const QString &src)
{
    QStringList &l = project->variables()[src];
    if(project->isActiveConfig("yacc_no_name_mangle") && l.count() > 1)
	warn_msg(WarnLogic, "yacc_no_name_mangle specified, but multiple parsers expected.\n"
		 "This can lead to link problems.\n");
    QString default_out_c = "lex.$base.c";
    if(!project->isEmpty("QMAKE_LEX_SOURCE"))
	default_out_c = project->first("QMAKE_LEX_SOURCE");
    QString stringBase("$base");
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QFileInfo fi((*it));
	QString dir;
	if(fi.dirPath() != ".")
	    dir = fi.dirPath() + Option::dir_sep;
	dir = fileFixify(dir, QDir::currentDirPath(), Option::output_dir);
	if(!dir.isEmpty() && dir.right(Option::dir_sep.length()) != Option::dir_sep)
	    dir += Option::dir_sep;
	QString impl = dir + fi.baseName(TRUE) + Option::lex_mod + Option::cpp_ext.first();

	QString lexflags = "$(LEXFLAGS)", stub="yy";
	if(!project->isActiveConfig("yacc_no_name_mangle")) {
	    stub = fi.baseName(TRUE);
	    lexflags += " -P" + stub;
	}
	QString out_c = default_out_c;
	if(!stub.isEmpty())
	    out_c.replace(stringBase, stub);

	t << impl << ": " << (*it) << " " << findDependencies((*it)).join(" \\\n\t\t") << "\n\t"
	  << ( "$(LEX) " + lexflags + " " ) << (*it) << "\n\t"
	  << "-$(DEL_FILE) " << impl << " " << "\n\t"
	  << "-$(MOVE) " << out_c << " " << impl << endl << endl;
    }
}

void
MakefileGenerator::writeImageObj(QTextStream &t, const QString &obj)
{
    QStringList &objl = project->variables()[obj];
    QString stringSrc("$src");
    QString stringObj("$obj");

    QString uidir;
    for(QStringList::Iterator oit = objl.begin(); oit != objl.end(); oit++) {
        QString src(project->first("QMAKE_IMAGE_COLLECTION"));
	t << (*oit) << ": " << src;
	bool use_implicit_rule = !project->isEmpty("QMAKE_RUN_CXX_IMP");
	if(use_implicit_rule) {
	    if(!project->isEmpty("OBJECTS_DIR") || !project->isEmpty("UI_DIR") || !project->isEmpty("UI_SOURCES_DIR")) {
		use_implicit_rule = FALSE;
	    } else {
		int dot = src.findRev('.');
		if(dot == -1 || (src.left(dot) + Option::obj_ext != (*oit)))
		    use_implicit_rule = FALSE;
	    }
	}
	if(!use_implicit_rule && !project->isEmpty("QMAKE_RUN_CXX")) {
	    QString p = var("QMAKE_RUN_CXX"), srcf(src);
	    p.replace(stringSrc, srcf);
	    p.replace(stringObj, (*oit));
	    t << "\n\t" << p;
	}
	t << endl << endl;
    }
}


void
MakefileGenerator::writeImageSrc(QTextStream &t, const QString &src)
{
    QStringList &l = project->variables()[src];
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QString gen = project->first("MAKEFILE_GENERATOR");
	if ( gen == "MSVC" ) {
	    t << (*it) << ": " << findDependencies((*it)).join(" \\\n\t\t") << "\n\t"
		<< "$(UIC)  -o " << (*it) << " -embed " << project->first("QMAKE_ORIG_TARGET")
		<< " -f <<\n" << findDependencies((*it)).join(" ") << "\n<<" << endl << endl;
	} else if ( gen == "BMAKE" ) {
	    t << (*it) << ": " << findDependencies((*it)).join(" \\\n\t\t") << "\n\t"
		<< "$(UIC) " << " -embed " << project->first("QMAKE_ORIG_TARGET")
		<< " -f &&|\n" << findDependencies((*it)).join(" ") << "\n| -o " << (*it) << endl << endl;
	} else {
	    t << (*it) << ": " << findDependencies((*it)).join(" \\\n\t\t") << "\n\t"
		<< "$(UIC) " << " -embed " << project->first("QMAKE_ORIG_TARGET")
		<< " " << findDependencies((*it)).join(" ") << " -o " << (*it) << endl << endl;
	}
    }
}


void
MakefileGenerator::writeInstalls(QTextStream &t, const QString &installs)
{
    QString all_installs, all_uninstalls;
    QStringList &l = project->variables()[installs];
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QString pvar = (*it) + ".path";
	if(project->variables()[(*it) + ".CONFIG"].findIndex("no_path") == -1 &&
	   project->variables()[pvar].isEmpty()) {
	    warn_msg(WarnLogic, "%s is not defined: install target not created\n", pvar.latin1());
	    continue;
	}

	bool do_default = TRUE;
	const QString root = "$(INSTALL_ROOT)";
	QString target, dst= fileFixify(project->variables()[pvar].first());
	if(dst.right(1) != Option::dir_sep)
	    dst += Option::dir_sep;
	QStringList tmp, uninst = project->variables()[(*it) + ".uninstall"];
	//other
	tmp = project->variables()[(*it) + ".extra"];
	if(tmp.isEmpty())
	    tmp = project->variables()[(*it) + ".commands"]; //to allow compatible name
	if(!tmp.isEmpty()) {
	    do_default = FALSE;
	    if(!target.isEmpty())
		target += "\n\t";
	    target += tmp.join(" ");
	}
	//masks
	tmp = project->variables()[(*it) + ".files"];
	if(!tmp.isEmpty()) {
	    if(!target.isEmpty())
		target += "\n";
	    do_default = FALSE;
	    for(QStringList::Iterator wild_it = tmp.begin(); wild_it != tmp.end(); ++wild_it) {
		QString wild = Option::fixPathToLocalOS((*wild_it), FALSE), wild_var = fileFixify(wild);
		QString dirstr = QDir::currentDirPath(), filestr = wild;
		int slsh = filestr.findRev(Option::dir_sep);
		if(slsh != -1) {
		    dirstr = filestr.left(slsh+1);
		    filestr = filestr.right(filestr.length() - slsh - 1);
		}
		if(dirstr.right(Option::dir_sep.length()) != Option::dir_sep)
		    dirstr += Option::dir_sep;
		if(QFile::exists(wild)) { //real file
		    QString file = wild;
		    QFileInfo fi(wild);
		    if(!target.isEmpty())
			target += "\t";
		    QString cmd =  QString(fi.isDir() ? "-$(INSTALL_DIR)" : "-$(INSTALL_FILE)") + " \"" +
				   Option::fixPathToTargetOS(fileFixify(wild, QString::null,
									QString::null, FALSE, FALSE), FALSE) +
				   "\" \"" + root + dst + "\"\n";
		    target += cmd;
		    if(!project->isActiveConfig("debug") &&
		       !fi.isDir() && fi.isExecutable() && !project->isEmpty("QMAKE_STRIP"))
			target += QString("\t-") + var("QMAKE_STRIP") + " \"" +
				  root + fileFixify(dst + filestr, QString::null, QString::null, FALSE, FALSE) +
				  "\"\n";
		    if(!uninst.isEmpty())
			uninst.append("\n\t");
		    uninst.append(
#ifdef Q_WS_WIN
		    QString("-$(DEL_FILE)")
#else
		    QString("-$(DEL_FILE) -r")
#endif
		    + " \"" + root + fileFixify(dst + filestr, QString::null, QString::null, FALSE, FALSE) + "\"");
		    continue;
		}
		fixEnvVariables(dirstr);
		QDir dir(dirstr, filestr);		    //wild
		for(uint x = 0; x < dir.count(); x++) {
		    QString file = dir[x];
		    if(file == "." || file == "..") //blah
			continue;
		    if(!uninst.isEmpty())
			uninst.append("\n\t");
		    uninst.append(
#ifdef Q_WS_WIN
			QString("-$(DEL_FILE)")
#else
			QString("-$(DEL_FILE) -r")
#endif
			+ " \"" + root + fileFixify(dst + file, QString::null, QString::null, FALSE, FALSE) +
			"\"");
		    QFileInfo fi(Option::fixPathToTargetOS(fileFixify(dirstr + file), TRUE));
		    if(!target.isEmpty())
			target += "\t";
		    QString cmd = QString(fi.isDir() ? "-$(INSTALL_DIR)" : "-$(INSTALL_FILE)") + " \"" +
				  Option::fixPathToTargetOS(fileFixify(dirstr + file, QString::null,
								       QString::null, FALSE, FALSE), FALSE) +
				  "\" \"" + root + dst + "\"\n";
		    target += cmd;
		    if(!project->isActiveConfig("debug") &&
		       !fi.isDir() && fi.isExecutable() && !project->isEmpty("QMAKE_STRIP"))
			target += QString("\t-") + var("QMAKE_STRIP") + " \"" +
				  root + fileFixify(dst + file, QString::null, QString::null, FALSE, FALSE) +
				  "\"\n";
		}
	    }
	}
	//default?
	if(do_default) {
	    target = defaultInstall((*it));
	    uninst = project->variables()[(*it) + ".uninstall"];
	}

	if(!target.isEmpty()) {
	    t << "install_" << (*it) << ": all ";
	    const QStringList &deps = project->variables()[(*it) + ".depends"];
	    if(!deps.isEmpty()) {
		for(QStringList::ConstIterator dep_it = deps.begin(); dep_it != deps.end(); ++dep_it) {
		    QString targ = var((*dep_it) + ".target");
		    if(targ.isEmpty())
			targ = (*dep_it);
		    t << targ;
		}
	    }
	    t << "\n\t";
	    const QStringList &dirs = project->variables()[pvar];
	    for(QStringList::ConstIterator pit = dirs.begin(); pit != dirs.end(); ++pit) {
		QString tmp_dst = fileFixify((*pit));
#ifndef Q_WS_WIN
		if(tmp_dst.right(1) != Option::dir_sep)
		    tmp_dst += Option::dir_sep;
#endif
		t << mkdir_p_asstring(root+tmp_dst) << "\n\t";
	    }
	    t << target << endl << endl;
	    if(!uninst.isEmpty()) {
		t << "uninstall_" << (*it) << ": " << "\n\t"
		  << uninst.join("") << "\n\t"
		  << "-$(DEL_DIR) \"" << ( root + dst ) << "\"" << endl << endl;
	    }
	    t << endl;

	    if(project->variables()[(*it) + ".CONFIG"].findIndex("no_default_install") == -1) {
		all_installs += QString("install_") + (*it) + " ";
		if(!uninst.isEmpty())
		    all_uninstalls += "uninstall_" + (*it) + " ";
	    }
	}   else {
	    debug_msg(1, "no definition for install %s: install target not created",(*it).latin1());
	}
    }
    t << "install: " << all_installs << " " << var("INSTALLDEPS")   << "\n\n";
    t << "uninstall: " << all_uninstalls << " " << var("UNINSTALLDEPS") << "\n\n";
}

QString
MakefileGenerator::var(const QString &var)
{
    return val(project->variables()[var]);
}

QString
MakefileGenerator::val(const QStringList &varList)
{
    return valGlue(varList, "", " ", "");
}

QString
MakefileGenerator::varGlue(const QString &var, const QString &before, const QString &glue, const QString &after)
{
    return valGlue(project->variables()[var], before, glue, after);
}

QString
MakefileGenerator::valGlue(const QStringList &varList, const QString &before, const QString &glue, const QString &after)
{
    QString ret;
    for(QStringList::ConstIterator it = varList.begin(); it != varList.end(); ++it) {
	if(!(*it).isEmpty()) {
	    if(!ret.isEmpty())
		ret += glue;
	    ret += (*it);
	}
    }
    return ret.isEmpty() ? QString("") : before + ret + after;
}


QString
MakefileGenerator::varList(const QString &var)
{
    return valList(project->variables()[var]);
}

QString
MakefileGenerator::valList(const QStringList &varList)
{
    return valGlue(varList, "", " \\\n\t\t", "");
}


QStringList
MakefileGenerator::createObjectList(const QString &var)
{
    QStringList &l = project->variables()[var], ret;
    QString objdir, dir;
    if(!project->variables()["OBJECTS_DIR"].isEmpty())
	objdir = project->first("OBJECTS_DIR");
    for(QStringList::Iterator it = l.begin(); it != l.end(); ++it) {
	QFileInfo fi(Option::fixPathToLocalOS((*it)));
	if(objdir.isEmpty() && project->isActiveConfig("object_with_source")) {
	    QString fName = Option::fixPathToTargetOS((*it), FALSE);
	    int dl = fName.findRev(Option::dir_sep);
	    if(dl != -1)
		dir = fName.left(dl + 1);
	} else {
	    dir = objdir;
	}
	ret.append(dir + fi.baseName(TRUE) + Option::obj_ext);
    }
    return ret;
}

bool
MakefileGenerator::writeMakefile(QTextStream &t)
{
    t << "####### Compile" << endl << endl;
    writeObj(t, "OBJECTS", "SOURCES");
    writeUicSrc(t, "FORMS");
    writeObj(t, "UICOBJECTS", "UICIMPLS");
    writeMocObj(t, "OBJMOC", "SRCMOC" );
    writeMocSrc(t, "HEADERS");
    writeMocSrc(t, "SOURCES");
    writeMocSrc(t, "UICDECLS");
    writeYaccSrc(t, "YACCSOURCES");
    writeLexSrc(t, "LEXSOURCES");
    writeImageObj(t, "IMAGEOBJECTS");
    writeImageSrc(t, "QMAKE_IMAGE_COLLECTION");

    t << "####### Install" << endl << endl;
    writeInstalls(t, "INSTALLS");
    return TRUE;
}

QString MakefileGenerator::buildArgs()
{
    static QString ret;
    if(ret.isEmpty()) {
	//special variables
	if(!project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH"))
	    ret += " QMAKE_ABSOLUTE_SOURCE_PATH=\"" + project->first("QMAKE_ABSOLUTE_SOURCE_PATH") + "\"";

	//warnings
	else if(Option::warn_level == WarnNone)
	    ret += " -Wnone";
	else if(Option::warn_level == WarnAll)
	    ret += " -Wall";
	else if(Option::warn_level & WarnParser)
		ret += " -Wparser";
	//other options
	if(!Option::user_template.isEmpty())
	    ret += " -t " + Option::user_template;
	if(!Option::mkfile::do_cache)
	    ret += " -nocache";
	if(!Option::mkfile::do_deps)
	    ret += " -nodepend";
	if(!Option::mkfile::do_mocs)
	    ret += " -nomoc";
	if(!Option::mkfile::do_dep_heuristics)
	    ret += " -nodependheuristics";
	if(!Option::mkfile::qmakespec_commandline.isEmpty())
	    ret += " -spec " + Option::mkfile::qmakespec_commandline;

	//arguments
	for(QStringList::Iterator it = Option::before_user_vars.begin();
	    it != Option::before_user_vars.end(); ++it) {
	    if((*it).left(qstrlen("QMAKE_ABSOLUTE_SOURCE_PATH")) != "QMAKE_ABSOLUTE_SOURCE_PATH")
		ret += " \"" + (*it) + "\"";
	}
	if(Option::after_user_vars.count()) {
	    ret += " -after ";
	    for(QStringList::Iterator it = Option::after_user_vars.begin();
		it != Option::after_user_vars.end(); ++it) {
		if((*it).left(qstrlen("QMAKE_ABSOLUTE_SOURCE_PATH")) != "QMAKE_ABSOLUTE_SOURCE_PATH")
		    ret += " \"" + (*it) + "\"";
	    }
	}
    }
    return ret;
}

//could get stored argv, but then it would have more options than are
//probably necesary this will try to guess the bare minimum..
QString MakefileGenerator::build_args()
{
    static QString ret;
    if(ret.isEmpty()) {
	ret = "$(QMAKE)";

	// general options and arguments
	ret += buildArgs();

	//output
	QString ofile = Option::fixPathToTargetOS(fileFixify(Option::output.name()));
	if (!ofile.isEmpty() && ofile != project->first("QMAKE_MAKEFILE"))
	    ret += " -o " + ofile;

	//inputs
	QStringList files = fileFixify(Option::mkfile::project_files);
	ret += " " + files.join(" ");
    }
    return ret;
}

bool
MakefileGenerator::writeHeader(QTextStream &t)
{
    time_t foo = time(NULL);
    t << "#############################################################################" << endl;
    t << "# Makefile for building: " << var("TARGET") << endl;
    t << "# Generated by qmake (" << qmake_version() << ") (Qt " << QT_VERSION_STR << ") on: " << ctime(&foo);
    t << "# Project:  " << fileFixify(project->projectFile()) << endl;
    t << "# Template: " << var("TEMPLATE") << endl;
    t << "# Command: " << build_args() << endl;
    t << "#############################################################################" << endl;
    t << endl;
    return TRUE;
}


//makes my life easier..
bool
MakefileGenerator::writeMakeQmake(QTextStream &t)
{
    QString ofile = Option::fixPathToTargetOS(fileFixify(Option::output.name()));
    if(project->isEmpty("QMAKE_FAILED_REQUIREMENTS") && !project->isActiveConfig("no_autoqmake") &&
       !project->isEmpty("QMAKE_INTERNAL_PRL_FILE")) {
	QStringList files = fileFixify(Option::mkfile::project_files);
	t << project->first("QMAKE_INTERNAL_PRL_FILE") << ": " << "\n\t"
	  << "@$(QMAKE) -prl " << buildArgs() << " " << files.join(" ") << endl;
    }

    QString pfile = project->projectFile();
    if(pfile != "(stdin)") {
	QString qmake = build_args();
	if(!ofile.isEmpty() && !project->isActiveConfig("no_autoqmake")) {
	    t << ofile << ": " << fileFixify(pfile) << " ";
	    if(Option::mkfile::do_cache)
		t <<  fileFixify(Option::mkfile::cachefile) << " ";
	    if(!specdir().isEmpty()) {
		if (QFile::exists(Option::fixPathToLocalOS(specdir()+QDir::separator()+"qmake.conf")))
		    t << specdir() << Option::dir_sep << "qmake.conf" << " ";
		else if (QFile::exists(Option::fixPathToLocalOS(specdir()+QDir::separator()+"tmake.conf")))
		    t << specdir() << Option::dir_sep << "tmake.conf" << " ";
	    }
	    t << project->variables()["QMAKE_INTERNAL_INCLUDED_FILES"].join(" \\\n\t\t") << "\n\t"
	      << qmake <<endl;
	}
	if(project->first("QMAKE_ORIG_TARGET") != "qmake") {
	    t << "qmake: " <<
		project->variables()["QMAKE_INTERNAL_QMAKE_DEPS"].join(" \\\n\t\t") << "\n\t"
	      << "@" << qmake << endl << endl;
	}
    }
    return TRUE;
}

QStringList
MakefileGenerator::fileFixify(const QStringList& files, const QString &out_dir, const QString &in_dir,
			      bool force_fix, bool canon) const
{
    if(files.isEmpty())
	return files;
    QStringList ret;
    for(QStringList::ConstIterator it = files.begin(); it != files.end(); ++it) {
	if(!(*it).isEmpty())
	    ret << fileFixify((*it), out_dir, in_dir, force_fix, canon);
    }
    return ret;
}

QString
MakefileGenerator::fileFixify(const QString& file0, const QString &out_d,
			      const QString &in_d, bool force_fix, bool canon) const
{
    if(file0.isEmpty())
	return file0;
    QString key = file0;
    if(QDir::isRelativePath(file0))
	key.prepend(QDir::currentDirPath() + "--");
    if(!in_d.isEmpty() || !out_d.isEmpty() || force_fix || !canon)
	key.prepend(in_d + "--" + out_d + "--" + QString::number((int)force_fix) + "--" +
		    QString::number((int)canon) + "-");
    if(fileFixed.contains(key))
	return fileFixed[key];

    QString file = file0;
    int depth = 4;
    if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE ||
       Option::qmake_mode == Option::QMAKE_GENERATE_PRL) {
	if(project && !project->isEmpty("QMAKE_PROJECT_DEPTH"))
	    depth = project->first("QMAKE_PROJECT_DEPTH").toInt();
	else if(Option::mkfile::cachefile_depth != -1)
	    depth = Option::mkfile::cachefile_depth;
    }

    QChar quote;
    if((file.startsWith("'") || file.startsWith("\"")) && file.startsWith(file.right(1))) {
	quote = file.at(0);
	file = file.mid(1, file.length() - 2);
    }
    QString orig_file = file;
    if(!force_fix && project->isActiveConfig("no_fixpath")) {
	if(!project->isEmpty("QMAKE_ABSOLUTE_SOURCE_PATH")) { //absoluteify it
	    QString qfile = Option::fixPathToLocalOS(file, TRUE, canon);
	    if(QDir::isRelativePath(file)) { //already absolute
		QFileInfo fi(qfile);
		if(!fi.convertToAbs()) //strange
		    file = fi.filePath();
	    }
	}
    } else { //fix it..
	QString qfile(Option::fixPathToLocalOS(file, TRUE, canon)), in_dir(in_d), out_dir(out_d);
	{
	    if(out_dir.isNull() || QDir::isRelativePath(out_dir))
		out_dir.prepend(Option::output_dir + QDir::separator());
	    if(out_dir == ".")
		out_dir = QDir::currentDirPath();
	    if(in_dir.isEmpty() || QDir::isRelativePath(in_dir))
		in_dir.prepend(QDir::currentDirPath() + QDir::separator());
	    if(in_dir == ".")
		in_dir = QDir::currentDirPath();

	    if(!QDir::isRelativePath(in_dir) || !QDir::isRelativePath(out_dir)) {
		QFileInfo in_fi(in_dir);
		if(!in_fi.convertToAbs())
		    in_dir = in_fi.filePath();
		QFileInfo out_fi(out_dir);
		if(!out_fi.convertToAbs())
		    out_dir = out_fi.filePath();
	    }
	    QString in_canonical_dir = QDir(in_dir).canonicalPath(),
		   out_canonical_dir = QDir(out_dir).canonicalPath();
	    if(!in_canonical_dir.isEmpty())
		in_dir = in_canonical_dir;
	    if(!out_canonical_dir.isEmpty())
		out_dir = out_canonical_dir;
	}
	if(out_dir != in_dir || !QDir::isRelativePath(qfile)) {
	    if(QDir::isRelativePath(qfile)) {
		if(file.left(Option::dir_sep.length()) != Option::dir_sep &&
		   in_dir.right(Option::dir_sep.length()) != Option::dir_sep)
		    file.prepend(Option::dir_sep);
		file.prepend(in_dir);
	    }
	    file = Option::fixPathToTargetOS(file, FALSE, canon);
	    if(canon && QFile::exists(file) && file == Option::fixPathToTargetOS(file, TRUE, canon)) {
		QString real_file = QDir(file).canonicalPath();
		if(!real_file.isEmpty())
                    file = Option::fixPathToTargetOS(real_file, false, canon);
	    }
	    QString match_dir = Option::fixPathToTargetOS(out_dir, FALSE, canon);
	    if(file == match_dir) {
		file = "";
	    } else if(file.startsWith(match_dir) &&
	       file.mid(match_dir.length(), Option::dir_sep.length()) == Option::dir_sep) {
		file = file.right(file.length() - (match_dir.length() + 1));
	    } else {
		for(int i = 1; i <= depth; i++) {
		    int sl = match_dir.findRev(Option::dir_sep);
		    if(sl == -1)
			break;
		    match_dir = match_dir.left(sl);
		    if(match_dir.isEmpty())
			break;
		    if(file.startsWith(match_dir) &&
		       file.mid(match_dir.length(), Option::dir_sep.length()) == Option::dir_sep) {
			//concat
			int remlen = file.length() - (match_dir.length() + 1);
			if (remlen < 0)
			    remlen = 0;
			file = file.right(remlen);
			//prepend
			for(int o = 0; o < i; o++)
			    file.prepend(".." + Option::dir_sep);
		    }
		}
	    }
	}
    }
    file = Option::fixPathToTargetOS(file, FALSE, canon);
    if(file.isEmpty())
	file = ".";
    if(!quote.isNull())
	file = quote + file + quote;
    debug_msg(3, "Fixed %s :: to :: %s (%d) [%s::%s]", orig_file.latin1(), file.latin1(), depth,
	      in_d.latin1(), out_d.latin1());
    ((MakefileGenerator*)this)->fileFixed.insert(key, file);
    return file;
}

QString
MakefileGenerator::cleanFilePath(const QString &file) const
{
    return fileFixify(Option::fixPathToTargetOS(file));
}

void MakefileGenerator::logicWarn(const QString &f, const QString &w)
{
    if(!(Option::warn_level & WarnLogic))
	return;
    QString file = f;
    int slsh = f.findRev(Option::dir_sep);
    if(slsh != -1)
	file = file.right(file.length() - slsh - 1);
    QStringList &l = project->variables()[w];
    for(QStringList::Iterator val_it = l.begin(); val_it != l.end(); ++val_it) {
	QString file2((*val_it));
	slsh = file2.findRev(Option::dir_sep);
	if(slsh != -1)
	    file2 = file2.right(file2.length() - slsh - 1);
	if(file2 == file) {
	    warn_msg(WarnLogic, "Found potential symbol conflict of %s (%s) in %s",
		     file.latin1(), (*val_it).latin1(), w.latin1());
	    break;
	}
    }
}

QString 
MakefileGenerator::dependencyKey(const QString &file) const
{
     QString key = file;
     Option::fixPathToTargetOS(key);
     if(key.find(Option::dir_sep))
 	key = key.right(key.length() - key.findRev(Option::dir_sep) - 1);
     return key;
}

void 
MakefileGenerator::setProcessedDependencies(const QString &file, bool b)
{
    depProcessed[dependencyKey(file)] = b;
}

bool 
MakefileGenerator::processedDependencies(const QString &file)
{
    QString key = dependencyKey(file);
    if(!depProcessed.contains(key))
	return FALSE;
    return depProcessed[key];
}

QStringList
&MakefileGenerator::findDependencies(const QString &file)
{
    return depends[dependencyKey(file)];
}


QString
MakefileGenerator::specdir()
{
    if(!spec.isEmpty())
	return spec;
    spec = Option::mkfile::qmakespec;
#if 0
    if(const char *d = getenv("QTDIR")) {
	QString qdir = Option::fixPathToTargetOS(QString(d));
	if(qdir.endsWith(QString(QChar(QDir::separator()))))
	    qdir.truncate(qdir.length()-1);
	//fix path
	QFileInfo fi(spec);
	QString absSpec(fi.absFilePath());
	absSpec = Option::fixPathToTargetOS(absSpec);
	//replace what you can
	if(absSpec.startsWith(qdir)) {
	    absSpec.replace(0, qdir.length(), "$(QTDIR)");
	    spec = absSpec;
	}
    }
#else
    spec = Option::fixPathToTargetOS(spec);
#endif
    return spec;
}

bool
MakefileGenerator::openOutput(QFile &file) const
{
    {
	QString outdir;
	if(!file.name().isEmpty()) {
	    if(QDir::isRelativePath(file.name()))
		file.setName(Option::output_dir + file.name()); //pwd when qmake was run
	    QFileInfo fi(file);
	    if(fi.isDir())
		outdir = file.name() + QDir::separator();
	}
	if(!outdir.isEmpty() || file.name().isEmpty()) {
	    QString fname = "Makefile";
	    if(!project->isEmpty("MAKEFILE"))
	       fname = project->first("MAKEFILE");
	    file.setName(outdir + fname);
	}
    }
    if(QDir::isRelativePath(file.name()))
	file.setName(Option::output_dir + file.name()); //pwd when qmake was run
    if(project->isEmpty("QMAKE_MAKEFILE"))
	project->variables()["QMAKE_MAKEFILE"].append(file.name());
    int slsh = file.name().findRev(Option::dir_sep);
    if(slsh != -1)
	createDir(file.name().left(slsh));
    if(file.open(IO_WriteOnly | IO_Translate)) {
	QFileInfo fi(Option::output);
	QString od = Option::fixPathToTargetOS((fi.isSymLink() ? fi.readLink() : fi.dirPath()) );
	if(QDir::isRelativePath(od))
	    od.prepend(Option::output_dir);
	Option::output_dir = od;
	return TRUE;
    }
    return FALSE;
}



//Factory thing
#include "unixmake.h"
#include "msvc_nmake.h"
#include "borland_bmake.h"
#include "mingw_make.h"
#include "msvc_dsp.h"
#include "msvc_vcproj.h"
#include "metrowerks_xml.h"
#include "pbuilder_pbx.h"
#include "projectgenerator.h"

MakefileGenerator *
MakefileGenerator::create(QMakeProject *proj)
{
    if(Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT)
	return new ProjectGenerator(proj);

    MakefileGenerator *mkfile = NULL;
    QString gen = proj->first("MAKEFILE_GENERATOR");
    if(gen.isEmpty()) {
	fprintf(stderr, "No generator specified in config file: %s\n",
		proj->projectFile().latin1());
    } else if(gen == "UNIX") {
	mkfile = new UnixMakefileGenerator(proj);
    } else if(gen == "MSVC") {
	// Visual Studio =< v6.0
	if(proj->first("TEMPLATE").find(QRegExp("^vc.*")) != -1)
	    mkfile = new DspMakefileGenerator(proj);
	else
	    mkfile = new NmakeMakefileGenerator(proj);
    } else if(gen == "MSVC.NET") {
	// Visual Studio >= v7.0
	if(proj->first("TEMPLATE").find(QRegExp("^vc.*")) != -1)
	    mkfile = new VcprojGenerator(proj);
	else
	    mkfile = new NmakeMakefileGenerator(proj);
    } else if(gen == "BMAKE") {
	mkfile = new BorlandMakefileGenerator(proj);
    } else if(gen == "MINGW") {
	mkfile = new MingwMakefileGenerator(proj);
    } else if(gen == "METROWERKS") {
	mkfile = new MetrowerksMakefileGenerator(proj);
    } else if(gen == "PROJECTBUILDER") {
	mkfile = new ProjectBuilderMakefileGenerator(proj);
    } else {
	fprintf(stderr, "Unknown generator specified: %s\n", gen.latin1());
    }
    return mkfile;
}
