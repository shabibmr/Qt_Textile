#ifndef LEDGERVOUCHERTABLEVIEW_H
#define LEDGERVOUCHERTABLEVIEW_H

#include <QTableView>
#include <QTableWidget>
#include <datamodels/transaction/generalvoucherdataobject.h>
#include <QHeaderView>
#include <QKeyEvent>


class LedgervoucherTableView : public QTableView
{
    Q_OBJECT

    void updateLastRowGeometry();

public:
    explicit LedgervoucherTableView(GeneralVoucherDataObject *obj,
                                    int Type,
                                    QTableView *parent= Q_NULLPTR);
    virtual ~LedgervoucherTableView() override;

    QTableWidget *lastRow;

    QString RowName="Grand Total";

    void setRowName(const QString &value);

    QList<int> avlcols {1,2,3};

    int vType;

    int numColumns;

    void setColumns();

    void setVoucher(GeneralVoucherDataObject *value);

    void setAvlcols(const QList<int> &value);

public slots:
    void setValues();
    void updateSectionWidth(int logicalIndex, int, int newSize);

    void setFocusAfterInsert(int row);


protected:
    void resizeEvent(QResizeEvent *event) override;

    GeneralVoucherDataObject *voucher;

private:
    virtual void keyPressEvent(QKeyEvent *pEvent) override
    {
        QAbstractItemView::keyPressEvent(pEvent); //comment this to enable code
        return;
        if (pEvent->key() == Qt::Key_Return)
        {
            // we captured the Enter key press, now we need to move to the next row
            qint32 nNextRow = currentIndex().row();
            qint32 nNextColumn = currentIndex().column();

            //            qDebug()<<"From "<<nNextRow<<":"<<nNextColumn;
            //            nNextColumn = getnextColNumber(nNextColumn);

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
                // we are all the way down, we can't go any further
                qDebug()<<"Reached Last Row";
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
                // qDebug()<<"Sttart Edit";
                // if we're not editing, start editing
                edit(currentIndex());
            }
        }
        else
        {
            // any other key was pressed, inform base class
            QAbstractItemView::keyPressEvent(pEvent);
        }
    }


    int getnextColNumber(int needle){
        int closest = avlcols[0];
        for ( int i = 0; i < avlcols.length(); ++i ) {
            if ( avlcols[ i ] - needle  >0  )
            {
                closest = avlcols[i];
                break;
            }
        }

        return closest;
    }


};

#endif // LEDGERVOUCHERTABLEVIEW_H
