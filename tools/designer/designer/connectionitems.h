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

#ifndef CONNECTIONITEMS_H
#define CONNECTIONITEMS_H

class FormWindow;

#include <qtable.h>

class SenderItem;
class ReceiverItem;
class SignalItem;
class SlotItem;
class ConnectionContainer;


class ConnectionItem : public QObject,
		       public QComboTableItem
{
    Q_OBJECT

public:
    ConnectionItem( QTable *table, FormWindow *fw );

    void paint( QPainter *p, const QColorGroup &cg,
		const QRect &cr, bool selected );

    void setSender( SenderItem *i );
    void setReceiver( ReceiverItem *i );
    void setSignal( SignalItem *i );
    void setSlot( SlotItem *i );
    void setConnection( ConnectionContainer *c );

public slots:
    virtual void senderChanged( QObject *sender );
    virtual void receiverChanged( QObject *receiver );
    virtual void signalChanged( const QString &sig );
    virtual void slotChanged( const QString &slot );

signals:
    void changed();

protected:
    FormWindow *formWindow;

private:
    ConnectionContainer *conn;

};

// ------------------------------------------------------------------

class SenderItem : public ConnectionItem
{
    Q_OBJECT

public:
    SenderItem( QTable *table, FormWindow *fw );
    QWidget *createEditor() const;
    void setSenderEx( QObject *sender );

signals:
    void currentSenderChanged( QObject *sender );

private slots:
    void senderChanged( const QString &sender );

};

// ------------------------------------------------------------------

class ReceiverItem : public ConnectionItem
{
    Q_OBJECT

public:
    ReceiverItem( QTable *table, FormWindow *fw );
    QWidget *createEditor() const;
    void setReceiverEx( QObject *receiver );

signals:
    void currentReceiverChanged( QObject *receiver );

private slots:
    void receiverChanged( const QString &receiver );

};

// ------------------------------------------------------------------

class SignalItem : public ConnectionItem
{
    Q_OBJECT

public:
    SignalItem( QTable *table, FormWindow *fw );

    void senderChanged( QObject *sender );
    QWidget *createEditor() const;

signals:
    void currentSignalChanged( const QString & );

};

// ------------------------------------------------------------------

class SlotItem : public ConnectionItem
{
    Q_OBJECT

public:
    SlotItem( QTable *table, FormWindow *fw );

    void receiverChanged( QObject *receiver );
    void signalChanged( const QString &signal );
    QWidget *createEditor() const;

    void customSlotsChanged();

signals:
    void currentSlotChanged( const QString & );

private:
    void updateSlotList();
    bool ignoreSlot( const char* slot ) const;

private:
    QObject *lastReceiver;
    QString lastSignal;

};

// ------------------------------------------------------------------

class ConnectionContainer : public QObject
{
    Q_OBJECT

public:
    ConnectionContainer( QObject *parent, SenderItem *i1, SignalItem *i2,
			 ReceiverItem *i3, SlotItem *i4, int r )
	: QObject( parent ), mod( FALSE ), se( i1 ), si( i2 ),
	  re( i3 ), sl( i4 ), rw ( r ) {
	      i1->setConnection( this );
	      i2->setConnection( this );
	      i3->setConnection( this );
	      i4->setConnection( this );
	      connect( i1, SIGNAL( changed() ), this, SLOT( somethingChanged() ) );
	      connect( i2, SIGNAL( changed() ), this, SLOT( somethingChanged() ) );
	      connect( i3, SIGNAL( changed() ), this, SLOT( somethingChanged() ) );
	      connect( i4, SIGNAL( changed() ), this, SLOT( somethingChanged() ) );
    }

    bool isModified() const { return mod; }
    void setModified( bool b ) { mod = b; repaint(); }
    bool isValid() const {
	return se->currentText()[0] != '<' &&
		si->currentText()[0] != '<' &&
		re->currentText()[0] != '<' &&
		sl->currentText()[0] != '<';
    }

    void repaint() {
	se->table()->updateCell( se->row(), se->col() );
	si->table()->updateCell( si->row(), si->col() );
	re->table()->updateCell( re->row(), re->col() );
	sl->table()->updateCell( sl->row(), sl->col() );
    }

    int row() const { return rw; }
    void setRow( int r ) { rw = r; }

    SenderItem *senderItem() const { return se; }
    SignalItem *signalItem() const { return si; }
    ReceiverItem *receiverItem() const { return re; }
    SlotItem *slotItem() const { return sl; }

public slots:
    void somethingChanged() { mod = TRUE; emit changed( this ); }

signals:
    void changed( ConnectionContainer *c );

private:
    bool mod;
    SenderItem *se;
    SignalItem *si;
    ReceiverItem *re;
    SlotItem *sl;
    int rw;

};


#endif
