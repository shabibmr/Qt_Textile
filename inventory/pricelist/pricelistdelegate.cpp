#include "pricelistdelegate.h"



PriceListDelegate::PriceListDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                                               QSqlQueryModel *model,
                                                               UomDataBaseHelper *uHelper)
{
    this->model = model;
    this->itemHelper = iHelper;
    this->uomHelper = uHelper;
    qDebug()<<"Constr Set";
}

PriceListDelegate::~PriceListDelegate()
{

}

void PriceListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    //    initStyleOption(&option,index);

    QRect rect = option.rect;
    if(index.column()==1 && index.row()== index.model()->rowCount()-1){
        //        qDebug()<<"Painting last elem";
        QFont italic; italic.setItalic(true);
        painter->setFont(italic);
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
        else
            painter->setPen(option.palette.color(cg, QPalette::Text));

        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
                          Qt::AlignLeft|Qt::AlignVCenter, "Search...");
    }
    else if(index.column()==8){
        QStyleOptionButton button;
        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 20;//the X coordinate
        x = r.left()+ (r.width() -20)/2;
        y = r.top();//the Y coordinate
        y = r.top()+ (r.height() -20)/2;
        w = 15;//button width(based on the requirement)
        h = 15;//button height(based on the requirement)
        button.icon= QIcon(QString::fromUtf8(":/icons/trash.ico"));
        button.iconSize = QSize(20,20);
        button.rect = QRect(x,y,w,h);
        button.text = "";//no text . since if text will be given then it will push the icon to left side based on the coordinates .
        button.state = QStyle::State_Enabled;
        //QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
        QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
    }

    else
        QStyledItemDelegate::paint(painter,option,index);
}


QWidget *PriceListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    switch (index.column())
    {

    case 1 :
    { GMInventoryItemSearch *itemLine = new GMInventoryItemSearch(itemHelper,model);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMInventoryItemSearch::itemSelected,
                         this, &PriceListDelegate::commitAndCloseItemEditor);
        return itemLine;
        break;
    }
    case 2 :{
        QString itemID = index.model()->data(index, Qt::UserRole).toString();

        GMUOMSearchWidget *itemLine
                = new GMUOMSearchWidget(
                    uomHelper,
                    uomHelper->getUomModelByItem(itemID));

        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMUOMSearchWidget::itemSelected,
                         this, &PriceListDelegate::commitAndCloseUOMEditor);
        return itemLine;
        break;
    }

    case 4
    :
        QToolButton* button = new QToolButton(parent);
        button->setIcon(QIcon(":/icons/trash.ico"));
        button->setProperty("id",index.row());
        button->setEnabled(true);
        connect(button,&QToolButton::clicked,
                this,&PriceListDelegate::deleteButtonClicked);
        return button;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void PriceListDelegate::deleteButtonClicked(){

    qDebug()<<"Clicked";
    qDebug()<<sender()->property("id") <<"count";
}

void PriceListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {
    case 1:{
        QString value;
        if(index.row()==index.model()->rowCount())
            value ="";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();
            qDebug()<<"valid String : "<<value;
        }
        GMInventoryItemSearch *itemLine = static_cast <GMInventoryItemSearch*>(editor);
        itemLine->setText(value);
        //        itemLine->setNewText(value,"");

        break;

    }
    case 2 :{
        QString itemID = index.model()->data(index, Qt::UserRole).toString();
        UomDataModel uomObj =  uomHelper->getUomObjectByID(index.model()->data(index, Qt::UserRole+1).toString());

        qDebug()<< "ITEM ID FROM USER ROLE :" << itemID;
        GMUOMSearchWidget *uomLine
                = new GMUOMSearchWidget(
                    uomHelper,
                    uomHelper->getUomModelByItem(itemID));
        uomLine->setItem(uomObj);
        uomLine->setFrame(false);

        break;
    }

    case 8:
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        break;
    }

    QStyledItemDelegate::setEditorData(editor, index);

}

void PriceListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {
    case 1:{
        GMInventoryItemSearch *itemLine = qobject_cast<GMInventoryItemSearch *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.ItemName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.ItemName;
        if(itemLine->item.ItemID.length())
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;
    }
    case 2 :
    {
        QString itemID = index.model()->data(index, Qt::UserRole).toString();

        //        qDebug()<< "ITEM ID FROM USER ROLE :" << itemID;
        GMUOMSearchWidget *itemLine = qobject_cast<GMUOMSearchWidget *>(editor);
        if(itemLine->uom._id.length())
            model->setData(index,QVariant::fromValue(itemLine->uom));
        return;

    }
    case 8:
    {
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        qDebug()<<"Delgate setting data at "<<index.row();
        model->setData(index,QVariant::fromValue(index.row()));
    }

    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void PriceListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void PriceListDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    qDebug()<<".............................COMMITTING DATA...........................";
    GMInventoryItemSearch *editor = qobject_cast<GMInventoryItemSearch *>(sender());
    qDebug()<<"item Set as"<<editor->item.ItemName;
    emit commitData(editor);
    emit closeEditor(editor);
}

void PriceListDelegate::commitAndCloseUOMEditor(QModelIndex index)
{
    qDebug()<<".............................COMMITTING DATA...........................";
    GMUOMSearchWidget *editor = qobject_cast<GMUOMSearchWidget *>(sender());
    qDebug()<<"item Set as"<<editor->uom.UomName;
    emit commitData(editor);
    emit closeEditor(editor);
}
