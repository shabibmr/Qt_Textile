#include "gminventorylistitemdelegate.h"
#include "datamodels/inventory/sales_inventory_item/inventoryitemdatamodel.h"

GMInventoryListItemDelegate::GMInventoryListItemDelegate()
{

}

GMInventoryListItemDelegate::~GMInventoryListItemDelegate()
{

}

void GMInventoryListItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    int row = index.row();
    //    QString ItemName  = index.model()->data(index.model()->index(row,1), Qt::DisplayRole).toString();
    //    QString uom = index.model()->data(index.model()->index(row,2), Qt::DisplayRole).toString();
    //    QString quantity = index.model()->data(index.model()->index(row,3), Qt::DisplayRole).toString();

    inventoryItemDataModel item = qvariant_cast<inventoryItemDataModel>  (index.model()->data(index, Qt::UserRole+9));

    QRect rect = option.rect;
    int width = rect.width();
    int height = rect.height();

    int margins =3;

    QRect sRect;
    QRect pRect;

//    painter->drawRect(rect);

    painter->drawText(QRect(rect.left()+margins, rect.top()+margins, width/2, rect.height()/2),
                      Qt::AlignLeft|Qt::AlignVCenter, item.ItemName,&sRect);

    painter->drawText(QRect(rect.left()+margins, rect.top()+margins+rect.height()/2+margins, rect.width()/2, rect.height()/2),
                      Qt::AlignLeft|Qt::AlignVCenter,QString::number(item.price,'f',2),&pRect);


    painter->drawText(QRect(width/2+margins, rect.top()+margins, rect.width()/4, rect.height()/2),
                      Qt::AlignRight|Qt::AlignVCenter,QString::number(item.quantity / item.uomObject.convRate,'f',
                      item.uomObject.decimalPoints) + " "+ item.uomObject.UomSymbol,&sRect);

    painter->drawText(QRect(width*3/4+margins, rect.top()+margins, rect.width()-(width*3/4+margins), rect.height()/2),
                      Qt::AlignRight|Qt::AlignVCenter, QString::number(item.subTotal,'f',2) + "");



   // QStyledItemDelegate::paint(painter,option, index);
}

QWidget *GMInventoryListItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::createEditor(parent,option, index);
}

void GMInventoryListItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QStyledItemDelegate::setEditorData(editor, index);
}

void GMInventoryListItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QStyledItemDelegate::setModelData(editor,model, index);
}

void GMInventoryListItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

bool GMInventoryListItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    QStyledItemDelegate::editorEvent(event,model,option, index);
}

QSize GMInventoryListItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(result.height()*2);
    return result;
}
