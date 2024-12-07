/**
 *
 * Copyright (C) 2024 凉州刺史. All rights reserved.
 *
 * This file is part of Raptor.
 *
 * $RAPTOR_BEGIN_LICENSE$
 *
 * GNU General Public License Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 *
 * $RAPTOR_END_LICENSE$
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

#ifndef RAPTORUSERVIEWHEADER_H
#define RAPTORUSERVIEWHEADER_H

#include <QApplication>
#include <QHeaderView>
#include <QPainter>

class RaptorUserViewHeader Q_DECL_FINAL : public QHeaderView
{
    Q_OBJECT

public:
    using QHeaderView::QHeaderView;

protected:
    void paintSection(QPainter* qPainter,
                      const QRect& qRect,
                      int qLogicalIndex) const Q_DECL_OVERRIDE;
};

#endif // RAPTORUSERVIEWHEADER_H