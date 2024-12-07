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

#include "RaptorTrashViewModel.h"

RaptorTrashViewModel::RaptorTrashViewModel(QObject *qParent) : QAbstractTableModel(qParent)
{
    invokeInstanceInit();
}

QVariant RaptorTrashViewModel::headerData(int qSection,
                                          Qt::Orientation qOrientation,
                                          int qRole) const
{
    if (qOrientation != Qt::Horizontal)
    {
        return QVariant();
    }

    switch (qRole)
    {
        case Qt::DisplayRole:
            if (qSection > 0 && qSection <= _Headers.length())
            {
                return _Headers[qSection - 1];
            }

            return QVariant();
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        default:
            return QVariant();
    }
}

int RaptorTrashViewModel::rowCount(const QModelIndex &qIndex) const
{
    if (qIndex.isValid())
    {
        return 0;
    }

    return _Items.length();
}

int RaptorTrashViewModel::columnCount(const QModelIndex &qIndex) const
{
    if (qIndex.isValid())
    {
        return 0;
    }

    return _ColumnCount;
}

QVariant RaptorTrashViewModel::data(const QModelIndex &qIndex, int qRole) const
{
    if (!qIndex.isValid())
    {
        return QVariant();
    }

    const auto item = _Items[qIndex.row()];
    switch (qRole)
    {
        case Qt::UserRole:
            return QVariant::fromValue<RaptorTrashItem>(item);
        case Qt::DisplayRole:
        {
            switch (qIndex.column())
            {
                case 1:
                    return item._Name;
                case 2:
                    return item._Size;
                case 3:
                    return item._Trashed.split(' ')[0];
                default:
                    return QVariant();
            }
        }
        case Qt::TextAlignmentRole:
            return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
        default:
            return QVariant();
    }
}

bool RaptorTrashViewModel::setData(const QModelIndex &qIndex, const QVariant &qVariant, int qRole)
{
    if (!qIndex.isValid())
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    if (qRole != Qt::EditRole)
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    if (!qVariant.canConvert<RaptorTrashItem>())
    {
        return QAbstractTableModel::setData(qIndex, qVariant, qRole);
    }

    const auto item = qVariant.value<RaptorTrashItem>();
    _Items.replace(qIndex.row(), item);
    Q_EMIT dataChanged(qIndex, qIndex);
    return QAbstractTableModel::setData(qIndex, qVariant, qRole);
}

bool RaptorTrashViewModel::removeRow(int qRow, const QModelIndex &qIndex)
{
    if (qRow < 0 || qRow > _Items.length())
    {
        return false;
    }

    beginRemoveRows(qIndex, qRow, qRow);
    _Items.removeAt(qRow);
    endRemoveRows();
    return true;
}

void RaptorTrashViewModel::sort(int qColumn, Qt::SortOrder qOrder)
{
    if (qColumn == 0)
    {
        return;
    }

    beginResetModel();
    switch (qColumn)
    {
        case 1:
        {
            if (qOrder == Qt::AscendingOrder)
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsByNameAscSort, this, std::placeholders::_1, std::placeholders::_2));
            } else
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsByNameDescSort, this, std::placeholders::_1, std::placeholders::_2));
            }
            break;
        }
        case 2:
        {
            if (qOrder == Qt::AscendingOrder)
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsBySizeAscSort, this, std::placeholders::_1, std::placeholders::_2));
            } else
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsBySizeDescSort, this, std::placeholders::_1, std::placeholders::_2));
            }
            break;
        }
        case 3:
        {
            if (qOrder == Qt::AscendingOrder)
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsByTrashedAscSort, this, std::placeholders::_1, std::placeholders::_2));
            } else
            {
                std::sort(_Items.begin(), _Items.end(), std::bind(invokeItemsByTrashedDescSort, this, std::placeholders::_1, std::placeholders::_2));
            }
            break;
        }
        default:
            break;
    }

    endResetModel();
}

void RaptorTrashViewModel::invokeHeaderSet(const QVector<QString> &qHeader)
{
    _Headers = qHeader;
}

void RaptorTrashViewModel::invokeColumnCountSet(const quint16 &qCount)
{
    _ColumnCount = qCount;
}

void RaptorTrashViewModel::invokeItemAppend(const RaptorTrashItem &item)
{
    beginInsertRows(QModelIndex(), _Items.length(), _Items.length());
    _Items << item;
    endInsertRows();
}

void RaptorTrashViewModel::invokeItemsAppend(const QVector<RaptorTrashItem> &items)
{
    if (items.empty())
    {
        return;
    }

    beginInsertRows(QModelIndex(), _Items.length(), _Items.length() + items.length() - 1);
    _Items << items;
    endInsertRows();
}

void RaptorTrashViewModel::invokeItemsClear()
{
    beginResetModel();
    _Items.clear();
    endResetModel();
}

QVector<RaptorTrashItem> RaptorTrashViewModel::invokeItemsEject()
{
    return _Items;
}

void RaptorTrashViewModel::invokeInstanceInit()
{
    qCollator = QCollator(QLocale::Chinese);
    qCollator.setNumericMode(false);
    qCollator.setIgnorePunctuation(true);
}

bool RaptorTrashViewModel::invokeItemsByNameAscSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return qCollator.compare(item._Name, iten._Name) < 0;
}

bool RaptorTrashViewModel::invokeItemsByNameDescSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return qCollator.compare(item._Name, iten._Name) > 0;
}

bool RaptorTrashViewModel::invokeItemsBySizeAscSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return item._Byte < iten._Byte;
}

bool RaptorTrashViewModel::invokeItemsBySizeDescSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return item._Byte > iten._Byte;
}

bool RaptorTrashViewModel::invokeItemsByTrashedAscSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return qCollator.compare(item._Trashed, iten._Trashed) < 0;
}

bool RaptorTrashViewModel::invokeItemsByTrashedDescSort(const RaptorTrashItem &item, const RaptorTrashItem &iten) const
{
    return qCollator.compare(item._Trashed, iten._Trashed) > 0;
}
