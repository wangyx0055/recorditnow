/***************************************************************************
 *   Copyright (C) 2010 by Kai Dombrowe <just89@gmx.de>                    *
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

#ifndef PROXYLAYOUT_H
#define PROXYLAYOUT_H


// Qt
#include <QLayout>


class QPropertyAnimation;
class ProxyLayout : public QLayout
{


public:
    ProxyLayout(QWidget *parent = 0);
    ~ProxyLayout();

    void addItem(QLayoutItem *item);
    void setGeometry(const QRect &newGeometry);

    int	count() const;
    QLayoutItem *itemAt(int index) const;
    QLayoutItem *takeAt(int index);
    Qt::Orientations expandingDirections() const;
    QRect geometry() const;
    bool isEmpty() const;
    QSize maximumSize() const;
    QSize minimumSize() const;
    QSize sizeHint() const;


private:
    QLayoutItem *m_item;
    QPropertyAnimation *m_animation;


};


#endif // PROXYLAYOUT_H
