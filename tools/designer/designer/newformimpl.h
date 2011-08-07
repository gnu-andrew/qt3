/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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

#ifndef NEWFORMIMPL_H
#define NEWFORMIMPL_H

#include "newform.h"
#include <qiconview.h>

class Project;

class NewItem : public QIconViewItem
{
public:
    enum Type {
	ProjectType,
	Form,
	CustomForm,
	SourceFileType,
	SourceTemplateType
    };

    NewItem( QIconView *view, const QString &text )
	: QIconViewItem( view, text ) {}
    virtual void insert( Project *pro ) = 0;
    virtual void setProject( Project * ) {}

};

class ProjectItem : public NewItem
{
public:
    ProjectItem( QIconView *view, const QString &text );
    void insert( Project *pro );
    int rtti() const { return (int)ProjectType; }

    void setLanguage( const QString &l ) { lang = l; }
    QString language() const { return lang; }

private:
    QString lang;

};

class FormItem : public NewItem
{
public:
    enum FormType {
	Widget,
	Dialog,
	Wizard,
	MainWindow
    };

    FormItem( QIconView *view, const QString &text );
    void insert( Project *pro );
    int rtti() const { return (int)Form; }

    void setFormType( FormType ft ) { fType = ft; }
    FormType formType() const { return fType; }

private:
    FormType fType;

};

class CustomFormItem : public NewItem
{
public:
    CustomFormItem( QIconView *view, const QString &text );
    void insert( Project *pro );
    int rtti() const { return (int)CustomForm; }

    void setTemplateFile( const QString &tf ) { templFile = tf; }
    QString templateFileName() const { return templFile; }

private:
    QString templFile;

};

class SourceFileItem : public NewItem
{
public:
    SourceFileItem( QIconView *view, const QString &text );
    void insert( Project *pro );
    int rtti() const { return (int)SourceFileType; }

    void setExtension( const QString &e ) { ext = e; }
    QString extension() const { return ext; }
    void setLanguage( const QString &l ) { lang = l; }
    void setProject( Project *pro );

private:
    QString ext;
    QString lang;
    bool visible;

};

class SourceTemplateItem : public NewItem
{
public:
    SourceTemplateItem( QIconView *view, const QString &text );
    void insert( Project *pro );
    int rtti() const { return (int)SourceTemplateType; }

    void setTemplate( const QString &t ) { templ = t; }
    QString tenplate() const { return templ; }
    void setLanguage( const QString &l ) { lang = l; }
    void setProject( Project *pro );

private:
    QString templ;
    QString lang;
    bool visible;

};

class NewForm : public NewFormBase
{
    Q_OBJECT

public:
    NewForm( QIconView *templateView, const QString &templatePath );
    NewForm( QWidget *parent, const QStringList& projects,
	     const QString& currentProject, const QString &templatePath );
    void insertTemplates( QIconView*, const QString& );
    void accept();
    QPtrList<QIconViewItem> allViewItems();

protected slots:
    void projectChanged( const QString &project );
    void itemChanged( QIconViewItem *item );

private:
    QPtrList<QIconViewItem> allItems;

};

#endif
