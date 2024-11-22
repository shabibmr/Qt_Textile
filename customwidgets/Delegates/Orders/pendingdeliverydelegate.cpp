#include "pendingdeliverydelegate.h"
#include <QTime>
#include <QApplication>
#include <QDebug>
PendingDeliveryDelegate::PendingDeliveryDelegate()
{

}


void PendingDeliveryDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    int col = index.column();

    // int row = index.row();

    QString time1 = index.model()->data( index.model()->index(index.row(), 12)).toString();
    QDate  dd =  index.model()->data( index.model()->index(index.row(), 0)).toDate();
    QString date1 = dd.toString("dd-MM");
    QDateTime cur = QDateTime::fromString(date1+time1,"yyyy-MM-dd hh:mm:ss");

    if(cur.secsTo(QDateTime::currentDateTime())>(45*60)){
        opt.backgroundBrush = QBrush(QColor(140,15,1,25));

    }
    else if(cur.secsTo(QDateTime::currentDateTime())>(30*60)){
        opt.backgroundBrush = QBrush(QColor(80,20,21,15));

    }


    if(col == 0){
        // qDebug()<<"Col 0 "<<opt.text;
        QString time = QDate::fromString(opt.text,"dd-MM-yyyy").toString("dd-MM").toUpper();
        opt.text = date1;
        opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        return;
    }
    else
        if(col == 10){
        // qDebug()<<opt.text;
        QString amount = QString::number(opt.text.toFloat(),'f',2);
        opt.text = amount;
        opt.rect.setWidth(opt.rect.width()-3);
        opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        return;
    }
     else if(col ==12 || col==13){
        // qDebug()<<opt.text;
        QString time = QTime::fromString(opt.text," hh:mm:ss").toString("hh:mm ap").toUpper();
        opt.text = time;


    }
    QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
    return;

    // else
    //     QStyledItemDelegate::paint(painter,option,index);
}
