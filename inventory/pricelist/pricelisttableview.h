#ifndef PRICELISTTABLEVIEW_H
#define PRICELISTTABLEVIEW_H

#include <QTableView>
#include <QTableWidget>
#include <QAbstractItemModel>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>
#include "customwidgets/Material/components/qtmaterialscrollbar.h"


class PriceListTableView : public QTableView
{
    Q_OBJECT

    void updateLastRowGeometry();

public:
    explicit PriceListTableView(PriceListDataObject *obj,QTableView *parent= nullptr);
    ~PriceListTableView() override{}

    QTableWidget *lastRow;
    QtMaterialScrollBar *hScrollBarB;
    QtMaterialScrollBar *hScrollBarA;

    QString RowName="Grand Total";

    void setRowName(const QString &value);
    void setColumns();

    void setPriceList(PriceListDataObject *value);

public slots:
    void setValues();
    void updateSectionWidth(int logicalIndex, int, int newSize);
    void setFocusAfterInsert(int row);
protected:
    QList<int> avlcols {1,3};
    void resizeEvent(QResizeEvent *event) override;

    PriceListDataObject *priceList;
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
        }
        else
        {
            QAbstractItemView::keyPressEvent(pEvent);
        }
    }


};

#endif // PRICELISTTABLEVIEW_H
