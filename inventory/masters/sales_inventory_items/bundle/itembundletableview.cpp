
#include "itembundletableview.h"

ItemBundleTableView::ItemBundleTableView(QTableView *parent ):QTableView(parent)
{
    this->installEventFilter(this);
}

void ItemBundleTableView::setFocusAfterInsert(int row)
{
    QModelIndex index =  model()->index(row, 2);
    this->setCurrentIndex(index);
    this->scrollTo(index);
}

void ItemBundleTableView::setFocusToFirstRow(int row)
{
    this->setFocus();
    QModelIndex index =  model()->index(row, avlcols[0]);
    this->setCurrentIndex(index);
}

