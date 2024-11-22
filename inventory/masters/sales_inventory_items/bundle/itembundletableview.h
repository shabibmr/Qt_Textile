
#ifndef ITEMBUNDLETABLEVIEW_H
#define ITEMBUNDLETABLEVIEW_H

#include <QTableView>

#include <QObject>
#include <QKeyEvent>

class ItemBundleTableView : public QTableView
{
    Q_OBJECT
public:
    ItemBundleTableView(QTableView *parent = Q_NULLPTR);

public slots:
    void setFocusAfterInsert(int row);
    void setFocusToFirstRow(int row =0);
protected:
    QList<int> avlcols {1,2};
    virtual void keyPressEvent(QKeyEvent *pEvent) override
    {
        if (pEvent->key() == Qt::Key_Return || pEvent->key() == Qt::Key_Enter ||  pEvent->key() == Qt::Key_Tab)
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

#endif // ITEMBUNDLETABLEVIEW_H
