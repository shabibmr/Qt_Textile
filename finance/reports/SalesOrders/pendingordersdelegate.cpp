#include "pendingordersdelegate.h"


#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include <QApplication>
#include <QMouseEvent>
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QBitmap>

void PendingOrdersDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int bal = QDateTime::currentDateTime().secsTo(index.model()->data(index,Qt::UserRole+4).toDateTime());
    qDebug()<<"Bal : "<<bal<<index.model()->data(index,Qt::UserRole+4).toDateTime()<<index.row();
    if(bal<0){
          painter->fillRect(option.rect, Qt::lightGray);
    }
    else if(bal<30*60){
          painter->fillRect(option.rect, QColor().fromRgb(200,120,75,144));
    }
    else {

    }
    if(index.column() == 12){
        //        qDebug()<<"Paint col 11";
        QStyleOptionButton button;
        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 20;//the X coordinate
        x = r.left()+ (r.width() -20)/2;
        y = r.top();//the Y coordinate
        y = r.top()+ (r.height() -20)/2;
        w = 15;//button width(based on the requirement)
        h = 15;//button height(based on the requirement)
        button.icon= QtMaterialTheme::icon("action", "print");
        bool status = index.model()->data(index,Qt::UserRole+3).toBool();
        if(status){
            QPixmap pixmap = QPixmap(":/icons/icons/action/svg/production/ic_print_24px.svg");
            QBitmap mask  = pixmap.createMaskFromColor(QColor("transparent"), Qt::MaskInColor);
            pixmap.fill((QColor("#1976d2")));
            pixmap.setMask(mask);
            button.icon = QIcon(pixmap);
        }


        button.iconSize = QSize(20,20);
        button.rect = QRect(x,y,w,h);
        button.text = "";//no text . since if text will be given then it will push the icon to left side based on the coordinates .
        if (option.state & QStyle::State_Selected)
        {

        }
        button.state = QStyle::State_Enabled;
        //QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
        QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
    }
    if(index.column()==11){
        //        qDebug()<<"Paint col 11";
        QStyleOptionButton button;
        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 20;//the X coordinate
        x = r.left()+ (r.width() -20)/2;
        y = r.top();//the Y coordinate
        y = r.top()+ (r.height() -20)/2;
        w = 15;//button width(based on the requirement)
        h = 15;//button height(based on the requirement)
        int status = index.model()->data(index,Qt::UserRole+2).toInt();
        if(status == QuotationStatus::SalesInvoiceRaised)
            button.icon = QIcon(QString::fromUtf8(":/icons/check.ico"));
        else
            button.icon= QIcon(QString::fromUtf8(":/images/documentExport.ico"));//QtMaterialTheme::icon("av", "fast_forward");
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

bool PendingOrdersDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 11  )
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
            QString vno = index.model()->data(index, Qt::UserRole).toString();
            QString vpr = index.model()->data(index, Qt::UserRole+1).toString();
            //            if( clickX > x && clickX < x + w )
            //                if( clickY > y && clickY < y + h )
            //                {
            emit saveSalesVoucher(vno,vpr);
            // model()->data(index, Qt::UserRole+1) index.row();
            return true;
            //                }
        }
        else if(index.column() == 12  )
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
            QString vno = index.model()->data(index, Qt::UserRole).toString();
            QString vpr = index.model()->data(index, Qt::UserRole+1).toString();
            //                {
            emit printKot(vno,vpr);
            return true;
            // model()->data(index, Qt::UserRole+1) index.row();
            //                }
        }
        else{
            return QStyledItemDelegate::editorEvent(event,model,option,index);
        }


    }
    else{
        return QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}

void PendingOrdersDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
    emit commitData(editor);
    emit closeEditor(editor);
}

void PendingOrdersDelegate::commitAndCloseEmplEditor(QModelIndex index)
{

}


QWidget *PendingOrdersDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    if(index.column()==9){
        GMLedgerLineWidget *itemLine = new GMLedgerLineWidget(ledHelper,model);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMLedgerLineWidget::itemSelected,
                         this, &PendingOrdersDelegate::commitAndCloseItemEditor);
        return itemLine;
    }
    if(index.column()==3){
        GMEmployeeSearchLine *empLine = new GMEmployeeSearchLine(empHelper,empHelper->getEmployeeQueryForSearch());
        empLine->setFrame(false);
        empLine->setParent(parent);
        QObject::connect(empLine, &GMEmployeeSearchLine::itemSelected,
                         this, &PendingOrdersDelegate::commitAndCloseEmplEditor);
        return empLine;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void PendingOrdersDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column()==9){
        QString value;
        if(index.row() == index.model()->rowCount())
            value ="";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();
            //            qDebug()<<"valid String : "<<value;
        }

        GMLedgerLineWidget *itemLine = static_cast <GMLedgerLineWidget*>(editor);

        itemLine->setText(value);

        return;
    }
    if(index.column() == 3 ){

        QString value;
        if(index.row()==index.model()->rowCount())
            value = "";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();
        }
        GMEmployeeSearchLine *itemLine = static_cast <GMEmployeeSearchLine*>(editor);
        itemLine->setText(value);
        //        itemLine->setNewText(value,"");
        return;

    }
    QStyledItemDelegate::setEditorData(editor,index);
}

void PendingOrdersDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column()==9){
        GMLedgerLineWidget *itemLine = qobject_cast<GMLedgerLineWidget *>(editor);

        if(itemLine->item.LedgerID.length())
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;
    }
    if(index.column()==3){

        GMEmployeeSearchLine *itemLine = qobject_cast<GMEmployeeSearchLine *>(editor);
        if(itemLine->item._id > 0)
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;

    }
    QStyledItemDelegate::setModelData(editor,model,index);
}
