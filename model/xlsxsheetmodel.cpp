/****************************************************************************
** Copyright (c) 2013-2014 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

#include "xlsxsheetmodel.h"
#include "xlsxsheetmodel_p.h"
#include "xlsxworksheet.h"

#include <QBrush>
#include <QColor>
#include <QFont>

QT_BEGIN_NAMESPACE_XLSX

SheetModelPrivate::SheetModelPrivate(SheetModel *p)
    :q_ptr(p)
{

}

/*!
 * \class SheetModel
 *
 *  Helper class for gui applicaiton user
 *
 *  \note SheetModel indices start from 0, while Worksheet
 *  column/row indices start from 1.
 */

/*!
 * Creates a model object with the given \a sheet and \a parent.
 */
SheetModel::SheetModel(Worksheet *sheet, QObject *parent)
    :QAbstractTableModel(parent), d_ptr(new SheetModelPrivate(this)),
     m_changed(false)
{
    d_ptr->sheet = sheet;
}

/*!
 * Destroys the model.
 */
SheetModel::~SheetModel()
{
    delete d_ptr;
}

int SheetModel::rowCount(const QModelIndex &/*parent*/) const
{
    Q_D(const SheetModel);
    return d->sheet->dimension().lastRow() - 1; // header line, is used for captions
}


int SheetModel::columnCount(const QModelIndex &/*parent*/) const
{
    Q_D(const SheetModel);
    return d->sheet->dimension().lastColumn();
}

Qt::ItemFlags SheetModel::flags(const QModelIndex &index) const
{
    if(index.column() < 14) {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
    }
    else {
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    }
}

QVariant SheetModel::data(const QModelIndex &index, int role) const
{
    Q_D(const SheetModel);

    if (!index.isValid())
        return QVariant();

    Cell *cell = d->sheet->cellAt(index.row()+2, index.column()+1);
    if (!cell)
        return QString();
    QVariant userFriendlyValue = d->sheet->read(index.row()+2, index.column()+1);

    if (role == Qt::DisplayRole) {
        if (cell->isDateTime())
            return userFriendlyValue;
        return cell->value();
    } else if (role == Qt::EditRole) {
        return userFriendlyValue;
    } else if (role == Qt::TextAlignmentRole) {
        Qt::Alignment align;
        switch (cell->format().horizontalAlignment()) {
        case Format::AlignLeft:
            align |= Qt::AlignLeft;
            break;
        case Format::AlignRight:
            align |= Qt::AlignRight;
            break;
        case Format::AlignHCenter:
            align |= Qt::AlignHCenter;
            break;
        case Format::AlignHJustify:
            align |= Qt::AlignJustify;
            break;
        default:
            break;
        }
        switch (cell->format().verticalAlignment()) {
        case Format::AlignTop:
            align |= Qt::AlignTop;
            break;
        case Format::AlignBottom:
            align |= Qt::AlignBottom;
            break;
        case Format::AlignVCenter:
            align |= Qt::AlignVCenter;
            break;
        default:
            break;
        }
        return QVariant(align);
    } else if (role == Qt::FontRole) {
        if (cell->format().hasFontData())
            return cell->format().font();
    } else if (role == Qt::ForegroundRole) {
        if (cell->format().fontColor().isValid())
            return QBrush(cell->format().fontColor());
    } else if (role == Qt::BackgroundRole) {
        if (index.row() % 2 == 0) {
            return QBrush(Qt::gray);
        }
        else {
            return QBrush(Qt::white);
        }
    }
//        if (cell->format().patternBackgroundColor().isValid())
//            return QBrush(cell->format().patternBackgroundColor());

    return QVariant();
}

/*
 * Copy from xlsxutility.cpp, so this example don't depend on the xlsx-private
 * This function should be removed once this class moved to the xlsx library.
 */
static QString col_to_name(int col_num)
{
    QString col_str;

    int remainder;
    while (col_num) {
        remainder = col_num % 26;
        if (remainder == 0)
            remainder = 26;
        col_str.prepend(QChar('A'+remainder-1));
        col_num = (col_num - 1) / 26;
    }

    return col_str;
}

QVariant SheetModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const SheetModel);

    if (role == Qt::DisplayRole) {
        if(orientation == Qt::Vertical) {
            return QString::number(section + 1);
        }
        else {
            Cell *cell = d->sheet->cellAt(1, section + 1);
            if (!cell) {
                return col_to_name(section + 1);
            }
            return cell->value();
        }
    }
    else if (role == Qt::FontRole) {
        QFont font;
        font.setBold(true);
        return font;
    }
    return QVariant();
}

bool SheetModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D(const SheetModel);

    if (!index.isValid())
        return false;

    if (role == Qt::EditRole) {
        if (d->sheet->write(index.row()+2, index.column()+1, value)) {
            calculateValuesRow(index);
            m_changed = true;
            return true;
        }
    }

    return false;
}

/*!
 * Returns the sheet object.
 */
Worksheet *SheetModel::sheet() const
{
    Q_D(const SheetModel);
    return d->sheet;
}

bool SheetModel::updateValue(float *value, int row, int column)
{
    Q_D(const SheetModel);

    bool ok;

    Cell *cell = d->sheet->cellAt(row, column);
    if(!cell) {
        return false;
    }
    float tmp = cell->value().toFloat(&ok);
    if(ok) {
        *value = tmp;
        return true;
    }
    return false;
}

void SheetModel::calculateValuesRow(const QModelIndex &index)
{
    Q_D(const SheetModel);

    float a1, a2, a3, e1, e2, e3, diff, pen;
    float am, em, final;
    int row = index.row()+2;

    int firstColumn = 7;

    if(!updateValue(&e1, row, firstColumn)) return;
    if(!updateValue(&e2, row, firstColumn + 1)) return;
    if(!updateValue(&e3, row, firstColumn + 2)) return;
    if(!updateValue(&a1, row, firstColumn + 3)) return;
    if(!updateValue(&a2, row, firstColumn + 4)) return;
    if(!updateValue(&a3, row, firstColumn + 5)) return;
    if(!updateValue(&diff, row, firstColumn + 6)) return;
    if(!updateValue(&pen, row, firstColumn + 7)) return;

    am = (a1 + a2 + a3) / 3;
    em = (e1 + e2 + e3) / 3 * 2;
    final = am + em + pen + diff;

    d->sheet->write(index.row()+2, firstColumn + 8, em);
    d->sheet->write(index.row()+2, firstColumn + 9, am);
    d->sheet->write(index.row()+2, firstColumn + 10, final);
}

bool SheetModel::changed() const
{
    return m_changed;
}

void SheetModel::setChanged(bool value)
{
    m_changed = value;
}

QT_END_NAMESPACE_XLSX
