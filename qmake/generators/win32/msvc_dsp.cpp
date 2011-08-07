/****************************************************************************
**
** Implementation of DspMakefileGenerator class.
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

#include "msvc_dsp.h"
#include "option.h"
#include <qdir.h>
#include <qregexp.h>
#include <stdlib.h>
#include <time.h>

DspMakefileGenerator::DspMakefileGenerator(QMakeProject *p) : Win32MakefileGenerator(p), init_flag(FALSE)
{

}

bool
DspMakefileGenerator::writeMakefile(QTextStream &t)
{
    if(!project->variables()["QMAKE_FAILED_REQUIREMENTS"].isEmpty()) {
	/* for now just dump, I need to generated an empty dsp or something.. */
	fprintf(stderr, "Project file not generated because all requirements not met:\n\t%s\n",
		var("QMAKE_FAILED_REQUIREMENTS").latin1());
	return TRUE;
    }

    if(project->first("TEMPLATE") == "vcapp" ||
       project->first("TEMPLATE") == "vclib") {
	return writeDspParts(t);
    }
    else if(project->first("TEMPLATE") == "subdirs") {
	writeHeader(t);
	writeSubDirs(t);
	return TRUE;
    }
    return FALSE;
}

bool
DspMakefileGenerator::writeDspParts(QTextStream &t)
{
    QString dspfile;
    if ( !project->variables()["DSP_TEMPLATE"].isEmpty() ) {
	dspfile = project->first("DSP_TEMPLATE");
    } else {
	dspfile = project->first("MSVCDSP_TEMPLATE");
    }
    if (dspfile.startsWith("\"") && dspfile.endsWith("\""))
	dspfile = dspfile.mid(1, dspfile.length() - 2);
    QString dspfile_loc = findTemplate(dspfile);

    QFile file(dspfile_loc);
    if(!file.open(IO_ReadOnly)) {
	fprintf(stderr, "Cannot open dsp file: %s\n", dspfile.latin1());
	return FALSE;
    }
    QTextStream dsp(&file);

    QString platform = "Win32";
    if ( !project->variables()["QMAKE_PLATFORM"].isEmpty() )
	platform = varGlue("QMAKE_PLATFORM", "", " ", "");

    // Setup PCH variables
    precompH = project->first("PRECOMPILED_HEADER");
    QString namePCH = QFileInfo(precompH).fileName();
    usePCH = !precompH.isEmpty() && project->isActiveConfig("precompile_header");
    if (usePCH) {
	// Created files
	QString origTarget = project->first("QMAKE_ORIG_TARGET");
	origTarget.replace(QRegExp("-"), "_");
	precompObj = "\"$(IntDir)\\" + origTarget + Option::obj_ext + "\"";
	precompPch = "\"$(IntDir)\\" + origTarget + ".pch\"";
	// Add PRECOMPILED_HEADER to HEADERS
	if (!project->variables()["HEADERS"].contains(precompH))
	    project->variables()["HEADERS"] += precompH;
	// Add precompile compiler options
	project->variables()["PRECOMPILED_FLAGS_REL"]  = "/Yu\"" + namePCH + "\" /FI\"" + namePCH + "\" ";
	project->variables()["PRECOMPILED_FLAGS_DEB"]  = "/Yu\"" + namePCH + "\" /FI\"" + namePCH + "\" ";
	// Return to variable pool
	project->variables()["PRECOMPILED_OBJECT"] = precompObj;
	project->variables()["PRECOMPILED_PCH"]    = precompPch;
    }
    int rep;
    QString line;
    while ( !dsp.eof() ) {
	line = dsp.readLine();
	while((rep = line.find(QRegExp("\\$\\$[a-zA-Z0-9_-]*"))) != -1) {
	    QString torep = line.mid(rep, line.find(QRegExp("[^\\$a-zA-Z0-9_-]"), rep) - rep);
	    QString variable = torep.right(torep.length()-2);

	    t << line.left(rep); //output the left side
	    line = line.right(line.length() - (rep + torep.length())); //now past the variable
	    if(variable == "MSVCDSP_SOURCES") {
		if(project->variables()["SOURCES"].isEmpty())
		    continue;

		QString mocpath = var( "QMAKE_MOC" );
		mocpath = mocpath.replace( QRegExp( "\\..*$" ), "" ) + " ";

		QStringList list = project->variables()["SOURCES"] + project->variables()["DEF_FILE"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		QStringList::Iterator it;
		for( it = list.begin(); it != list.end(); ++it) {
		    beginGroupForFile((*it), t);
		    t << "# Begin Source File\n\nSOURCE=" << (*it) << endl;
		    if (usePCH && (*it).endsWith(".c"))
			t << "# SUBTRACT CPP /FI\"" << namePCH << "\" /Yu\"" << namePCH << "\" /Fp" << endl;
		    if ( project->isActiveConfig("moc") && (*it).endsWith(Option::cpp_moc_ext)) {
			QString base = (*it);
			base.replace(QRegExp("\\..*$"), "").upper();
			base.replace(QRegExp("[^a-zA-Z]"), "_");

			QString build = "\n\n# Begin Custom Build - Moc'ing " + findMocSource((*it)) +
					"...\n" "InputPath=.\\" + (*it) + "\n\n" "\"" + (*it) + "\""
					" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n"
					"\t" + mocpath + findMocSource((*it)) + " -o " +
					(*it) + "\n\n" "# End Custom Build\n\n";

			t << "USERDEP_" << base << "=\".\\" << findMocSource((*it)) << "\" \"$(QTDIR)\\bin\\moc.exe\"" << endl << endl;

			t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build
			  << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\""
			  << build << "!ENDIF " << endl << endl;
		    }
		    t << "# End Source File" << endl;
		}
		endGroups(t);
	    } else if(variable == "MSVCDSP_IMAGES") {
		if(project->variables()["IMAGES"].isEmpty())
		    continue;
		t << "# Begin Source File\n\nSOURCE=" << project->first("QMAKE_IMAGE_COLLECTION") << endl;
		t << "# End Source File" << endl;
	    } else if(variable == "MSVCDSP_HEADERS") {
		if(project->variables()["HEADERS"].isEmpty())
		    continue;

		QStringList list = project->variables()["HEADERS"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
//		    beginGroupForFile((*it), t);
		    t << "# Begin Source File\n\nSOURCE=" << (*it) << endl << endl;
		    QString compilePCH;
		    QStringList customDependencies;
		    QString createMOC;
		    QString buildCmdsR, buildCmdsD;
		    QString buildCmds = "\nBuildCmds= \\\n";
		    // Create unique baseID
		    QString base = (*it);
		    {
			base.replace(QRegExp("\\..*$"), "").upper();
			base.replace(QRegExp("[^a-zA-Z]"), "_");
		    }
		    if (usePCH && precompH.endsWith(*it)) {
			QString basicBuildCmd = QString("\tcl.exe /TP /W3 /FD /c /D \"WIN32\" /Yc /Fp\"%1\" /Fo\"%2\" %3 %4 %5 %6 %7 %8 %9 /D \"")
							.arg(precompPch)
							.arg(precompObj)
							.arg(var("MSVCDSP_INCPATH"))
							.arg(var("MSVCDSP_DEFINES"))
							.arg(var("MSVCDSP_CXXFLAGS"));
			buildCmdsR = basicBuildCmd
					    .arg("/D \"NDEBUG\"")
					    .arg(var("QMAKE_CXXFLAGS_RELEASE"))
					    .arg(var("MSVCDSP_MTDEF"))
					    .arg(var("MSVCDSP_RELDEFS"));
			buildCmdsD = basicBuildCmd
					    .arg("/D \"_DEBUG\" /Od")
					    .arg(var("QMAKE_CXXFLAGS_DEBUG"))
					    .arg(var("MSVCDSP_MTDEFD"))
					    .arg(var("MSVCDSP_DEBUG_OPT"));
			if (project->first("TEMPLATE") == "vcapp") {	// App
			    buildCmdsR += var("MSVCDSP_WINCONDEF");
			    buildCmdsD += var("MSVCDSP_WINCONDEF");
			} else if (project->isActiveConfig("dll")) {	// Dll
			    buildCmdsR += "_WINDOWS\" /D \"_USRDLL";
			    buildCmdsD += "_WINDOWS\" /D \"_USRDLL";
			} else {					// Lib
			    buildCmdsR += "_LIB";
			    buildCmdsD += "_LIB";
			}
			buildCmdsR += "\" /Fd\"$(IntDir)\\\\\" " + (*it) + " \\\n";
			buildCmdsD += "\" /Fd\"$(IntDir)\\\\\" " + (*it) + " \\\n";

			compilePCH = precompPch + " : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n   $(BuildCmds)\n\n";

			QStringList &tmp = findDependencies(precompH);
			if(!tmp.isEmpty()) // Got Deps for PCH
			    customDependencies += tmp;
		    }
		    if (project->isActiveConfig("moc") && !findMocDestination((*it)).isEmpty()) {
			QString mocpath = var( "QMAKE_MOC" );
			mocpath = mocpath.replace( QRegExp( "\\..*$" ), "" ) + " ";
			buildCmds += "\t" + mocpath + (*it)  + " -o " + findMocDestination((*it)) + " \\\n";
			createMOC  = "\"" + findMocDestination((*it)) +	"\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n   $(BuildCmds)\n\n";
			customDependencies += "\"$(QTDIR)\\bin\\moc.exe\"";
		    }
		    if (!createMOC.isEmpty() || !compilePCH.isEmpty()) {
			bool doMOC = !createMOC.isEmpty();
			bool doPCH = !compilePCH.isEmpty();
			QString build = "\n\n# Begin Custom Build - "+ 
					QString(doMOC?"Moc'ing ":"") +
					QString((doMOC&&doPCH)?" and ":"") +
					QString(doPCH?"Creating PCH cpp from ":"") +
					(*it) + "...\nInputPath=.\\" + (*it) + "\n\n" +
					buildCmds + "%1\n" +
					createMOC + 
					compilePCH +
					"# End Custom Build\n\n";

			t << "USERDEP_" << base << "=" << valGlue(customDependencies, "\"", "\" \"", "\"") << endl << endl;
			t << "!IF  \"$(CFG)\" == \""     << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build.arg(buildCmdsR)
			  << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\""   << build.arg(buildCmdsD)
			  << "!ENDIF " << endl << endl;
		    }
		    t << "# End Source File" << endl;
		}
//		endGroups(t);
	    } else if(variable == "MSVCDSP_FORMSOURCES" || variable == "MSVCDSP_FORMHEADERS") {
		if(project->variables()["FORMS"].isEmpty())
		    continue;

		QString uiSourcesDir;
		QString uiHeadersDir;
		if(!project->variables()["UI_DIR"].isEmpty()) {
		    uiSourcesDir = project->first("UI_DIR");
		    uiHeadersDir = project->first("UI_DIR");
		} else {
		    if ( !project->variables()["UI_SOURCES_DIR"].isEmpty() )
			uiSourcesDir = project->first("UI_SOURCES_DIR");
		    else
			uiSourcesDir = "";
		    if ( !project->variables()["UI_HEADERS_DIR"].isEmpty() )
			uiHeadersDir = project->first("UI_HEADERS_DIR");
		    else
			uiHeadersDir = "";
		}

		QStringList list = project->variables()["FORMS"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		QString ext = variable == "MSVCDSP_FORMSOURCES" ? ".cpp" : ".h";
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    QString base = (*it);
		    int dot = base.findRev(".");
  		    base.replace( dot, base.length() - dot, ext );
		    QString fname = base;

		    int lbs = fname.findRev( "\\" );
		    QString fpath;
		    if ( lbs != -1 )
			fpath = fname.left( lbs + 1 );
		    fname = fname.right( fname.length() - lbs - 1 );

		    if ( ext == ".cpp" && !uiSourcesDir.isEmpty() )
			fname.prepend(uiSourcesDir);
		    else if ( ext == ".h" && !uiHeadersDir.isEmpty() )
			fname.prepend(uiHeadersDir);
		    else
			fname = base;
//		    beginGroupForFile(fname, t);
		    t << "# Begin Source File\n\nSOURCE=" << fname
		      << "\n# End Source File" << endl;
		}
//		endGroups(t);
	    } else if(variable == "MSVCDSP_TRANSLATIONS" ) {
		if(project->variables()["TRANSLATIONS"].isEmpty())
		    continue;

		t << "# Begin Group \"Translations\"\n";
		t << "# Prop Default_Filter \"ts\"\n";

		QStringList list = project->variables()["TRANSLATIONS"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    QString sify = *it;
		    sify.replace('/', '\\' );
		    QString base = (*it);
		    base.replace(QRegExp("\\..*$"), "").upper();
		    base.replace(QRegExp("[^a-zA-Z]"), "_");

//		    beginGroupForFile(sify, t);
		    t << "# Begin Source File\n\nSOURCE=" << sify << endl;
		    t << "\n# End Source File" << endl;
		}
//		endGroups(t);
		t << "\n# End Group\n";
	    } else if (variable == "MSVCDSP_MOCSOURCES" && project->isActiveConfig("moc")) {
		if ( project->variables()["SRCMOC"].isEmpty())
		    continue;

		QString mocpath = var( "QMAKE_MOC" );
		mocpath = mocpath.replace( QRegExp( "\\..*$" ), "" ) + " ";

		QStringList list = project->variables()["SRCMOC"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
//		    beginGroupForFile((*it), t);
		    t << "# Begin Source File\n\nSOURCE=" << (*it) << endl;
		    if ( project->isActiveConfig("moc") && (*it).endsWith(Option::cpp_moc_ext)) {
			QString base = (*it);
			base.replace(QRegExp("\\..*$"), "").upper();
			base.replace(QRegExp("[^a-zA-Z]"), "_");

			QString build = "\n\n# Begin Custom Build - Moc'ing " + findMocSource((*it)) +
					"...\n" "InputPath=.\\" + (*it) + "\n\n" "\"" + (*it) + "\""
					" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n"
					"\t" + mocpath + findMocSource((*it)) + " -o " +
					(*it) + "\n\n" "# End Custom Build\n\n";

			t << "USERDEP_" << base << "=\".\\" << findMocSource((*it)) << "\" \"$(QTDIR)\\bin\\moc.exe\"" << endl << endl;

			t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build
			  << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\""
			  << build << "!ENDIF " << endl << endl;
		    }
		    t << "# End Source File" << endl;
		}
//		endGroups(t);
	    } else if(variable == "MSVCDSP_PICTURES") {
		if(project->variables()["IMAGES"].isEmpty())
		    continue;

		t << "# Begin Group \"Images\"\n"
		  << "# Prop Default_Filter \"png jpeg bmp xpm\"\n";

		QStringList list = project->variables()["IMAGES"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		QStringList::Iterator it;

		// dump the image list to a file UIC can read.
		QFile f( "images.tmp" );
		f.open( IO_WriteOnly );
		QTextStream ts( &f );
		for( it = list.begin(); it != list.end(); ++it )
		    ts << " " << *it;
		f.close();

		// create an output step for images not more than once
		bool imagesBuildDone = FALSE;
		for( it = list.begin(); it != list.end(); ++it ) {
//		    beginGroupForFile((*it), t);
		    t << "# Begin Source File\n\nSOURCE=" << (*it) << endl;

		    QString base = (*it);
		    QString uicpath = var("QMAKE_UIC");
		    uicpath = uicpath.replace(QRegExp("\\..*$"), "") + " ";

		    if ( !imagesBuildDone ) {
			imagesBuildDone = TRUE;
			QString build = "\n\n# Begin Custom Build - Creating image collection...\n"
			    "InputPath=.\\" + base + "\n\n";

			build += "\"" + project->first("QMAKE_IMAGE_COLLECTION") + "\" : $(SOURCE) \"$(INTDIR)\" \"$(OUTDIR)\"\n";
			build += "\t" + uicpath + "-embed " + project->first("QMAKE_ORIG_TARGET") + " -f images.tmp -o "
				      + project->first("QMAKE_IMAGE_COLLECTION") + "\n\n";
			build.append("# End Custom Build\n\n");

			t << "USERDEP_" << base << "=";
			QStringList::Iterator it2 = list.begin();
			while ( it2 != list.end() ) {
			    t << "\"" << (*it2) << "\"";
			    it2++;
			    if ( it2 != list.end() )
				t << "\\\n";
			}
			t << endl << endl;

			t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - Win32 Release\"" << build
			  << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - Win32 Debug\"" << build
			  << "!ENDIF \n\n" << endl;
		    }

		    t << "# End Source File" << endl;
		}
//		endGroups(t);
		t << "\n# End Group\n";
	    } else if(variable == "MSVCDSP_FORMS") {
		if(project->variables()["FORMS"].isEmpty())
		    continue;

		t << "# Begin Group \"Forms\"\n"
		  << "# Prop Default_Filter \"ui\"\n";

		QString uicpath = var("QMAKE_UIC");
		uicpath = uicpath.replace(QRegExp("\\..*$"), "") + " ";
		QString mocpath = var( "QMAKE_MOC" );
		mocpath = mocpath.replace( QRegExp( "\\..*$" ), "" ) + " ";

		QStringList list = project->variables()["FORMS"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    QString base = (*it);
//		    beginGroupForFile(base, t);
		    t <<  "# Begin Source File\n\nSOURCE=" << base << endl;

		    QString fname = base;
		    fname.replace(".ui", "");
		    int lbs = fname.findRev( "\\" );
		    QString fpath;
		    if ( lbs != -1 )
			fpath = fname.left( lbs + 1 );
		    fname = fname.right( fname.length() - lbs - 1 );

		    QString mocFile;
		    if(!project->variables()["MOC_DIR"].isEmpty())
			mocFile = project->first("MOC_DIR");
		    else
			mocFile = fpath;

		    QString uiSourcesDir;
		    QString uiHeadersDir;
		    if(!project->variables()["UI_DIR"].isEmpty()) {
			uiSourcesDir = project->first("UI_DIR");
			uiHeadersDir = project->first("UI_DIR");
		    } else {
			if ( !project->variables()["UI_SOURCES_DIR"].isEmpty() )
			    uiSourcesDir = project->first("UI_SOURCES_DIR");
			else
			    uiSourcesDir = fpath;
			if ( !project->variables()["UI_HEADERS_DIR"].isEmpty() )
			    uiHeadersDir = project->first("UI_HEADERS_DIR");
			else
			    uiHeadersDir = fpath;
		    }

		    t << "USERDEP_" << base << "=\"$(QTDIR)\\bin\\moc.exe\" \"$(QTDIR)\\bin\\uic.exe\"" << endl << endl;

		    QString build = "\n\n# Begin Custom Build - Uic'ing " + base + "...\n"
			"InputPath=.\\" + base + "\n\n" "BuildCmds= \\\n\t" + uicpath + base +
				    " -o " + uiHeadersDir + fname + ".h \\\n" "\t" + uicpath  + base +
				    " -i " + fname + ".h -o " + uiSourcesDir + fname + ".cpp \\\n"
				    "\t" + mocpath + " " + uiHeadersDir +
				    fname + ".h -o " + mocFile + Option::h_moc_mod + fname + Option::h_moc_ext + " \\\n";

		    build.append("\n\"" + uiHeadersDir + fname + ".h\" : \"$(SOURCE)\" \"$(INTDIR)\" \"$(OUTDIR)\""  "\n"
				 "\t$(BuildCmds)\n\n"
				 "\"" + uiSourcesDir + fname + ".cpp\" : \"$(SOURCE)\" \"$(INTDIR)\" \"$(OUTDIR)\"" "\n"
				 "\t$(BuildCmds)\n\n"
				 "\"" + mocFile + Option::h_moc_mod + fname + Option::h_moc_ext + "\" : \"$(SOURCE)\" \"$(INTDIR)\" \"$(OUTDIR)\"" "\n"
				 "\t$(BuildCmds)\n\n");

		    build.append("# End Custom Build\n\n");

		    t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build
		      << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\"" << build
		      << "!ENDIF \n\n" << "# End Source File" << endl;
		}
//		endGroups(t);
		t << "\n# End Group\n";
	    } else if(variable == "MSVCDSP_LEXSOURCES") {
		if(project->variables()["LEXSOURCES"].isEmpty())
		    continue;

		t << "# Begin Group \"Lexables\"\n"
		  << "# Prop Default_Filter \"l\"\n";

		QString lexpath = var("QMAKE_LEX") + varGlue("QMAKE_LEXFLAGS", " ", " ", "") + " ";

		QStringList list = project->variables()["LEXSOURCES"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    QString fname = (*it);
//		    beginGroupForFile(fname, t);
		    t <<  "# Begin Source File\n\nSOURCE=" << fname << endl;
		    fname.replace(".l", Option::lex_mod + Option::cpp_ext.first());

		    QString build = "\n\n# Begin Custom Build - Lex'ing " + (*it) + "...\n"
			"InputPath=.\\" + (*it) + "\n\n"
				    "\"" + fname + "\" : \"$(SOURCE)\" \"$(INTDIR)\" \"$(OUTDIR)\"" "\n"
				    "\t" + lexpath + (*it) + "\\\n"
				    "\tdel " + fname + "\\\n"
				    "\tcopy lex.yy.c " + fname + "\n\n" +
				    "# End Custom Build\n\n";
		    t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build
		      << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\"" << build
		      << "!ENDIF \n\n" << build

		      << "# End Source File" << endl;
		}
//		endGroups(t);
		t << "\n# End Group\n";
	    } else if(variable == "MSVCDSP_YACCSOURCES") {
		if(project->variables()["YACCSOURCES"].isEmpty())
		    continue;

		t << "# Begin Group \"Yaccables\"\n"
		  << "# Prop Default_Filter \"y\"\n";

		QString yaccpath = var("QMAKE_YACC") + varGlue("QMAKE_YACCFLAGS", " ", " ", "") + " ";

		QStringList list = project->variables()["YACCSOURCES"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    QString fname = (*it);
//		    beginGroupForFile(fname, t);
		    t <<  "# Begin Source File\n\nSOURCE=" << fname << endl;
		    fname.replace(".y", Option::yacc_mod);

		    QString build = "\n\n# Begin Custom Build - Yacc'ing " + (*it) + "...\n"
			"InputPath=.\\" + (*it) + "\n\n"
				    "\"" + fname + Option::cpp_ext.first() + "\" : \"$(SOURCE)\" \"$(INTDIR)\" \"$(OUTDIR)\"" "\n"
				    "\t" + yaccpath + (*it) + "\\\n"
				    "\tdel " + fname + Option::h_ext.first() + "\\\n"
				    "\tmove y.tab.h " + fname + Option::h_ext.first() + "\n\n" +
				    "\tdel " + fname + Option::cpp_ext.first() + "\\\n"
				    "\tmove y.tab.c " + fname + Option::cpp_ext.first() + "\n\n" +
				    "# End Custom Build\n\n";

		    t << "!IF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Release\"" << build
		      << "!ELSEIF  \"$(CFG)\" == \"" << var("MSVCDSP_PROJECT") << " - " << platform << " Debug\"" << build
		      << "!ENDIF \n\n"
		      << "# End Source File" << endl;
		}
//		endGroups(t);
		t << "\n# End Group\n";
	    } else if( variable == "MSVCDSP_CONFIGMODE" ) {
		if( project->isActiveConfig( "debug" ) )
		    t << "Debug";
		else
		    t << "Release";
	    } else if( variable == "MSVCDSP_IDLSOURCES" ) {
		QStringList list = project->variables()["MSVCDSP_IDLSOURCES"];
		if(!project->isActiveConfig("flat"))
		    list.sort();
		for(QStringList::Iterator it = list.begin(); it != list.end(); ++it) {
		    t << "# Begin Source File" << endl << endl;
		    t << "SOURCE=" << (*it) << endl;
		    t << "# PROP Exclude_From_Build 1" << endl;
		    t << "# End Source File" << endl << endl;
		}
	    }
	    else
		t << var(variable);
	}
	t << line << endl;
    }
    t << endl;
    file.close();
    return TRUE;
}



void
DspMakefileGenerator::init()
{
    if(init_flag)
	return;
    QStringList::Iterator it;
    init_flag = TRUE;

    const bool thread = project->isActiveConfig("thread");

    if ( project->isActiveConfig("stl") ) {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_STL_ON"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_STL_ON"];
    } else {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_STL_OFF"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_STL_OFF"];
    }
    if ( project->isActiveConfig("exceptions") ) {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_EXCEPTIONS_ON"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_EXCEPTIONS_ON"];
    } else {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_EXCEPTIONS_OFF"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_EXCEPTIONS_OFF"];
    }
    if ( project->isActiveConfig("rtti") ) {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_RTTI_ON"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_RTTI_ON"];
    } else {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_RTTI_OFF"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_RTTI_OFF"];
    }

    
    /* this should probably not be here, but I'm using it to wrap the .t files */
    if(project->first("TEMPLATE") == "vcapp" )
	project->variables()["QMAKE_APP_FLAG"].append("1");
    else if(project->first("TEMPLATE") == "vclib")
	project->variables()["QMAKE_LIB_FLAG"].append("1");
    if ( project->variables()["QMAKESPEC"].isEmpty() )
	project->variables()["QMAKESPEC"].append( getenv("QMAKESPEC") );

    bool is_qt = (project->first("TARGET") == "qt"QTDLL_POSTFIX || project->first("TARGET") == "qt-mt"QTDLL_POSTFIX);
    project->variables()["QMAKE_ORIG_TARGET"] = project->variables()["TARGET"];

    QStringList &configs = project->variables()["CONFIG"];
    if (project->isActiveConfig("shared"))
	project->variables()["DEFINES"].append("QT_DLL");
    if (project->isActiveConfig("qt_dll"))
	if(configs.findIndex("qt") == -1) configs.append("qt");
    if ( project->isActiveConfig("qtopia") ) {
	if(configs.findIndex("qtopialib") == -1)
	    configs.append("qtopialib");
	if(configs.findIndex("qtopiainc") == -1)
	    configs.append("qtopiainc");
    }
    if ( project->isActiveConfig("qt") ) {
	if ( project->isActiveConfig( "plugin" ) ) {
	    project->variables()["CONFIG"].append("dll");
	    project->variables()["DEFINES"].append("QT_PLUGIN");
	}
	if ( (project->variables()["DEFINES"].findIndex("QT_NODLL") == -1) &&
	     ((project->variables()["DEFINES"].findIndex("QT_MAKEDLL") != -1 ||
	       project->variables()["DEFINES"].findIndex("QT_DLL") != -1) ||
	      (getenv("QT_DLL") && !getenv("QT_NODLL"))) ) {
	    project->variables()["QMAKE_QT_DLL"].append("1");
	    if ( is_qt && !project->variables()["QMAKE_LIB_FLAG"].isEmpty() )
		project->variables()["CONFIG"].append("dll");
	}
    }
    if ( project->isActiveConfig("dll") || !project->variables()["QMAKE_APP_FLAG"].isEmpty() ) {
	project->variables()["CONFIG"].remove("staticlib");
	project->variables()["QMAKE_APP_OR_DLL"].append("1");
    } else {
	project->variables()["CONFIG"].append("staticlib");
    }

    if ( project->isActiveConfig("qt") || project->isActiveConfig("opengl") ) {
	project->variables()["CONFIG"].append("windows");
    }
    if ( !project->variables()["VERSION"].isEmpty() ) {
	QString version = project->variables()["VERSION"][0];
	int firstDot = version.find( "." );
	QString major = version.left( firstDot );
	QString minor = version.right( version.length() - firstDot - 1 );
	minor.replace( ".", "" );
	project->variables()["MSVCDSP_VERSION"].append( "/VERSION:" + major + "." + minor );
    }

    if ( project->isActiveConfig("qtopiainc") )
	project->variables()["INCLUDEPATH"] += project->variables()["QMAKE_INCDIR_QTOPIA"];
    if ( project->isActiveConfig("qtopialib") ) {
	if(!project->isEmpty("QMAKE_LIBDIR_QTOPIA"))
	    project->variables()["QMAKE_LIBDIR"] += project->variables()["QMAKE_LIBDIR_QTOPIA"];
	project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_QTOPIA"];
    }

    if ( project->isActiveConfig("qt") ) {
	project->variables()["CONFIG"].append("moc");
	project->variables()["INCLUDEPATH"] +=	project->variables()["QMAKE_INCDIR_QT"];
	project->variables()["QMAKE_LIBDIR"] += project->variables()["QMAKE_LIBDIR_QT"];

	if ( is_qt && !project->variables()["QMAKE_LIB_FLAG"].isEmpty() ) {
	    if ( !project->variables()["QMAKE_QT_DLL"].isEmpty() ) {
		project->variables()["DEFINES"].append("QT_MAKEDLL");
		project->variables()["QMAKE_LFLAGS"].append("/base:\"0x39D00000\"");
	    }
	} else {
	    if( thread )
		project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_QT_THREAD"];
	    else
		project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_QT"];
	    if ( !project->variables()["QMAKE_QT_DLL"].isEmpty() ) {
		int hver = findHighestVersion(project->first("QMAKE_LIBDIR_QT"), "qt");
		if ( hver == -1 )
		    hver = findHighestVersion(project->first("QMAKE_LIBDIR_QT"), "qt-mt");
		if(hver != -1) {
		    QString ver;
		    ver.sprintf("qt%s" QTDLL_POSTFIX "%d.lib", (thread ? "-mt" : ""), hver);
		    QStringList &libs = project->variables()["QMAKE_LIBS"];
		    for(QStringList::Iterator libit = libs.begin(); libit != libs.end(); ++libit)
			(*libit).replace(QRegExp("qt(-mt)?\\.lib"), ver);
		}
	    }
	    if ( project->isActiveConfig( "activeqt" ) ) {
		project->variables().remove("QMAKE_LIBS_QT_ENTRY");
		project->variables()["QMAKE_LIBS_QT_ENTRY"] = "qaxserver.lib";
		if ( project->isActiveConfig( "dll" ) )
		    project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_QT_ENTRY"];
	    }
	    if ( !project->isActiveConfig("dll") && !project->isActiveConfig("plugin") ) {
		project->variables()["QMAKE_LIBS"] +=project->variables()["QMAKE_LIBS_QT_ENTRY"];
	    }
	}
    }

    if ( project->isActiveConfig("debug") ) {
	if ( !project->first("OBJECTS_DIR").isEmpty() )
	    project->variables()["MSVCDSP_OBJECTSDIRDEB"] = project->first("OBJECTS_DIR");
	else
	    project->variables()["MSVCDSP_OBJECTSDIRDEB"] = "Debug";
	project->variables()["MSVCDSP_OBJECTSDIRREL"] = "Release";
	if ( !project->first("DESTDIR").isEmpty() )
	    project->variables()["MSVCDSP_TARGETDIRDEB"] = project->first("DESTDIR");
	else
	    project->variables()["MSVCDSP_TARGETDIRDEB"] = "Debug";
	project->variables()["MSVCDSP_TARGETDIRREL"] = "Release";
    } else {
	if ( !project->first("OBJECTS_DIR").isEmpty() )
	    project->variables()["MSVCDSP_OBJECTSDIRREL"] = project->first("OBJECTS_DIR");
	else
	    project->variables()["MSVCDSP_OBJECTSDIRREL"] = "Release";
	project->variables()["MSVCDSP_OBJECTSDIRDEB"] = "Debug";
	if ( !project->first("DESTDIR").isEmpty() )
	    project->variables()["MSVCDSP_TARGETDIRREL"] = project->first("DESTDIR");
	else
	    project->variables()["MSVCDSP_TARGETDIRREL"] = "Release";
	project->variables()["MSVCDSP_TARGETDIRDEB"] = "Debug";
    }

    if ( project->isActiveConfig("opengl") ) {
	project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_OPENGL"];
	project->variables()["QMAKE_LFLAGS"] += project->variables()["QMAKE_LFLAGS_OPENGL"];
    }
    if ( thread ) {
	if(project->isActiveConfig("qt"))
	    project->variables()[is_qt ? "PRL_EXPORT_DEFINES" : "DEFINES"].append("QT_THREAD_SUPPORT" );
        if ( project->isActiveConfig("dll") || project->first("TARGET") == "qtmain"
	     || !project->variables()["QMAKE_QT_DLL"].isEmpty() ) {
	    project->variables()["MSVCDSP_MTDEFD"] += project->variables()["QMAKE_CXXFLAGS_MT_DLLDBG"];
	    project->variables()["MSVCDSP_MTDEF"] += project->variables()["QMAKE_CXXFLAGS_MT_DLL"];
	} else {
	    // YES we want to use the DLL even in a static build
	    project->variables()["MSVCDSP_MTDEFD"] += project->variables()["QMAKE_CXXFLAGS_MT_DBG"];
	    project->variables()["MSVCDSP_MTDEF"] += project->variables()["QMAKE_CXXFLAGS_MT"];
	}
	if ( !project->variables()["DEFINES"].contains("QT_DLL") && is_qt
	     && project->first("TARGET") != "qtmain" )
	    project->variables()["QMAKE_LFLAGS"].append("/NODEFAULTLIB:\"libc\"");
    }

    if(project->isActiveConfig("qt")) {
	if ( project->isActiveConfig("accessibility" ) )
	    project->variables()[is_qt ? "PRL_EXPORT_DEFINES" : "DEFINES"].append("QT_ACCESSIBILITY_SUPPORT");
	if ( project->isActiveConfig("tablet") )
	    project->variables()[is_qt ? "PRL_EXPORT_DEFINES" : "DEFINES"].append("QT_TABLET_SUPPORT");
    }
    if ( project->isActiveConfig("dll") ) {
	project->variables()["QMAKE_CFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_CFLAGS_CONSOLE_DLL"];
	project->variables()["QMAKE_CXXFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_CXXFLAGS_CONSOLE_DLL"];
	project->variables()["QMAKE_LFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_LFLAGS_CONSOLE_DLL"];
	project->variables()["QMAKE_LFLAGS_WINDOWS_ANY"] = project->variables()["QMAKE_LFLAGS_WINDOWS_DLL"];
	if ( !project->variables()["QMAKE_LIB_FLAG"].isEmpty() ) {
	    QString ver_xyz(project->first("VERSION"));
	    ver_xyz.replace(".", "");
	    project->variables()["TARGET_EXT"].append(ver_xyz + ".dll");
	} else {
	    project->variables()["TARGET_EXT"].append(".dll");
	}
    } else {
	project->variables()["QMAKE_CFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_CFLAGS_CONSOLE"];
	project->variables()["QMAKE_CXXFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_CXXFLAGS_CONSOLE"];
	project->variables()["QMAKE_LFLAGS_CONSOLE_ANY"] = project->variables()["QMAKE_LFLAGS_CONSOLE"];
	project->variables()["QMAKE_LFLAGS_WINDOWS_ANY"] = project->variables()["QMAKE_LFLAGS_WINDOWS"];
	if ( !project->variables()["QMAKE_APP_FLAG"].isEmpty() )
	    project->variables()["TARGET_EXT"].append(".exe");
	else
	    project->variables()["TARGET_EXT"].append(".lib");
    }

    if ( project->isActiveConfig("windows") ) {
	if ( project->isActiveConfig("console") ) {
	    project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_CONSOLE_ANY"];
	    project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_CONSOLE_ANY"];
	    project->variables()["QMAKE_LFLAGS"] += project->variables()["QMAKE_LFLAGS_CONSOLE_ANY"];
	    project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_CONSOLE"];
	} else {
	    project->variables()["QMAKE_LFLAGS"] += project->variables()["QMAKE_LFLAGS_WINDOWS_ANY"];
	}
	project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_WINDOWS"];
    } else {
	project->variables()["QMAKE_CFLAGS"] += project->variables()["QMAKE_CFLAGS_CONSOLE_ANY"];
	project->variables()["QMAKE_CXXFLAGS"] += project->variables()["QMAKE_CXXFLAGS_CONSOLE_ANY"];
	project->variables()["QMAKE_LFLAGS"] += project->variables()["QMAKE_LFLAGS_CONSOLE_ANY"];
	project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_CONSOLE"];
    }

    project->variables()["MSVCDSP_VER"] = "6.00";
    project->variables()["MSVCDSP_DEBUG_OPT"] = "/GZ /ZI";

    if(!project->isActiveConfig("incremental")) {
	project->variables()["QMAKE_LFLAGS"].append(QString("/incremental:no"));
        if ( is_qt )
	    project->variables()["MSVCDSP_DEBUG_OPT"] = "/GZ /Zi";
    }

    QString msvcdsp_project;
    if ( project->variables()["TARGET"].count() )
	msvcdsp_project = project->variables()["TARGET"].first();

    QString targetfilename = project->variables()["TARGET"].first();
    project->variables()["TARGET"].first() += project->first("TARGET_EXT");
    if ( project->isActiveConfig("moc") )
	setMocAware(TRUE);

    project->variables()["QMAKE_LIBS"] += project->variables()["LIBS"];
    project->variables()["QMAKE_FILETAGS"] += QStringList::split(' ',
								 "HEADERS SOURCES DEF_FILE RC_FILE TARGET QMAKE_LIBS DESTDIR DLLDESTDIR INCLUDEPATH");
    QStringList &l = project->variables()["QMAKE_FILETAGS"];
    for(it = l.begin(); it != l.end(); ++it) {
	QStringList &gdmf = project->variables()[(*it)];
	for(QStringList::Iterator inner = gdmf.begin(); inner != gdmf.end(); ++inner)
	    (*inner) = Option::fixPathToTargetOS((*inner), FALSE);
    }

    MakefileGenerator::init();
    if ( msvcdsp_project.isEmpty() )
	msvcdsp_project = Option::output.name();

    msvcdsp_project = msvcdsp_project.right( msvcdsp_project.length() - msvcdsp_project.findRev( "\\" ) - 1 );
    msvcdsp_project = msvcdsp_project.left( msvcdsp_project.findRev( "." ) );
    msvcdsp_project.replace("-", "");

    project->variables()["MSVCDSP_PROJECT"].append(msvcdsp_project);
    QStringList &proj = project->variables()["MSVCDSP_PROJECT"];

    for(it = proj.begin(); it != proj.end(); ++it)
	(*it).replace(QRegExp("\\.[a-zA-Z0-9_]*$"), "");

    if ( !project->variables()["QMAKE_APP_FLAG"].isEmpty() ) {
	project->variables()["MSVCDSP_TEMPLATE"].append("win32app" + project->first( "DSP_EXTENSION" ) );
	if ( project->isActiveConfig("console") ) {
	    project->variables()["MSVCDSP_CONSOLE"].append("Console");
	    project->variables()["MSVCDSP_WINCONDEF"].append("_CONSOLE");
	    project->variables()["MSVCDSP_DSPTYPE"].append("0x0103");
	} else {
	    project->variables()["MSVCDSP_CONSOLE"].clear();
	    project->variables()["MSVCDSP_WINCONDEF"].append("_WINDOWS");
	    project->variables()["MSVCDSP_DSPTYPE"].append("0x0101");
	}
    } else {
        if ( project->isActiveConfig("dll") ) {
            project->variables()["MSVCDSP_TEMPLATE"].append("win32dll" + project->first( "DSP_EXTENSION" ) );
        } else {
            project->variables()["MSVCDSP_TEMPLATE"].append("win32lib" + project->first( "DSP_EXTENSION" ) );
        }
    }

    project->variables()["QMAKE_LIBS"] += project->variables()["QMAKE_LIBS_WINDOWS"];
 
    processPrlFiles();

     // Update -lname to name.lib,
    QStringList &libList = project->variables()["QMAKE_LIBS"];
    for( QStringList::Iterator stIt = libList.begin(); stIt != libList.end(); ) {
	QString s = *stIt;
	if( s.startsWith( "-l" ) ) {
	    stIt = libList.remove( stIt );
	    stIt = libList.insert( stIt, s.mid( 2 ) + ".lib" );
        } else if( s.startsWith( "-L" ) ) {
	    stIt = libList.remove( stIt );
	    project->variables()["QMAKE_LIBDIR"].append(QDir::convertSeparators(s.mid( 2 )));
	} else {
	    stIt++;
	}
    }
    
    project->variables()["MSVCDSP_LFLAGS" ] += project->variables()["QMAKE_LFLAGS"];
    if ( !project->variables()["QMAKE_LIBDIR"].isEmpty() )
	project->variables()["MSVCDSP_LFLAGS" ].append(varGlue("QMAKE_LIBDIR","/LIBPATH:\"","\" /LIBPATH:\"","\""));
    project->variables()["MSVCDSP_CXXFLAGS" ] += project->variables()["QMAKE_CXXFLAGS"];
    project->variables()["MSVCDSP_DEFINES"].append(varGlue("DEFINES","/D ","" " /D ",""));
    project->variables()["MSVCDSP_DEFINES"].append(varGlue("PRL_EXPORT_DEFINES","/D ","" " /D ",""));

    if (!project->variables()["RES_FILE"].isEmpty())
	project->variables()["QMAKE_LIBS"] += project->variables()["RES_FILE"];

    QStringList &libs = project->variables()["QMAKE_LIBS"];
    for(QStringList::Iterator libit = libs.begin(); libit != libs.end(); ++libit) {
	QString lib = (*libit);
	lib.replace(QRegExp("\""), "");
	project->variables()["MSVCDSP_LIBS"].append(" \"" + lib + "\"");
    }

    QStringList &incs = project->variables()["INCLUDEPATH"];
    for(QStringList::Iterator incit = incs.begin(); incit != incs.end(); ++incit) {
	QString inc = (*incit);
	inc.replace("\"", "");
	if(inc.endsWith("\\")) // Remove trailing \'s from paths
	    inc.truncate(inc.length()-1);
	if (inc.startsWith("\"") && inc.endsWith("\""))
	    inc = inc.mid(1, inc.length() - 2);
	project->variables()["MSVCDSP_INCPATH"].append("/I \"" + inc + "\"");
    }

    project->variables()["MSVCDSP_INCPATH"].append("/I \"" + specdir() + "\"");
    if ( project->isActiveConfig("qt") ) {
	project->variables()["MSVCDSP_RELDEFS"].append("/D \"QT_NO_DEBUG\"");
    } else {
	project->variables()["MSVCDSP_RELDEFS"].clear();
    }

    QString dest;
    QString postLinkStep;
    QString copyDllStep;
    QString activeQtStepPreCopyDll;
    QString activeQtStepPostCopyDll;
    QString activeQtStepPreCopyDllDebug;
    QString activeQtStepPostCopyDllDebug;
    QString activeQtStepPreCopyDllRelease;
    QString activeQtStepPostCopyDllRelease;

    if ( !project->variables()["QMAKE_POST_LINK"].isEmpty() )
	postLinkStep += var("QMAKE_POST_LINK");

    if ( !project->variables()["DESTDIR"].isEmpty() ) {
	project->variables()["TARGET"].first().prepend(project->first("DESTDIR"));
	Option::fixPathToTargetOS(project->first("TARGET"));
	dest = project->first("TARGET");
        if ( project->first("TARGET").startsWith("$(QTDIR)") )
	    dest.replace( "$(QTDIR)", getenv("QTDIR") );
	project->variables()["MSVCDSP_TARGET"].append(
	    QString("/out:\"") + dest + "\"");
	if ( project->isActiveConfig("dll") ) {
	    QString imp = dest;
	    imp.replace(".dll", ".lib");
	    project->variables()["MSVCDSP_TARGET"].append(QString(" /implib:\"") + imp + "\"");
	}
    }
    if ( project->isActiveConfig("dll") && !project->variables()["DLLDESTDIR"].isEmpty() ) {
	QStringList dlldirs = project->variables()["DLLDESTDIR"];
	if ( dlldirs.count() )
	    copyDllStep += "\t";
	for ( QStringList::Iterator dlldir = dlldirs.begin(); dlldir != dlldirs.end(); ++dlldir ) {
	    copyDllStep += "copy \"$(TargetPath)\" \"" + *dlldir + "\"\t";
	}
    }

    if ( project->isActiveConfig("activeqt") ) {
	QString idl = project->variables()["QMAKE_IDL"].first();
	QString idc = project->variables()["QMAKE_IDC"].first();
	QString version = project->variables()["VERSION"].first();
	if ( version.isEmpty() )
	    version = "1.0";
	project->variables()["MSVCDSP_IDLSOURCES"].append( var("OBJECTS_DIR") + targetfilename + ".idl" );
	if ( project->isActiveConfig( "dll" ) ) {
	    activeQtStepPreCopyDll += 
			     "\t" + idc + " %1 -idl " + var("OBJECTS_DIR") + targetfilename + ".idl -version " + version +
			     "\t" + idl + " /nologo " + var("OBJECTS_DIR") + targetfilename + ".idl /tlb " + var("OBJECTS_DIR") + targetfilename + ".tlb" +
			     "\t" + idc + " %2 /tlb " + var("OBJECTS_DIR") + targetfilename + ".tlb";
	    activeQtStepPostCopyDll +=
			     "\t" + idc + " %1 /regserver\n";

	    QString executable = project->variables()["MSVCDSP_TARGETDIRREL"].first() + "\\" + targetfilename + ".dll";
	    activeQtStepPreCopyDllRelease = activeQtStepPreCopyDll.arg(executable).arg(executable);
	    activeQtStepPostCopyDllRelease = activeQtStepPostCopyDll.arg(executable);

	    executable = project->variables()["MSVCDSP_TARGETDIRDEB"].first() + "\\" + targetfilename + ".dll";
	    activeQtStepPreCopyDllDebug = activeQtStepPreCopyDll.arg(executable).arg(executable);
	    activeQtStepPostCopyDllDebug = activeQtStepPostCopyDll.arg(executable);
	} else {
	    activeQtStepPreCopyDll += 
			     "\t%1 -dumpidl " + var("OBJECTS_DIR") + targetfilename + ".idl -version " + version +
			     "\t" + idl + " /nologo " + var("OBJECTS_DIR") + targetfilename + ".idl /tlb " + var("OBJECTS_DIR") + targetfilename + ".tlb" +
			     "\t" + idc + " %2 /tlb " + var("OBJECTS_DIR") + targetfilename + ".tlb";
	    activeQtStepPostCopyDll +=
			     "\t%1 -regserver\n";
	    QString executable = project->variables()["MSVCDSP_TARGETDIRREL"].first() + "\\" + targetfilename + ".exe";
	    activeQtStepPreCopyDllRelease = activeQtStepPreCopyDll.arg(executable).arg(executable);
	    activeQtStepPostCopyDllRelease = activeQtStepPostCopyDll.arg(executable);

	    executable = project->variables()["MSVCDSP_TARGETDIRDEB"].first() + "\\" + targetfilename + ".exe";
	    activeQtStepPreCopyDllDebug = activeQtStepPreCopyDll.arg(executable).arg(executable);
	    activeQtStepPostCopyDllDebug = activeQtStepPostCopyDll.arg(executable);
	}

    }

    
    if ( !postLinkStep.isEmpty() || !copyDllStep.isEmpty() || !activeQtStepPreCopyDllDebug.isEmpty() || !activeQtStepPreCopyDllRelease.isEmpty() ) {
	project->variables()["MSVCDSP_POST_LINK_DBG"].append(
	    "# Begin Special Build Tool\n"
	    "SOURCE=$(InputPath)\n"
	    "PostBuild_Desc=Post Build Step\n"
	    "PostBuild_Cmds=" + postLinkStep + activeQtStepPreCopyDllDebug + copyDllStep + activeQtStepPostCopyDllDebug + "\n"
	    "# End Special Build Tool\n" );
	project->variables()["MSVCDSP_POST_LINK_REL"].append(
	    "# Begin Special Build Tool\n"
	    "SOURCE=$(InputPath)\n"
	    "PostBuild_Desc=Post Build Step\n"
	    "PostBuild_Cmds=" + postLinkStep + activeQtStepPreCopyDllRelease + copyDllStep + activeQtStepPostCopyDllRelease + "\n"
	    "# End Special Build Tool\n" );
    }

    if ( !project->variables()["SOURCES"].isEmpty() || !project->variables()["RC_FILE"].isEmpty() ) {
	project->variables()["SOURCES"] += project->variables()["RC_FILE"];
    }
    QStringList &list = project->variables()["FORMS"];
    for( it = list.begin(); it != list.end(); ++it ) {
	if ( QFile::exists( *it + ".h" ) )
	    project->variables()["SOURCES"].append( *it + ".h" );
    }
    project->variables()["QMAKE_INTERNAL_PRL_LIBS"] << "MSVCDSP_LIBS"; 
}


QString
DspMakefileGenerator::findTemplate(const QString &file)
{
    QString ret;
    if(!QFile::exists((ret = file)) &&
       !QFile::exists((ret = QString(Option::mkfile::qmakespec + "/" + file))) &&
       !QFile::exists((ret = QString(getenv("QTDIR")) + "/mkspecs/win32-msvc/" + file)) &&
       !QFile::exists((ret = (QString(getenv("HOME")) + "/.tmake/" + file))))
	return "";
    return ret;
}


void
DspMakefileGenerator::processPrlVariable(const QString &var, const QStringList &l)
{
    if(var == "QMAKE_PRL_DEFINES") {
	QStringList &out = project->variables()["MSVCDSP_DEFINES"];
	for(QStringList::ConstIterator it = l.begin(); it != l.end(); ++it) {
	    if(out.findIndex((*it)) == -1)
		out.append((" /D \"" + *it + "\""));
	}
    } else {
	MakefileGenerator::processPrlVariable(var, l);
    }
}


void
DspMakefileGenerator::beginGroupForFile(QString file, QTextStream &t,
					const QString& filter)
{
    if(project->isActiveConfig("flat"))
	return;
    fileFixify(file, QDir::currentDirPath(), QDir::currentDirPath(), TRUE);
    file = file.section(Option::dir_sep, 0, -2);
    if(file.right(Option::dir_sep.length()) != Option::dir_sep)
	file += Option::dir_sep;
    if(file == currentGroup)
	return;

    if(file.isEmpty() || !QDir::isRelativePath(file)) {
	endGroups(t);
	return;
    }
    
    QString tempFile = file;
    if(tempFile.startsWith(currentGroup))
	tempFile = tempFile.mid(currentGroup.length());
    int dirSep = currentGroup.findRev( Option::dir_sep );

    while( !tempFile.startsWith( currentGroup ) && dirSep != -1 ) {
	currentGroup.truncate( dirSep );
	dirSep = currentGroup.findRev( Option::dir_sep );
	if ( !tempFile.startsWith( currentGroup ) && dirSep != -1 )
	    t << "\n# End Group\n";
    }
    if ( !file.startsWith( currentGroup ) ) {
	t << "\n# End Group\n";
	currentGroup = "";
    }

    QStringList dirs = QStringList::split(Option::dir_sep, file.right( file.length() - currentGroup.length() ) );
    for(QStringList::Iterator dir_it = dirs.begin(); dir_it != dirs.end(); ++dir_it) {
	t << "# Begin Group \"" << (*dir_it) << "\"\n"
	    << "# Prop Default_Filter \"" << filter << "\"\n";
    }
    currentGroup = file;
}


void
DspMakefileGenerator::endGroups(QTextStream &t)
{
    if(project->isActiveConfig("flat"))
	return;
    else if(currentGroup.isEmpty())
	return;

    QStringList dirs = QStringList::split(Option::dir_sep, currentGroup);
    for(QStringList::Iterator dir_it = dirs.end(); dir_it != dirs.begin(); --dir_it) {
	t << "\n# End Group\n";
    }
    currentGroup = "";
}

bool
DspMakefileGenerator::openOutput(QFile &file) const
{
    QString outdir;
    if(!file.name().isEmpty()) {
	if(QDir::isRelativePath(file.name()))
	    file.setName(Option::output_dir + file.name()); //pwd when qmake was run
	QFileInfo fi(file);
	if(fi.isDir())
	    outdir = file.name() + QDir::separator();
    }
    if(!outdir.isEmpty() || file.name().isEmpty())
	file.setName(outdir + project->first("TARGET") + project->first("DSP_EXTENSION"));
    if(QDir::isRelativePath(file.name())) {
	QString ofile;
	ofile = file.name();
	int slashfind = ofile.findRev('\\');
	if (slashfind == -1) {
	    ofile = ofile.replace(QRegExp("-"), "_");
	} else {
	    int hypenfind = ofile.find('-', slashfind);
	    while (hypenfind != -1 && slashfind < hypenfind) {
		ofile = ofile.replace(hypenfind, 1, "_");
		hypenfind = ofile.find('-', hypenfind + 1);
	    }
	}
	file.setName(Option::fixPathToLocalOS(QDir::currentDirPath() + Option::dir_sep + ofile));
    }
    return Win32MakefileGenerator::openOutput(file);
}
