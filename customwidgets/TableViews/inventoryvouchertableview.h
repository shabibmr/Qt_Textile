#ifndef INVENTORYVOUCHERTABLEVIEW_H
#define INVENTORYVOUCHERTABLEVIEW_H

#include <QTableView>
#include <QTableWidget>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>
#include "customwidgets/Material/components/qtmaterialscrollbar.h"


class InventoryVoucherTableView : public QTableView
{
    Q_OBJECT



public:
    explicit InventoryVoucherTableView(GeneralVoucherDataObject *obj,QTableView *parent= nullptr);
    ~InventoryVoucherTableView() override{}

    QTableWidget *lastRow;
    QtMaterialScrollBar *hScrollBarB;
    QtMaterialScrollBar *hScrollBarA;


    QString RowName="Grand Total";

    void setRowName(const QString &value);
    void setColumns();

    void setVoucher(GeneralVoucherDataObject *value);

public slots:
    void setValues();
    void updateSectionWidth(int logicalIndex, int, int newSize);
    void setFocusAfterInsert(int row);
    void setFocusToFirstRow(int row =0);
    void updateLastRowGeometry();
protected:
    QList<int> avlcols {1,2,3,4};
    void resizeEvent(QResizeEvent *event) override;

    GeneralVoucherDataObject *voucher;
    virtual void keyPressEvent(QKeyEvent *pEvent) override
    {
        if (pEvent->key() == Qt::Key_Return||pEvent->key() == Qt::Key_Enter ||  pEvent->key() == Qt::Key_Tab)
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
        }
        else
        {
            QAbstractItemView::keyPressEvent(pEvent);
        }
    }



    // QWidget interface


    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // INVENTORYVOUCHERTABLEVIEW_H
