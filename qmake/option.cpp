/****************************************************************************
**
** Implementation of Option class.
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

#include "option.h"
#include <qdir.h>
#include <qregexp.h>
#include <stdlib.h>
#include <stdarg.h>

//convenience
QString Option::prf_ext;
QString Option::prl_ext;
QString Option::libtool_ext;
QString Option::pkgcfg_ext;
QString Option::ui_ext;
QStringList Option::h_ext;
QString Option::cpp_moc_ext;
QString Option::h_moc_ext;
QStringList Option::cpp_ext;
QString Option::obj_ext;
QString Option::lex_ext;
QString Option::yacc_ext;
QString Option::dir_sep;
QString Option::h_moc_mod;
QString Option::cpp_moc_mod;
QString Option::yacc_mod;
QString Option::lex_mod;

//mode
Option::QMAKE_MODE Option::qmake_mode = Option::QMAKE_GENERATE_NOTHING;

//all modes
int Option::warn_level = WarnLogic;
int Option::debug_level = 0;
QFile Option::output;
QString Option::output_dir;
QStringList Option::before_user_vars;
QStringList Option::after_user_vars;
QString Option::user_template;
QString Option::user_template_prefix;
#if defined(Q_OS_WIN32)
Option::TARG_MODE Option::target_mode = Option::TARG_WIN_MODE;
#elif defined(Q_OS_MAC9)
Option::TARG_MODE Option::target_mode = Option::TARG_MAC9_MODE;
#elif defined(Q_OS_MACX)
Option::TARG_MODE Option::target_mode = Option::TARG_MACX_MODE;
#elif defined(Q_OS_QNX6)
Option::TARG_MODE Option::target_mode = Option::TARG_QNX6_MODE;
#else
Option::TARG_MODE Option::target_mode = Option::TARG_UNIX_MODE;
#endif

//QMAKE_*_PROPERTY stuff
QStringList Option::prop::properties;

//QMAKE_GENERATE_PROJECT stuff
bool Option::projfile::do_pwd = TRUE;
bool Option::projfile::do_recursive = TRUE;
QStringList Option::projfile::project_dirs;

//QMAKE_GENERATE_MAKEFILE stuff
QString Option::mkfile::qmakespec;
int Option::mkfile::cachefile_depth = -1;
bool Option::mkfile::do_deps = TRUE;
bool Option::mkfile::do_mocs = TRUE;
bool Option::mkfile::do_dep_heuristics = TRUE;
bool Option::mkfile::do_preprocess = FALSE;
bool Option::mkfile::do_cache = TRUE;
QString Option::mkfile::cachefile;
QStringList Option::mkfile::project_files;
QString Option::mkfile::qmakespec_commandline;

static Option::QMAKE_MODE default_mode(QString progname)
{
    int s = progname.findRev(Option::dir_sep);
    if(s != -1)
	progname = progname.right(progname.length() - (s + 1));
    if(progname == "qmakegen")
	return Option::QMAKE_GENERATE_PROJECT;
    else if(progname == "qt-config")
	return Option::QMAKE_QUERY_PROPERTY;
    return Option::QMAKE_GENERATE_MAKEFILE;
}

QString project_builtin_regx();
bool usage(const char *a0)
{
    fprintf(stdout, "Usage: %s [mode] [options] [files]\n"
	    "\n"
	    "   QMake has two modes, one mode for generating project files based on\n"
	    "some heuristics, and the other for generating makefiles. Normally you\n"
	    "shouldn't need to specify a mode, as makefile generation is the default\n"
	    "mode for qmake, but you may use this to test qmake on an existing project\n"
	    "\n"
	    "Mode:\n"
	    "\t-project       Put qmake into project file generation mode%s\n"
	    "\t               In this mode qmake interprets files as files to\n"
	    "\t               be built,\n"
	    "\t               defaults to %s\n"
	    "\t-makefile      Put qmake into makefile generation mode%s\n"
	    "\t               In this mode qmake interprets files as project files to\n"
	    "\t               be processed, if skipped qmake will try to find a project\n"
	    "\t               file in your current working directory\n"
	    "\n"
	    "Warnings Options:\n"
	    "\t-Wnone         Turn off all warnings\n"
	    "\t-Wall          Turn on all warnings\n"
	    "\t-Wparser       Turn on parser warnings\n"
	    "\t-Wlogic        Turn on logic warnings\n"
	    "\n"
	    "Options:\n"
	    "\t * You can place any variable assignment in options and it will be     *\n"
	    "\t * processed as if it was in [files]. These assignments will be parsed *\n"
	    "\t * before [files].                                                     *\n"
	    "\t-o file        Write output to file\n"
	    "\t-unix          Run in unix mode\n"
	    "\t-win32         Run in win32 mode\n"
	    "\t-macx          Run in Mac OS X mode\n"
	    "\t-d             Increase debug level\n"
	    "\t-t templ       Overrides TEMPLATE as templ\n"
	    "\t-tp prefix     Overrides TEMPLATE so that prefix is prefixed into the value\n"
	    "\t-help          This help\n"
	    "\t-v             Version information\n"
	    "\t-after         All variable assignments after this will be\n"
	    "\t               parsed after [files]\n"
	    "\t-cache file    Use file as cache           [makefile mode only]\n"
	    "\t-spec spec     Use spec as QMAKESPEC       [makefile mode only]\n"
	    "\t-nocache       Don't use a cache file      [makefile mode only]\n"
	    "\t-nodepend      Don't generate dependencies [makefile mode only]\n"
	    "\t-nomoc         Don't generate moc targets  [makefile mode only]\n"
	    "\t-nopwd         Don't look for files in pwd [ project mode only]\n"
	    "\t-norecursive   Don't do a recursive search [ project mode only]\n"
	    ,a0, 
	    default_mode(a0) == Option::QMAKE_GENERATE_PROJECT  ? " (default)" : "", project_builtin_regx().latin1(),
	    default_mode(a0) == Option::QMAKE_GENERATE_MAKEFILE ? " (default)" : "");
    return FALSE;
}

enum {
    QMAKE_CMDLINE_SUCCESS,
    QMAKE_CMDLINE_SHOW_USAGE,
    QMAKE_CMDLINE_BAIL
};
int
Option::internalParseCommandLine(int argc, char **argv, int skip)
{
    bool before = TRUE;
    for(int x = skip; x < argc; x++) {
	if(*argv[x] == '-' && strlen(argv[x]) > 1) { /* options */
	    QString opt = argv[x] + 1;

	    //first param is a mode, or we default
	    if(x == 1) {
		bool specified = TRUE;
		if(opt == "project") {
		    Option::qmake_mode = Option::QMAKE_GENERATE_PROJECT;
		} else if(opt == "prl") {
		    Option::mkfile::do_deps = FALSE;
		    Option::mkfile::do_mocs = FALSE;
		    Option::qmake_mode = Option::QMAKE_GENERATE_PRL;
		} else if(opt == "set") {
		    Option::qmake_mode = Option::QMAKE_SET_PROPERTY;
		} else if(opt == "query") {
		    Option::qmake_mode = Option::QMAKE_QUERY_PROPERTY;
		} else if(opt == "makefile") {
		    Option::qmake_mode = Option::QMAKE_GENERATE_MAKEFILE;
		} else {
		    specified = FALSE;
		}
		if(specified)
		    continue;
	    }
	    //all modes
	    if(opt == "o" || opt == "output") {
		Option::output.setName(argv[++x]);
	    } else if(opt == "after") {
		before = FALSE;
	    } else if(opt == "t" || opt == "template") {
		Option::user_template = argv[++x];
	    } else if(opt == "tp" || opt == "template_prefix") {
		Option::user_template_prefix = argv[++x];
	    } else if(opt == "mac9") {
		Option::target_mode = TARG_MAC9_MODE;
	    } else if(opt == "macx") {
		Option::target_mode = TARG_MACX_MODE;
	    } else if(opt == "unix") {
		Option::target_mode = TARG_UNIX_MODE;
	    } else if(opt == "win32") {
		Option::target_mode = TARG_WIN_MODE;
	    } else if(opt == "d") {
		Option::debug_level++;
	    } else if(opt == "version" || opt == "v" || opt == "-version") {
		fprintf(stderr, "Qmake version: %s (Qt %s)\n", qmake_version(), QT_VERSION_STR);
		fprintf(stderr, "Qmake is free software from Trolltech ASA.\n");
		return QMAKE_CMDLINE_BAIL;
	    } else if(opt == "h" || opt == "help") {
		return QMAKE_CMDLINE_SHOW_USAGE;
	    } else if(opt == "Wall") {
		Option::warn_level |= WarnAll;
	    } else if(opt == "Wparser") {
		Option::warn_level |= WarnParser;
	    } else if(opt == "Wlogic") {
		Option::warn_level |= WarnLogic;
	    } else if(opt == "Wnone") {
		Option::warn_level = WarnNone;
	    } else {
		if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE ||
		   Option::qmake_mode == Option::QMAKE_GENERATE_PRL) {
		    if(opt == "nodepend") {
			Option::mkfile::do_deps = FALSE;
		    } else if(opt == "nomoc") {
			Option::mkfile::do_mocs = FALSE;
		    } else if(opt == "nocache") {
			Option::mkfile::do_cache = FALSE;
		    } else if(opt == "nodependheuristics") {
			Option::mkfile::do_dep_heuristics = FALSE;
		    } else if(opt == "E") {
			Option::mkfile::do_preprocess = TRUE;
		    } else if(opt == "cache") {
			Option::mkfile::cachefile = argv[++x];
		    } else if(opt == "platform" || opt == "spec") {
			Option::mkfile::qmakespec = argv[++x];
			Option::mkfile::qmakespec_commandline = argv[x];
		    } else {
			fprintf(stderr, "***Unknown option -%s\n", opt.latin1());
			return QMAKE_CMDLINE_SHOW_USAGE;
		    }
		} else if(Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT) {
		    if(opt == "nopwd") {
			Option::projfile::do_pwd = FALSE;
		    } else if(opt == "r") {
			Option::projfile::do_recursive = TRUE;
		    } else if(opt == "norecursive") {
			Option::projfile::do_recursive = FALSE;
		    } else {
			fprintf(stderr, "***Unknown option -%s\n", opt.latin1());
			return QMAKE_CMDLINE_SHOW_USAGE;
		    }
		}
	    }
	} else {
	    QString arg = argv[x];
	    if(arg.find('=') != -1) {
		if(before)
		    Option::before_user_vars.append(arg);
		else
		    Option::after_user_vars.append(arg);
	    } else {
		bool handled = TRUE;
		if(Option::qmake_mode == Option::QMAKE_QUERY_PROPERTY ||
		    Option::qmake_mode == Option::QMAKE_SET_PROPERTY) {
		    Option::prop::properties.append(arg);
		} else {
		    QFileInfo fi(arg);
		    if(!fi.convertToAbs()) //strange
			arg = fi.filePath();
		    if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE ||
		       Option::qmake_mode == Option::QMAKE_GENERATE_PRL)
			Option::mkfile::project_files.append(arg);
		    else if(Option::qmake_mode == Option::QMAKE_GENERATE_PROJECT)
			Option::projfile::project_dirs.append(arg);
		    else 
			handled = FALSE;
		}
		if(!handled)
		    return QMAKE_CMDLINE_SHOW_USAGE;
	    }
	}
    }
    return QMAKE_CMDLINE_SUCCESS;
}


bool
Option::parseCommandLine(int argc, char **argv)
{
    Option::cpp_moc_mod = "";
    Option::h_moc_mod = "moc_";
    Option::lex_mod = "_lex";
    Option::yacc_mod = "_yacc";
    Option::prl_ext = ".prl";
    Option::libtool_ext = ".la";
    Option::pkgcfg_ext = ".pc";
    Option::prf_ext = ".prf";
    Option::ui_ext = ".ui";
    Option::h_ext << ".h" << ".hpp" << ".hh" << ".H" << ".hxx";
    Option::cpp_moc_ext = ".moc";
    Option::h_moc_ext = ".cpp";
    Option::cpp_ext << ".cpp" << ".cc" << ".cxx" << ".C";
    Option::lex_ext = ".l";
    Option::yacc_ext = ".y";

    if(Option::qmake_mode == Option::QMAKE_GENERATE_NOTHING)
	Option::qmake_mode = default_mode(argv[0]);
    if(const char *envflags = getenv("QMAKEFLAGS")) {
	int env_argc = 0, env_size = 0, currlen=0;
	char quote = 0, **env_argv = NULL;
	for(int i = 0; envflags[i]; i++) {
	    if(!quote && (envflags[i] == '\'' || envflags[i] == '"')) {
		quote = envflags[i];
	    } else if(envflags[i] == quote) {
		quote = 0;
	    } else if(!quote && envflags[i] == ' ') {
		if(currlen && env_argv && env_argv[env_argc]) {
		    env_argv[env_argc][currlen] = '\0';
		    currlen = 0;
		    env_argc++;
		}
	    } else {
		if(!env_argv || env_argc > env_size) {
		    env_argv = (char **)realloc(env_argv, sizeof(char *)*(env_size+=10));
		    for(int i2 = env_argc; i2 < env_size; i2++)
			env_argv[i2] = NULL;
		}
		if(!env_argv[env_argc]) {
		    currlen = 0;
		    env_argv[env_argc] = (char*)malloc(255);
		}
		if(currlen < 255) 
		    env_argv[env_argc][currlen++] = envflags[i];
	    }
	}
	if(env_argv[env_argc]) {
	    env_argv[env_argc][currlen] = '\0';
	    currlen = 0;
	    env_argc++;
	}
	internalParseCommandLine(env_argc, env_argv);
	for(int i2 = 0; i2 < env_size; i2++) {
	    if(env_argv[i2])
		free(env_argv[i2]);
	}
	free(env_argv);
    }
    {
	int ret = internalParseCommandLine(argc, argv, 1);
	if(ret != QMAKE_CMDLINE_SUCCESS) 
	    return ret == QMAKE_CMDLINE_SHOW_USAGE ? usage(argv[0]) : FALSE;
    }

    //last chance for defaults
    if(Option::qmake_mode == Option::QMAKE_GENERATE_MAKEFILE ||
	Option::qmake_mode == Option::QMAKE_GENERATE_PRL) {
	if(Option::mkfile::qmakespec.isNull() || Option::mkfile::qmakespec.isEmpty())
	    Option::mkfile::qmakespec = getenv("QMAKESPEC");

	//try REALLY hard to do it for them, lazy..
	if(Option::mkfile::project_files.isEmpty()) {
	    QString pwd = QDir::currentDirPath(), 
		   proj = pwd + "/" + pwd.right(pwd.length() - (pwd.findRev('/') + 1)) + ".pro";
	    if(QFile::exists(proj)) {
		Option::mkfile::project_files.append(proj);
	    } else { //last try..
		QDir d(pwd, "*.pro");
		if(d.count() != 1)
		    return usage(argv[0]);
		Option::mkfile::project_files.append(pwd + "/" + d[0]);
	    }
	}
    }

    //defaults for globals
    if(Option::target_mode == Option::TARG_WIN_MODE) {
	Option::dir_sep = "\\";
	Option::obj_ext =  ".obj";
    } else {
	if(Option::target_mode == Option::TARG_MAC9_MODE)
	    Option::dir_sep = ":";
	else
	    Option::dir_sep = "/";
	Option::obj_ext = ".o";
    }
    return TRUE;
}

bool Option::postProcessProject(QMakeProject *project)
{
    Option::cpp_ext = project->variables()["QMAKE_EXT_CPP"];
    if(cpp_ext.isEmpty())
	cpp_ext << ".cpp"; //something must be there
    Option::h_ext = project->variables()["QMAKE_EXT_H"];
    if(h_ext.isEmpty())
	h_ext << ".h";

    if(!project->isEmpty("QMAKE_EXT_PKGCONFIG"))
	Option::pkgcfg_ext = project->first("QMAKE_EXT_PKGCONFIG");
    if(!project->isEmpty("QMAKE_EXT_LIBTOOL"))
	Option::libtool_ext = project->first("QMAKE_EXT_LIBTOOL");
    if(!project->isEmpty("QMAKE_EXT_PRL"))
	Option::prl_ext = project->first("QMAKE_EXT_PRL");
    if(!project->isEmpty("QMAKE_EXT_PRF"))
	Option::prf_ext = project->first("QMAKE_EXT_PRF");
    if(!project->isEmpty("QMAKE_EXT_UI"))
	Option::ui_ext = project->first("QMAKE_EXT_UI");
    if(!project->isEmpty("QMAKE_EXT_CPP_MOC"))
	Option::cpp_moc_ext = project->first("QMAKE_EXT_CPP_MOC");
    if(!project->isEmpty("QMAKE_EXT_H_MOC"))
	Option::h_moc_ext = project->first("QMAKE_EXT_H_MOC");
    if(!project->isEmpty("QMAKE_EXT_LEX"))
	Option::lex_ext = project->first("QMAKE_EXT_LEX");
    if(!project->isEmpty("QMAKE_EXT_YACC"))
	Option::yacc_ext = project->first("QMAKE_EXT_YACC");
    if(!project->isEmpty("QMAKE_EXT_OBJ"))
	Option::obj_ext = project->first("QMAKE_EXT_OBJ");
    if(!project->isEmpty("QMAKE_H_MOD_MOC"))
	Option::h_moc_mod = project->first("QMAKE_H_MOD_MOC");
    if(!project->isEmpty("QMAKE_CPP_MOD_MOC"))
	Option::cpp_moc_mod = project->first("QMAKE_CPP_MOD_MOC");
    if(!project->isEmpty("QMAKE_MOD_LEX"))
	Option::lex_mod = project->first("QMAKE_MOD_LEX");
    if(!project->isEmpty("QMAKE_MOD_YACC"))
	Option::yacc_mod = project->first("QMAKE_MOD_YACC");
    if(!project->isEmpty("QMAKE_DIR_SEP"))
	Option::dir_sep = project->first("QMAKE_DIR_SEP");
    return TRUE;
}

void fixEnvVariables(QString &x)
{
    int rep;
    QRegExp reg_var("\\$\\(.*\\)");
    reg_var.setMinimal( TRUE );
    while((rep = reg_var.search(x)) != -1)
	x.replace(rep, reg_var.matchedLength(), QString(getenv(x.mid(rep + 2, reg_var.matchedLength() - 3).latin1())));
}
static QString fixPath(QString x)
{
#if 0
    QFileInfo fi(x);
    if(fi.isDir()) {
	QDir dir(x);
	x = dir.canonicalPath();
    } else {
	QString dir = fi.dir().canonicalPath();
	if(!dir.isEmpty() && dir.right(1) != Option::dir_sep)
	    dir += Option::dir_sep;
	x = dir + fi.fileName();
    }
#endif
    return QDir::cleanDirPath(x);
}


QString
Option::fixPathToTargetOS(const QString& in, bool fix_env, bool canonical)
{
    QString tmp(in);
    if(fix_env)
	fixEnvVariables(tmp);
    if(canonical)
	tmp = fixPath(tmp);
    QString rep;
    if(Option::target_mode == TARG_MAC9_MODE) 
	tmp = tmp.replace('/', Option::dir_sep).replace('\\', Option::dir_sep);
    else if(Option::target_mode == TARG_WIN_MODE) 
	tmp = tmp.replace('/', Option::dir_sep);
    else 
	tmp = tmp.replace('\\', Option::dir_sep);
    return tmp;
}

QString
Option::fixPathToLocalOS(const QString& in, bool fix_env, bool canonical)
{
    QString tmp(in);
    if(fix_env)
	fixEnvVariables(tmp);
    if(canonical)
	tmp = fixPath(tmp);
#if defined(Q_OS_WIN32)
    return tmp.replace('/', '\\');
#else
    return tmp.replace('\\', '/');
#endif
}

const char *qmake_version()
{
    static char *ret = NULL;
    if(ret)
	return ret;
    ret = (char *)malloc(15);
    sprintf(ret, "%d.%02d%c", QMAKE_VERSION_MAJOR, QMAKE_VERSION_MINOR, 'a' + QMAKE_VERSION_PATCH);
    return ret;
}

void debug_msg(int level, const char *fmt, ...)
{
    if(Option::debug_level < level)
	return;
    fprintf(stderr, "DEBUG %d: ", level);
    {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
    }
    fprintf(stderr, "\n");
}

void warn_msg(QMakeWarn type, const char *fmt, ...)
{
    if(!(Option::warn_level & type))
	return;
    fprintf(stderr, "WARNING: ");
    {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
    }
    fprintf(stderr, "\n");
}
