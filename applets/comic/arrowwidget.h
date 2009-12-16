/***************************************************************************
 *   Copyright (C) 2009 Matthias Fuchs <mat69@gmx.net>                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#ifndef ARROWWIDGET_H
#define ARROWWIDGET_H

#include <QGraphicsWidget>

#include <Plasma/Plasma>

class QGraphicsSceneMouseEvent;

namespace Plasma {
class Svg;
}

class Arrow : public QGraphicsWidget
{
    Q_OBJECT

    public:
        Arrow( QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0 );
        ~Arrow();

        void paint( QPainter *p, const QStyleOptionGraphicsItem *, QWidget* widget = 0 );
        void setDirection( Plasma::Direction direction );
        Plasma::Direction direction() const;

    protected:
        QSizeF sizeHint( Qt::SizeHint which, const QSizeF &constraint = QSizeF() ) const;

    private:
        Plasma::Svg *mArrow;
        Plasma::Direction mDirection;
        QString mArrowName;

        static const int HEIGHT;
        static const int WIDTH;
};

class ArrowWidget : public QGraphicsWidget
{
    Q_OBJECT

    public:
        ArrowWidget ( QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0 );
        ~ArrowWidget();

        void setDirection( Plasma::Direction direction );
        Plasma::Direction direction() const;

    Q_SIGNALS:
        void clicked();

    protected:
        void mousePressEvent( QGraphicsSceneMouseEvent *event );
        QSizeF sizeHint( Qt::SizeHint which, const QSizeF &constraint = QSizeF() ) const;
        void hideEvent( QHideEvent *event );
        void showEvent( QShowEvent *event );

    private:
        Arrow *mArrow;
};

#endif
