#ifndef SALESORDERSTIMEBASEDREPORTDELEGATE_H
#define SALESORDERSTIMEBASEDREPORTDELEGATE_H

#include <QStyledItemDelegate>

#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QDate>

class SalesOrdersTimeBasedReportDelegate : public QStyledItemDelegate
{
public:
    SalesOrdersTimeBasedReportDelegate();

    void paint(QPainter* painter, const QStyleOptionViewItem& option2, const QModelIndex& index) const
    {
        QStyleOptionViewItem opt = option2;
        initStyleOption(&opt, index);


        //        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        //        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();
        //        QString line2 = index.model()->data(index.model()->index(index.row(), 0)).toString();
        //        int col = index.column();

        QVariant colType = index.model()->data(index).type();
        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected){
            //            painter->fillRect(rect,QBrush(QColor(127,127,127)));
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        }
        else{
            painter->setPen(opt.palette.color(cg, QPalette::Text));
        }

        if(index.model()->data(index).type() == QVariant::Double){
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            if(opt.text.toDouble()!=0)
                opt.text = QString::number(opt.text.toDouble(),'f',2);
            else
                opt.text="";
            //            painter->fillRect(opt.rect,opt.backgroundBrush);
            //            painter->drawText(QRect(rect.left(), rect.top(), rect.width()-3, rect.height()),
            //                              Qt::AlignRight|Qt::AlignVCenter,QString::number(opt.text.toFloat(),'f',2));

            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
            return;
        }

        else  if(colType == QVariant::Int||colType == QVariant::LongLong||
                 colType == QVariant::UInt||colType == QVariant::ULongLong )
        {
            //            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
            //                              Qt::AlignRight|Qt::AlignVCenter, opt.text);
            if(opt.text.toInt()==0)
                opt.text="";
            opt.rect.setWidth(opt.rect.width()-3);
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            //            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        }
        else if (index.model()->data(index).type()  == QVariant::Date){
            opt.text = index.model()->data(index).toDate().toString("dd-MM-yyyy");
        }
        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    }
};

#endif // SALESORDERSTIMEBASEDREPORTDELEGATE_H
