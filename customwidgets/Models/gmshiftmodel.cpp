#include "gmshiftmodel.h"
#include <QDebug>
#include <QColor>

GMShiftModel::GMShiftModel(ShiftDataModel *model, int tablePos, QObject *parent)
{

//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->shift = model;
    this->colCount = 2;
    switch (tablePos) {
    case 1:{
        this->rowCnt = 8;
        break;
    }
    case 2:{
        this->rowCnt = 16;
        break;
    }

    }
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

}

QModelIndex GMShiftModel::index(int row, int column, const QModelIndex &parent) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(row< rowCnt && column< colCount){
            return this->createIndex(row,column);
    }
    return QModelIndex();
}

QModelIndex GMShiftModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

int GMShiftModel::rowCount(const QModelIndex &parent) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    return rowCnt;
}

int GMShiftModel::columnCount(const QModelIndex &parent) const
{
    return colCount;
}

QVariant GMShiftModel::data(const QModelIndex &index, int role) const
{
//    qDebug()<<Q_FUNC_INFO<<__LINE__;
    int row = index.row();
    int col = index.column();

    if(role == Qt::ForegroundRole ){
        if(rowCnt == 16 && row == 13){
            if(shift->CashInCounter > shift->totalCash)
                return QVariant( QColor( Qt::green ) );
            else if (shift->CashInCounter == shift->totalCash) {
            }
            else {
                return QVariant( QColor( Qt::red ) );
            }
        }
    }

    if(role == Qt::TextAlignmentRole && col == 1 && rowCnt == 16){
        return Qt::AlignRight;
    }

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<col<<rowCnt;
        if(col == 0){
            if(rowCnt == 8){
//                qDebug()<<Q_FUNC_INFO<<__LINE__;
                switch (row) {
                case 0:
                    return "Cashier Name";
                case 1:
                    return "Bill From";
                case 2:
                    return "Bill To";
                case 3:
                    return "Bills Count";
                case 4:
                    return "Start Time";
                case 5:
                    return "End Time";
                case 6:
                    return "POS #";
                case 7:
                    return "Shift ID";



                }
            }
            if(rowCnt == 16){
//                qDebug()<<Q_FUNC_INFO<<__LINE__;
                switch (row) {
                case 0:
                    return "Opening Cash";
                case 1:
                    return "Sales Total";
                case 2:
                    return "Sales By Cash";
                case 3:
                    return "Sales By Card";
                case 4:
                    return "Sales By Credit";
                case 5:
                    return "Cash Purchase";
                case 6:
                    return "Cash Payments";
                case 7:
                    return "Cash Receipts";
                case 8:
                    return "Advance Received";
                case 9:
                    return "Total Cash";
                case 10:
                    return "Cash In Counter";
                case 11:
                    return "Cash Withdrawn";
                case 12:
                    return "Cash Balance in Counter";
                case 13:{
                    if(shift->CashInCounter > shift->totalCash)
                        return "Till Difference \n (Cash Excess)";
                    else if (shift->CashInCounter == shift->totalCash) {
                        return  "Till Difference";
                    }
                    else {
                        return "Till Difference \n (Cash Short)";
                    }
                }
                case 14:
                    return "Taxable Total";
                case 15:
                    return "Output Tax Amount";


                }
            }
        }
        else if (col == 1) {
            if(rowCnt == 8){
                switch (row) {
                case 0:
                    return shift->cashierName;
                case 1:
                    return shift->BillsFrom;
                case 2:
                    return shift->BillsTo;
                case 3:
                    return shift->billsCount;
                case 4:
                    return shift->ShiftStartDateTime;
                case 5:
                    return shift->ShiftEndDateTime;
                case 6:
                    return shift->POSNumber;
                case 7:
                    return shift->shiftID;



                }
            }
            else if(rowCnt == 16){
                switch (row) {
                case 0:
                    return QString::number(shift->OpeningCash, 'f', 2);
                case 1:
                    return QString::number(shift->SalesTotal, 'f', 2);
                case 2:
                    return QString::number(shift->CashSales, 'f', 2);
                case 3:
                    return QString::number(shift->CardSales, 'f', 2);
                case 4:
                    return QString::number(shift->CreditSales, 'f', 2);
                case 5:
                    return QString::number(shift->CashPurchase, 'f', 2);
                case 6:
                    return QString::number(shift->CashPayments, 'f', 2);
                case 7:
                    return QString::number(shift->CashReceipts, 'f', 2);
                case 8:
                    return QString::number(shift->advanceRecieved, 'f', 2);
                case 9:
                    return QString::number(shift->totalCash, 'f', 2);
                case 10:
                    return QString::number(shift->CashInCounter, 'f', 2);
                case 11:
                    return QString::number(shift->CashWithDrawn, 'f', 2);
                case 12:
                    return QString::number(shift->CashBalance, 'f', 2);
                case 13:
                    return QString::number(shift->TillDifference, 'f', 2);
                case 14:
                    return QString::number(shift->taxableTotal, 'f', 2);
                case 15:
                    return QString::number(shift->vatTotal, 'f', 2);


                }
            }

        }
    }
//    qDebug()<<Q_FUNC_INFO<<__LINE__;

    return QVariant();
}

bool GMShiftModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();
        if(rowCnt == 16 && index.column() == 1){
            if(row == 0){
                shift->OpeningCash = value.toFloat();
                shift->totalCash = shift->OpeningCash + shift->CashSales - shift->CashPurchase
                        -shift->CashPayments + shift->CashReceipts;

                shift->TillDifference   =   shift->CashInCounter - shift->totalCash ;
                shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;
            }
            else if (row == 11) {
                shift->CashWithDrawn = value.toFloat();
                shift->CashBalance      = shift->CashInCounter - shift->CashWithDrawn;

            }

        }

        return true;
    }

    return false;
}

Qt::ItemFlags GMShiftModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable| QAbstractItemModel::flags(index) ;
}
