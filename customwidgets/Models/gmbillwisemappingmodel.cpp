#include "gmbillwisemappingmodel.h"
#include <QtMath>
#include <QFont>
#include <QBitmap>
#include <QIcon>

GMBillwiseMappingModel::GMBillwiseMappingModel(QSqlQueryModel *qModel,
                                               LedgerMasterDataModel *ledger, QObject *parent) :
    QAbstractItemModel(parent)
{

//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->model = qModel;
    this->ledger = ledger;
    colCount = 9;
    headerStrings<<"#"<<"V Date"<<"V No."<<"V Type"<<"Amount"<<"Selected Amount"<<"Balance Amount"<<"Method of Adj."<<"";
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
}


QModelIndex GMBillwiseMappingModel::index(int row, int column, const QModelIndex &parent) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(row<=model->rowCount() && column< colCount){
        if((row == model->rowCount() && column <= 1 )|| row < model->rowCount())
//            qDebug()<<Q_FUNC_INFO<<__LINE__;
            return this->createIndex(row,column);
    }
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QModelIndex();
}

QModelIndex GMBillwiseMappingModel::parent(const QModelIndex &child) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QModelIndex();
}

int GMBillwiseMappingModel::rowCount(const QModelIndex &parent) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return model->rowCount();
}

int GMBillwiseMappingModel::columnCount(const QModelIndex &parent) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return colCount;
}

QVariant GMBillwiseMappingModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<index.row()<<index.column()<<role;

    if (index.column() >=1  && role == Qt::TextAlignmentRole) {
        return QVariant(Qt::AlignLeft|Qt::AlignVCenter) ;//& Qt::AlignVCenter;
    } else if (role == Qt::TextAlignmentRole){
        return QVariant(Qt::AlignRight|Qt::AlignVCenter)  ;//Qt::AlignVCenter;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

        switch (index.column()) {
        case 0:
            return index.row() + 1;
        case 1:
            // date
            return model->record(index.row()).value(0).toDate();
        case 2:
            // vno - prefix
            return model->record(index.row()).value(1).toString() + (model->record(index.row()).value(3).toString().length() > 0? " - " + model->record(index.row()).value(3).toString() : "");
        case 3:
            // vtype
            return model->record(index.row()).value(2).toString();
        case 4:
            // amount
            if(model->record(index.row()).value(5).toFloat() > 0) // cr
                return QString::number(qFabs(model->record(index.row()).value(5).toFloat()), 'f', 2) + " Cr";
            else // dr
                return QString::number(qFabs(model->record(index.row()).value(5).toFloat()), 'f', 2) + " Dr";
        case 5:{
            // selected amount
            if(model->record(index.row()).value(7).toString() != "ON ACC" || true ){
                int row1 = getCurrentMapping(model->record(index.row()).value(1).toString(),
                                             model->record(index.row()).value(2).toString(),
                                             model->record(index.row()).value(3).toString());
                if(row1 == -1){
//                    if(model->record(index.row()).value(6).toFloat() > 0) // cr
//                        return QString::number(qFabs(model->record(index.row()).value(6).toFloat()), 'f', 2) + " Cr";
//                    else // dr
//                        return QString::number(qFabs(model->record(index.row()).value(6).toFloat()), 'f', 2) + " Dr";
                }
                else{
//                    qDebug()<<Q_FUNC_INFO<<__LINE__<<ledger->mapList[row1]->CreditAmount<<ledger->mapList[row1]->DebitAmount;
                    if(ledger->mapList[row1]->CreditAmount - ledger->mapList[row1]->DebitAmount > 0)
                        return  QString::number(qFabs(ledger->mapList[row1]->CreditAmount - ledger->mapList[row1]->DebitAmount), 'f', 2) + " Cr";
                    else
                        return  QString::number(qFabs(ledger->mapList[row1]->CreditAmount - ledger->mapList[row1]->DebitAmount), 'f', 2) + " Dr";
                }
            }
            break;
        }
        case 6:
        {
            int row1 = getCurrentMapping(model->record(index.row()).value(1).toString(),
                                         model->record(index.row()).value(2).toString(),
                                         model->record(index.row()).value(3).toString());
            if(row1 == -1)
                return "";
            else{
                float val = model->record(index.row()).value(5).toFloat() - (ledger->mapList[row1]->DebitAmount - ledger->mapList[row1]->CreditAmount);
                if(val > 0)
                    return QString::number(qFabs(val), 'f',2) + " Cr";
                else
                    return QString::number(qFabs(val), 'f',2) + " Dr";
            }
        }
        case 7:
            //method of adj
            return model->record(index.row()).value(7).toString();
//        case 7:
//            return "";


        }
    }

    if(role == Qt::CheckStateRole && index.column() ==8)
    {
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
        int row1 = getCurrentMapping(model->record(index.row()).value(1).toString(),
                                     model->record(index.row()).value(2).toString(),
                                     model->record(index.row()).value(3).toString());
        if(row1 >= 0){
            if(qFabs(ledger->mapList[row1]->DebitAmount - ledger->mapList[row1]->CreditAmount - model->record(index.row()).value(5).toFloat()) > 0)
                return Qt::Unchecked;
            else
                return  Qt::Checked;
        }
        else {
            return Qt::Unchecked;
        }
    }

    return QVariant();

}

bool GMBillwiseMappingModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if (index.isValid() && role == Qt::EditRole) {
        int row1 = getCurrentMapping(model->record(index.row()).value(1).toString(),
                                     model->record(index.row()).value(2).toString(),
                                     model->record(index.row()).value(3).toString());
        if(row1 == -1){
            BillwiseRecordDataModel *map = new BillwiseRecordDataModel;
            map->LedgerID = ledger->LedgerID;
            map->RefVoucherNo = model->record(index.row()).value(1).toString();
            map->RefPrefix = model->record(index.row()).value(3).toString();
            map->RefType = model->record(index.row()).value(2).toString();

            qDebug()<<Q_FUNC_INFO<<__LINE__;
            ledger->mapList.append(map);

            row1 = ledger->mapList.size() - 1;


        }
        if(index.column() == 5){
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<value.toFloat();
            if(value.toFloat() > 0) {
                if(model->record(index.row()).value(5).toFloat() < 0)
                    ledger->mapList[row1]->CreditAmount = value.toFloat();
                else {
                    ledger->mapList[row1]->DebitAmount = value.toFloat();
                }
                if(model->record(index.row()).value(2).toString() == "On Account")
                    ledger->mapList[row1]->MethodOfAdjustment = "ON ACC";
                else
                    ledger->mapList[row1]->MethodOfAdjustment = "AGAINST REFERENCE";
//                qDebug()<<Q_FUNC_INFO<<__LINE__<<row1<<ledger->mapList[row1]->CreditAmount<<ledger->mapList[row1]->DebitAmount;
            }
            else {
                ledger->mapList.removeAt(row1);
            }
        }
        if(index.column() == 8){
            if(value.toBool()){
                if(model->record(index.row()).value(5).toFloat()  > 0)
                    ledger->mapList[row1]->DebitAmount = model->record(index.row()).value(5).toFloat();
                else {
                    ledger->mapList[row1]->CreditAmount = qFabs(model->record(index.row()).value(5).toFloat());
                }
            }
            else {
                ledger->mapList[row1]->DebitAmount = 0;
                ledger->mapList[row1]->CreditAmount = 0;
            }
        }
        emit mapChanged();
        return true;
    }
    return false;
}

Qt::ItemFlags GMBillwiseMappingModel::flags(const QModelIndex &index) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if (!index.isValid())
        return Qt::NoItemFlags;
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}


QVariant GMBillwiseMappingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<section;
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<orientation;
//    qDebug()<<Q_FUNC_INFO<<__LINE__<<role;
    if (role != Qt::DisplayRole){
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
        return QVariant();
    }



    if(section == colCount-1) {
//        qDebug()<<Q_FUNC_INFO<<__LINE__;
        if( orientation == Qt::Horizontal && role == Qt::DecorationRole){
//            qDebug()<<Q_FUNC_INFO<<__LINE__;
            QPixmap pixmap = QPixmap(":/icons/icons/navigation/svg/production/ic_check_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#ffffff")));
            pixmap.setMask(mask);
            QIcon icon = QIcon(pixmap);
            return  QVariant::fromValue(icon);
        }

    }


    else if (orientation == Qt::Horizontal) {
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<headerStrings[section];
        return headerStrings[section];
    }
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return QVariant();
}

int GMBillwiseMappingModel::getCurrentMapping(QString voucherNo, QString voucherType, QString voucherPrefix) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    for(int i=0; i<ledger->mapList.size(); i++){
        if(ledger->mapList[i]->RefVoucherNo == voucherNo &&
                ledger->mapList[i]->RefType == voucherType &&
                ledger->mapList[i]->RefPrefix == voucherPrefix){
            return i;
        }
    }
    return -1;
}
