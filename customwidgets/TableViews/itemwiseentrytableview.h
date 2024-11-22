#ifndef ITEMWISEENTRYTABLEVIEW_H
#define ITEMWISEENTRYTABLEVIEW_H

#include <QTableView>
#include <QTableWidget>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>
#include "customwidgets/Material/components/qtmaterialscrollbar.h"

class ItemwiseEntryTableView : public QTableView
{
    Q_OBJECT


public:

    explicit ItemwiseEntryTableView(GeneralVoucherDataObject *obj,QTableView *parent= nullptr);
    ~ItemwiseEntryTableView() override{}


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

    QList<int> avlcols {1,2,3,4,5,6,8,9};
    void resizeEvent(QResizeEvent *event) override;

    GeneralVoucherDataObject *voucher;
    virtual void keyPressEvent(QKeyEvent *pEvent) override

    {
        if (pEvent->key() == Qt::Key_Return || pEvent->key() == Qt::Key_Enter ||  pEvent->key() == Qt::Key_Tab)
        {
            qDebug()<<"Button pressed "<< pEvent->key();
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

            QModelIndex oNextIndex = model()->index(nNextRow, nNextColumn);
            setCurrentIndex(oNextIndex);
            if (state() == QAbstractItemView::EditingState)
            {
                qDebug()<<"Selecting Next"<<oNextIndex;
                selectionModel()->select(oNextIndex, QItemSelectionModel::ClearAndSelect);
            }
            else
            {
                 qDebug()<<"Editing Next"<<oNextIndex;
                edit(oNextIndex);
            }
            return;
        }
        else
        {
            QAbstractItemView::keyPressEvent(pEvent);
        }
    }

    // QWidget interface

};


#endif // ITEMWISEENTRYTABLEVIEW_H
