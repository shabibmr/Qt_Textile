#ifndef REPORTSTABLEVIEW_H
#define REPORTSTABLEVIEW_H

#include <QTableView>
#include <QTableWidget>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>
#include <customwidgets/gmreportsortfilterproxymodel.h>
#include "customwidgets/Material/components/qtmaterialscrollbar.h"
#include <QSqlQueryModel>
#include <QSqlRecord>

class ReportsTableView: public QTableView
{

    Q_OBJECT

    void updateLastRowGeometry();

public:
    explicit ReportsTableView(QString rowName = "Grand Total",bool show=false, QTableView *parent= nullptr);
    ~ReportsTableView() override{}

    QTableWidget *lastRow;
    QtMaterialScrollBar *hScrollBarB;
    QtMaterialScrollBar *hScrollBarA;

    bool useSort=false;


    QString RowName="Grand Total";

    void setRowName(const QString &value,const int &col,int alignment = Qt::AlignLeft|Qt::AlignVCenter);
    void setColumns();

    void gmHideColumn(int i);
    QList<int> avlcols;
    //    void setVoucher(GeneralVoucherDataObject *value);

    void setAvlcols(const QList<int> &value);

    QSqlQueryModel *modelQry;
    GMReportSortFilterProxyModel *sort;

    void setModelQry(QSqlQueryModel *value);

public slots:
    void setValues();
    void updateSectionWidth(int logicalIndex, int, int newSize);
    //    void setFocusAfterInsert(int row);
    //    void setFocusToFirstRow(int row =0);
protected:

    void resizeEvent(QResizeEvent *event) override;

    //    GeneralVoucherDataObject *voucher;
    virtual void keyPressEvent(QKeyEvent *pEvent) override
    {
        if (pEvent->key() == Qt::Key_Return)
        {

            qint32 nNextRow = currentIndex().row();
            qint32 nNextColumn = currentIndex().column();

            int closest = avlcols[0];
            int i=0;
            for (  i = 0; i < avlcols.length(); ++i ) {
                if ( avlcols[ i ] - nNextColumn  >0  )
                {
                    closest = avlcols[i];
                    break;
                }
            }

            if(i>=avlcols.length())
                nNextRow ++;

            nNextColumn = closest;

            if (nNextRow + 1 > model()->rowCount(currentIndex()))
            {
                nNextRow = nNextRow - 1;
            }

            if (state() == QAbstractItemView::EditingState)
            {
                QModelIndex oNextIndex = model()->index(nNextRow, nNextColumn);
                setCurrentIndex(oNextIndex);
                selectionModel()->select(oNextIndex, QItemSelectionModel::ClearAndSelect);
            }
            else
            {
                edit(currentIndex());
            }

            emit doubleClicked(currentIndex());
        }
        else
        {
            QAbstractItemView::keyPressEvent(pEvent);
        }
    }




    // QAbstractItemView interface
public:
//    void setModel(QAbstractItemModel *model) override;
    void setSort(GMReportSortFilterProxyModel *value);
    void setUseSort(bool value);

    void showVerticalHeader(bool b);
};

#endif // REPORTSTABLEVIEW_H
