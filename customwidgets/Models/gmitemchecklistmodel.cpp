#include "gmitemchecklistmodel.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include <QFont>
#include <QBitmap>

GMItemCheckListModel::GMItemCheckListModel(GeneralVoucherDataObject *voucher,
                                           /*QList<CompoundItemDataObject>checkingList,*/ QObject *parent)
{
    this->voucher = voucher;
    //    this->checkingList = checkingList;

    headerStrings<<"#"<<"Voucher Item"<<"UOM"<<"Qty"<<"Check \n Qty ";
    this->colCount = headerStrings.size() + 1;

    qDebug()<<columnCount(QModelIndex())<<rowCount(QModelIndex());
    qDebug()<<Q_FUNC_INFO<<__LINE__<<this->voucher->InventoryItems.size();
}

QVariant GMItemCheckListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if (role != Qt::DisplayRole && section <colCount-1)
        return QVariant();
    if(section == colCount-1) {
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole){
            QPixmap pixmap = QPixmap(":/icons/icons/navigation/svg/production/ic_check_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#ffffff")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }

    }

    else if (orientation == Qt::Horizontal ) {
        return headerStrings[section];
    }

    return QVariant();

}

QModelIndex GMItemCheckListModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(row<=voucher->InventoryItems.size() && column< colCount){
        if((row == voucher->InventoryItems.size() && column <= 1 )|| row < voucher->InventoryItems.size())
            return this->createIndex(row,column);
    }
    return QModelIndex();
}

QModelIndex GMItemCheckListModel::parent(const QModelIndex &index) const
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QModelIndex();
}

int GMItemCheckListModel::rowCount(const QModelIndex &parent) const
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<voucher->InventoryItems.size();
    return voucher->InventoryItems.size();
}

int GMItemCheckListModel::columnCount(const QModelIndex &parent) const
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return colCount;
}

QVariant GMItemCheckListModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<Q_FUNC_INFO<<role<<Qt::DisplayRole<<Qt::EditRole;
    qDebug()<<"size"<<voucher->InventoryItems.size()/*<<checkingList.size()*/;


    if(index.column() == colCount-1 && role == Qt::DecorationRole )
    {
        if( voucher->InventoryItems[index.row()].BaseItem.quantity > 0 &&
                voucher->InventoryItems[index.row()].BaseItem.quantity == voucher->InventoryItems[index.row()].BaseItem.quantityFull){
            qDebug()<<"check pass 72";
            QPixmap pixmap = QPixmap(":/icons/icons/navigation/svg/production/ic_check_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#2e7d32")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }
        else if(voucher->InventoryItems[index.row()].BaseItem.quantity == 0 /*&& voucher->InventoryItems[index.row()].BaseItem.quantityFull > 0*/){
            qDebug()<<"check pass 72";
            QPixmap pixmap = QPixmap(":/icons/icons/action/svg/production/ic_help_outline_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#1976d2")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }
        else {
            QPixmap pixmap = QPixmap(":/icons/icons/navigation/svg/production/ic_close_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#d50000")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }

    }

    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    //    if (role == Qt::FontRole && index.column() == 1 &&
    //            index.row() == voucher->InventoryItems.size()) {
    //        // First column items are bold.
    //        QFont font;
    //        font.setItalic(true);
    //        return font;
    //    }


    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        //        if(index.column() == 1) //qDebug()<<"Inside if for col "<<index.column();

        if (index.column() == 0)
        {
            return  index.row()+1;
        }
        else if (index.column() == 1)
        {
            return voucher->InventoryItems[index.row()].BaseItem.ItemName;
        }
        else if (index.column() == 2)
        {
            return voucher->InventoryItems[index.row()].BaseItem.uomObject.UomSymbol;
        }
        else if (index.column() == 3)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.quantity
                    / voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',
                    voucher->InventoryItems[index.row()].BaseItem.uomObject.decimalPoints);
        }

        //        else if (index.column() == 4)
        //        {
        //            return checkingList[index.row()].BaseItem.ItemName;
        //        }
        //        else if (index.column() == 5)
        //        {
        //            return checkingList[index.row()].BaseItem.uomObject.UomSymbol;
        //        }
        else if (index.column() == 4)
        {
            return QString::number(voucher->InventoryItems[index.row()].BaseItem.quantityFull
                    / voucher->InventoryItems[index.row()].BaseItem.uomObject.convRate,'f',
                    voucher->InventoryItems[index.row()].BaseItem.uomObject.decimalPoints);
        }
        else if (index.column() == colCount-1)
        {
            qDebug()<<"more icon"<<voucher->InventoryItems[index.row()].BaseItem.quantity<<voucher->InventoryItems[index.row()].BaseItem.quantityFull;
            if(voucher->InventoryItems[index.row()].BaseItem.quantity == voucher->InventoryItems[index.row()].BaseItem.quantityFull){
                qDebug()<<"check pass";
                QPixmap pixmap = QPixmap(":/icons/icons/navigation/svg/production/ic_check_24px.svg");
                QBitmap mask  = pixmap.createMaskFromColor(QColor("black"), Qt::MaskOutColor);
                pixmap.fill((QColor("#aed581")));
                pixmap.setMask(mask);
                QIcon icon = QIcon(pixmap);
                return  QVariant::fromValue(icon);
            }
        }

    }

    return QVariant();

}

bool GMItemCheckListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<Q_FUNC_INFO;

    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        if(row< voucher->InventoryItems.size() && row >=0 && index.column() == 4){
            voucher->InventoryItems[row].BaseItem.quantityFull = value.toFloat();
            emit checkQtyUpdated();
        }

        return true;
    }

    return false;
}



Qt::ItemFlags GMItemCheckListModel::flags(const QModelIndex &index) const
{
    qDebug()<<Q_FUNC_INFO;

    if (!index.isValid())
        return Qt::NoItemFlags;

    qDebug()<<"flags";

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}
