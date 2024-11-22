
#include "itembundlelistdelegate.h"

ItemBundleListDelegate::ItemBundleListDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                               QSqlQueryModel *model, QString priceListId ,QObject *parent)
    : QStyledItemDelegate{parent}
{
    this->model = model;
    this->itemHelper = iHelper;
    this->priceListId = priceListId;
}

void ItemBundleListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    QRect rect = option.rect;

    if(index.column()==1 && index.row()== index.model()->rowCount()-1){
        //        qDebug()<<"Painting last elem";
        QFont italic; italic.setItalic(true);
        painter->setFont(italic);
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(QColor(242,139,47),3,Qt::SolidLine,Qt::RoundCap));
            painter->drawRect(rect);
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));

        }
        else
            painter->setPen(option.palette.color(cg, QPalette::Text));

        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
                          Qt::AlignLeft|Qt::AlignVCenter, "Search...");
    }
    else{
        return QStyledItemDelegate::paint(painter,option,index);
    }

}



QWidget *ItemBundleListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    switch(index.column()){
    case 1:{
        GMInventoryItemSearch *itemLine = new GMInventoryItemSearch(itemHelper,model, priceListId);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMInventoryItemSearch::itemSelected,
                         this, &ItemBundleListDelegate::commitAndCloseItemEditor);
        return itemLine;
        break;
    }
    default: QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void ItemBundleListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    switch(index.column()){
    case 1:{

        QString value;
        if(index.row()==index.model()->rowCount())
            value ="";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();
        }
        GMInventoryItemSearch *itemLine = static_cast <GMInventoryItemSearch*>(editor);
        itemLine->setText(value);
        //        itemLine->setNewText(value,"");
        break;
    }
    default:  QStyledItemDelegate::setEditorData(editor, index);
    }
}

void ItemBundleListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch(index.column()){
    case 1:
    {
        GMInventoryItemSearch *itemLine = qobject_cast<GMInventoryItemSearch *>(editor);
        if(itemLine->item.ItemID.length())
            model->setData(index, QVariant::fromValue((itemLine->item)));
        return;
    }
    default:   QStyledItemDelegate::setModelData(editor, model, index);
    }
}

//void ItemBundleListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    editor->setGeometry(option.rect);
//}


void ItemBundleListDelegate::commitAndCloseItemEditor(QModelIndex index)
{

    GMInventoryItemSearch *editor = qobject_cast<GMInventoryItemSearch *>(sender());

    emit commitData(editor);
    emit closeEditor(editor);
}
