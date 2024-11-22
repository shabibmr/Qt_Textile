#include "gmlistwidget.h"

GMListWidget::GMListWidget(QListWidget *parent): QListWidget (parent)
{

}

GMListWidget::~GMListWidget()
{

}

void GMListWidget::setData(QList<inventoryItemDataModel> list)
{
    qDebug()<<"Setting data";
    for(inventoryItemDataModel invItem:list){
        SearchListItemWidget *item = new SearchListItemWidget();
        item->setData(invItem.ItemName,invItem.brandName,invItem.ItemCode,invItem.ItemID);
        QListWidgetItem *wItem = new QListWidgetItem();
        item->setProperty("item",1);
        wItem->setSizeHint(QSize(150,60));
        this->addItem(wItem);
        this->setItemWidget(wItem,item);

    }
    qDebug()<<"List Widget Size = "<<this->count();
}

void GMListWidget::setFilter(QString text)
{

}

void GMListWidget::focusOutEvent(QFocusEvent *e)
{
    this->close();
}
