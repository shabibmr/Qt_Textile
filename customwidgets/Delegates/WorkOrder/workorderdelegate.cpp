#include "workorderdelegate.h"
#include <QtGui>
#include <QApplication>


WorkOrderDelegate::WorkOrderDelegate(SalesInventoryItemDatabaseHelper *iHelper,
                                     QSqlQueryModel *model,
                                     UomDataBaseHelper *uHelper)
{
    this->model = model;
    this->itemHelper = iHelper;
    this->uomHelper = uHelper;
}

WorkOrderDelegate::~WorkOrderDelegate()
{

}

void WorkOrderDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
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
    else if(index.column()==7){
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
        button.icon= QIcon(QString::fromUtf8(":/icons/icons/action/svg/production/ic_print_24px.svg"));
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

QWidget *WorkOrderDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {

    case 1 :
    { GMInventoryItemSearch *itemLine = new GMInventoryItemSearch(itemHelper,model);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMInventoryItemSearch::itemSelected,
                         this, &WorkOrderDelegate::commitAndCloseItemEditor);
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
                         this, &WorkOrderDelegate::commitAndCloseUOMEditor);
        return itemLine;
        break;
    }

    case 7:{
        QToolButton* button = new QToolButton(parent);
        button->setIcon(QIcon(":/icons/trash.ico"));
        button->setProperty("id",index.row());
        button->setEnabled(true);
        //        connect(button,&QToolButton::clicked,
        //                this,&WorkOrderDelegate::deleteButtonClicked);
        return button;
    }


    case 8:{
        QToolButton* button = new QToolButton(parent);
        button->setIcon(QIcon(":/icons/icons/action/svg/production/ic_print_24px.svg"));
        button->setProperty("id",index.row());
        button->setEnabled(true);
        connect(button,&QToolButton::clicked,
                this,&WorkOrderDelegate::printButtonClicked);
        return button;
    }
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void WorkOrderDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
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
    case 2 :{
        QString itemID = index.model()->data(index, Qt::UserRole).toString();
        UomDataModel uomObj =  uomHelper->getUomObjectByID(index.model()->data(index, Qt::UserRole+1).toString());


        GMUOMSearchWidget *uomLine
                = new GMUOMSearchWidget(
                    uomHelper,
                    uomHelper->getUomModelByItem(itemID));
        uomLine->setItem(uomObj);
        uomLine->setFrame(false);

        break;
    }

    case 7:{
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        break;
    }

    case 8:{
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/icons/action/svg/production/ic_print_24px.svg"));
        break;
    }
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void WorkOrderDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    switch (index.column()) {
    case 1:{
        GMInventoryItemSearch *itemLine = qobject_cast<GMInventoryItemSearch *>(editor);
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
    case 7:
    {
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));

        model->setData(index,QVariant::fromValue(index.row()));
    }

    case 8:
    {
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/icons/action/svg/production/ic_print_24px.svg"));

        model->setData(index,QVariant::fromValue(index.row()));
    }
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void WorkOrderDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

void WorkOrderDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    GMInventoryItemSearch *editor = qobject_cast<GMInventoryItemSearch *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}

void WorkOrderDelegate::commitAndCloseUOMEditor(QModelIndex index)
{

    GMUOMSearchWidget *editor = qobject_cast<GMUOMSearchWidget *>(sender());
    qDebug()<<"item Set as"<<editor->uom.UomName;
    emit commitData(editor);
    emit closeEditor(editor);
}

void WorkOrderDelegate::deleteButtonClicked()
{
    model->removeRow(sender()->property("id").toInt());
}

void WorkOrderDelegate::printButtonClicked()
{
    int row = sender()->property("id").toInt();
    emit printBarcode(row);
}


bool WorkOrderDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 7  )
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

            //            if( clickX > x && clickX < x + w )
            //                if( clickY > y && clickY < y + h )
            //                {
            model->removeRows(index.row(),1);
            // model()->data(index, Qt::UserRole+1) index.row();
            //                }
        }
        if(index.column() == 8 )
        {
            emit printBarcode(index.row());
        }
        return true;
    }
    else{
        QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}
