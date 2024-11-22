#ifndef DAYBOOKREPORTDELEGATE_H
#define DAYBOOKREPORTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QDebug>

class DayBookReportDelegate : public QStyledItemDelegate
{
public:
    DayBookReportDelegate();
protected:

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);


        //        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        //        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();
        //        QString line2 = index.model()->data(index.model()->index(index.row(), 0)).toString();

        int col = index.column();

        bool ok;

        float fValue  = opt.text.toFloat(&ok);

        // draw correct background
        // opt.text = "";

        // qDebug()<<index.model()->data(index).type()<<col;

        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected){
            painter->fillRect(rect,QBrush(QColor(0,0,255)));
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        }
        else{
            painter->setPen(opt.palette.color(cg, QPalette::Text));
        }
        // draw 2 lines of text
        if(index.model()->data(index).type() == QVariant::Int||index.model()->data(index).type() == QVariant::LongLong||
                index.model()->data(index).type()== QVariant::UInt||index.model()->data(index).type()== QVariant::ULongLong )
        {
            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
                              Qt::AlignRight|Qt::AlignVCenter, opt.text);
        }
        else if(index.model()->data(index).type()== QVariant::Double){
            if( opt.text.toFloat()>0)
                painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
                                  Qt::AlignRight|Qt::AlignVCenter,QString::number(opt.text.toFloat(),'f',2));
        }
        else{
            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        }

    }
};

#endif // DAYBOOKREPORTDELEGATE_H
