#include "gmledgertransactiondelegate.h"
#include <QtGui>
#include <QApplication>
#include <QToolButton>

GMLedgerTransactionDelegate::~GMLedgerTransactionDelegate()
{

}

GMLedgerTransactionDelegate::GMLedgerTransactionDelegate(LedgerMasterDatabaseHelper *iHelper,
                                                         QSqlQueryModel *model)
{
    this->model = model;
    this->itemHelper = iHelper;
}

void GMLedgerTransactionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    if(index.column()==1 && index.row()== index.model()->rowCount()-1){
        //        qDebug()<<"Painting last elem";
        QFont italic;
        italic.setItalic(true);
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
    else if(index.column()==4){
        qDebug()<<"paint trash";
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
    else{

        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(QColor(242,139,47),4,Qt::SolidLine,Qt::RoundCap));
            painter->drawRect(rect);
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));

        }
        else
        {
            painter->setPen(option.palette.color(cg, QPalette::Text));
        }


        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-10, rect.height()),
                          index.model()->data(index, Qt::TextAlignmentRole).toInt(),index.model()->data(index, Qt::DisplayRole).toString());

    }
}

QWidget *GMLedgerTransactionDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    switch (index.column())
    {
    case 1 :
    {
        GMLedgerLineWidget *itemLine = new GMLedgerLineWidget(itemHelper,model);
        itemLine->setFrame(false);
        itemLine->setParent(parent);
        QObject::connect(itemLine, &GMLedgerLineWidget::itemSelected,
                         this, &GMLedgerTransactionDelegate::commitAndCloseItemEditor);
        return itemLine;
    }
    case 2:
    {
        break;

    }
    case 4:{

        QToolButton* button = new QToolButton(parent);
        button->setIcon(QIcon(":/icons/trash.ico"));
        button->setProperty("id",index.row());
        button->setEnabled(true);

        //        connect(button,&QToolButton::clicked,
        //                this,&GMInventoryTransactionDelegate::deleteButtonClicked);
        return button;
    }
    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void GMLedgerTransactionDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    //    qDebug()<<"Call Set EDITOR Data col : "<<index.column();
    switch (index.column()) {
    case 1:
    {
        QString value;
        if(index.row()==index.model()->rowCount())
            value ="";
        else{
            value = index.model()->data(index, Qt::DisplayRole).toString();
            //            qDebug()<<"valid String : "<<value;
        }
        //        qDebug()<<"..with value"<<value;
        GMLedgerLineWidget *itemLine = static_cast <GMLedgerLineWidget*>(editor);
        //        qDebug()<<"Current Text"<<itemLine->text();
        //        qDebug()<<"Current Value"<<value;
        itemLine->setText(value);
        //        itemLine->setNewText(value,"");

        break;
    }
    case 4:{
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        break;
    }
    }

    QStyledItemDelegate::setEditorData(editor, index);
}

void GMLedgerTransactionDelegate::deleteButtonClicked(){

    qDebug()<<"Clicked";
    qDebug()<<sender()->property("id") <<"count";
    model->removeRows(sender()->property("id").toInt(),1);

}

void GMLedgerTransactionDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    //    qDebug()<<".........................Set MOdel Data";
    switch (index.column()) {
    case 1:
    {
        GMLedgerLineWidget *itemLine = qobject_cast<GMLedgerLineWidget *>(editor);
        //        qDebug()<<"CHECK ME : "<<itemLine->item.LedgerName;
        //        qDebug()<<"Setting model data from delegate"<<itemLine->item.LedgerName;
        if(itemLine->item.LedgerID.length())
            model->setData(index, QVariant::fromValue(itemLine->item));
        return;
    }

    case 4:
    {
        QToolButton* button = qobject_cast<QToolButton*>(editor);
        button->setIcon(QIcon(":/icons/trash.ico"));
        model->setData(index,QVariant::fromValue(index.row()));
        return;
    }
    }
    QStyledItemDelegate::setModelData(editor, model, index);
}

void GMLedgerTransactionDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

bool GMLedgerTransactionDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 4  )
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
         return true;
    }
    else{
        QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}


void GMLedgerTransactionDelegate::commitAndCloseItemEditor(QModelIndex index)
{
    //    qDebug()<<".............................COMMITTING DATA...........................";
    GMLedgerLineWidget *editor = qobject_cast<GMLedgerLineWidget *>(sender());
    //    qDebug()<<"item Set as"<<editor->item.LedgerName;
    emit commitData(editor);
    emit closeEditor(editor);
}
