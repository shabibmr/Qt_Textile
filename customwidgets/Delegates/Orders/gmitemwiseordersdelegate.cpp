#include "gmitemwiseordersdelegate.h"

#include <QtGui>
#include <QApplication>
#include <QToolButton>
#include <QDateEdit>
GMItemwiseOrdersDelegate::~GMItemwiseOrdersDelegate()
{

}


GMItemwiseOrdersDelegate::GMItemwiseOrdersDelegate(LedgerMasterDatabaseHelper *iHelper, QSqlQueryModel *model, UomDataBaseHelper *uHelper,SalesInventoryItemDatabaseHelper *itHelper,
                                                   QSqlQueryModel *itemmodel, QString priceListId)
{
    this->model = model;
    this->ledHelper = iHelper;
    this->uomHelper = uHelper;

    this->imodel = itemmodel;
    this->priceID = priceListId;
    this->itemsHelper = itHelper;
}

void GMItemwiseOrdersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;

    if (option.state & QStyle::State_Selected)
    {
        painter->setPen(QPen(QColor(242,139,47),6,Qt::SolidLine,Qt::RoundCap));
        painter->drawRect(rect);
    }
    else{
        painter->setPen(QPen(QColor(Qt::transparent),6,Qt::SolidLine,Qt::RoundCap));
        painter->drawRect(rect);
    }


    if(index.column()==1 && index.row()== index.model()->rowCount()-1){
        //        qDebug()<<"Painting last elem";
        QFont italic; italic.setItalic(true);
        painter->setFont(italic);
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
        }
        else
            painter->setPen(option.palette.color(cg, QPalette::Text));

        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
                          Qt::AlignLeft|Qt::AlignVCenter, "Search...");
    }
    if(index.column()==3 && index.row()== index.model()->rowCount()-1){

        QFont italic; italic.setItalic(true);
        painter->setFont(italic);
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
        {

            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));

        }
        else
            painter->setPen(option.palette.color(cg, QPalette::Text));

        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
                          Qt::AlignLeft|Qt::AlignVCenter, "Search...");
    }
    else if(index.column()== 6){

        if(index.model()->data(index,Qt::UserRole+5).toString().length()==0){
            painter->fillRect(rect,QBrush(QColor(111,111,111,211)));
        }
        else{

            painter->setPen(QPen(Qt::black));
            painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-7, rect.height()),
                              Qt::AlignRight|Qt::AlignVCenter,  index.model()->data(index,Qt::DisplayRole).toString());
            QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;


            painter->setPen(QPen(Qt::black));
            painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-3, rect.height()),
                              Qt::AlignLeft|Qt::AlignVCenter,itemsHelper->getInventoryItemNameByID(index.model()->data(index,Qt::UserRole+5).toString()));
        }
    }
    else if(index.column()==11){
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
        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(QColor(242,139,47),6,Qt::SolidLine,Qt::RoundCap));
            painter->drawRect(rect);
        }
        QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
    }
    else

    {
        QStyledItemDelegate::paint(painter,option,index);
    }


}

QWidget *GMItemwiseOrdersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 1 :
    {
        GMInventoryItemSearch *itemLine = new GMInventoryItemSearch(itemsHelper,imodel, priceID);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMInventoryItemSearch::itemSelected,
                         this, &GMItemwiseOrdersDelegate::commitAndCloseItemEditor);
        return itemLine;

    }
    case 2 :
    {
        QDateEdit *dateLine = new QDateEdit();
        dateLine->setFrame(false);
        dateLine->setParent(parent);
        dateLine->setCalendarPopup(true);
        return dateLine;
    }

    case 3 :
    {
        GMLedgerLineWidget *itemLine = new GMLedgerLineWidget(ledHelper,model);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMLedgerLineWidget::itemSelected,
                         this, &GMItemwiseOrdersDelegate::commitAndCloseLedgerEditor);
        return itemLine;
    }
    case 4 :
    {
        QString itemID = index.model()->data(index, Qt::UserRole).toString();
        GMUOMSearchWidget *itemLine
                = new GMUOMSearchWidget(
                    uomHelper,
                    uomHelper->getUomModelByItem(itemID));

        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMUOMSearchWidget::itemSelected,
                         this, &GMItemwiseOrdersDelegate::commitAndCloseUOMEditor);
        return itemLine;

    }
    case 11:
        QToolButton* button = new QToolButton(parent);
        button->setIcon(QIcon(":/icons/trash.ico"));
        button->setProperty("id",index.row());
        button->setEnabled(true);

        //        connect(button,&QToolButton::clicked,
        //                this,&GMInventoryTransactionDelegate::deleteButtonClicked);
        return button;
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMItemwiseOrdersDelegate::deleteButtonClicked(){

    qDebug()<<"Clicked";
    qDebug()<<sender()->property("id") <<"count";
    model->removeRows(sender()->property("id").toInt(),1);

}

void GMItemwiseOrdersDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {
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
    case 3:
    {
        QString value;
        if(index.row()==index.model()->rowCount())
            value ="";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();

        }
        GMLedgerLineWidget *itemLine = static_cast <GMLedgerLineWidget*>(editor);
        itemLine->setText(value);
        //        itemLine->setNewText(value,"");

        break;
    }
    case 4 :
    {
        QString itemID = index.model()->data(index, Qt::UserRole).toString();
        UomDataModel uomObj =  uomHelper->getUomObjectByID(index.model()->data(index, Qt::UserRole).toString());


        GMUOMSearchWidget *uomLine
                = new GMUOMSearchWidget(
                    uomHelper,
                    uomHelper->getUomModelByItem(itemID));
        uomLine->setItem(uomObj);
        uomLine->setFrame(false);

        break;
    }
    case 11:
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        break;
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void GMItemwiseOrdersDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {

    case 1:{
        GMInventoryItemSearch *itemLine = qobject_cast<GMInventoryItemSearch *>(editor);
        if(itemLine->item.ItemID.length())
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;
    }
    case 3:{
        GMLedgerLineWidget *itemLine = qobject_cast<GMLedgerLineWidget *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(itemLine->item.LedgerID.length())
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;
    }
    case 4 :
    {
        QString itemID = index.model()->data(index, Qt::UserRole).toString();

        //        qDebug()<< "ITEM ID FROM USER ROLE :" << itemID;
        GMUOMSearchWidget *itemLine = qobject_cast<GMUOMSearchWidget *>(editor);
        if(itemLine->uom._id.length())
            model->setData(index,QVariant::fromValue(itemLine->uom));
        return;

    }
    case 11:
    {
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        //        model->setData(index,QVariant::fromValue(index.row()));
        return;
    }
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMItemwiseOrdersDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void GMItemwiseOrdersDelegate::commitAndCloseLedgerEditor(QModelIndex index)
{
    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
        qDebug()<<"item Set as"<<editor->item.LedgerName;
    emit commitData(editor);
    emit closeEditor(editor);
}

void GMItemwiseOrdersDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    GMInventoryItemSearch *editor = qobject_cast<GMInventoryItemSearch *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
    emit commitData(editor);
    emit closeEditor(editor);
}

void GMItemwiseOrdersDelegate::commitAndCloseDateEditor(QModelIndex index)
{
    QDateEdit *editor = qobject_cast<QDateEdit *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
    emit commitData(editor);
    emit closeEditor(editor);
}


void GMItemwiseOrdersDelegate::commitAndCloseUOMEditor(QModelIndex index)
{
    GMUOMSearchWidget *editor = qobject_cast<GMUOMSearchWidget*>(sender());
    //    qDebug()<<"item Set as"<<editor->item.UomName;
    emit commitData(editor);
    emit closeEditor(editor);
}


bool GMItemwiseOrdersDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 11 )
        {
            QMouseEvent * e = (QMouseEvent *)event;
            int clickX = e->x();
            int clickY = e->y();
            QRect r = option.rect;//getting the rect of the cell
            int x,y,w,h;
            x = r.left() + r.width() - 30;//the X coordinate
            y = r.top();//the Y coordinate
            w = 15;//button width
            h = 15;//button height
            model->removeRows(index.row(),1);
            qDebug()<<"Deleting row :L "<<index.row();
        }
        return true;
    }
    else{
        QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}
