#ifndef BILLWISEREPORTDELEGATE_H
#define BILLWISEREPORTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class BillwiseReportDelegate : public QStyledItemDelegate
{
public:
    BillwiseReportDelegate();
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        bool ok;
        float fValue  = opt.text.toFloat(&ok);
        int col = index.column();

        // draw correct background
        //        opt.text = "";

        if(col==3){
            QRect rect = opt.rect;
            QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
            if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
                cg = QPalette::Inactive;

            // set pen color
            if (opt.state & QStyle::State_Selected)
                painter->setPen(opt.palette.color(cg, QPalette::Text));
            else
                painter->setPen(opt.palette.color(cg, QPalette::Text));

            // draw 2 lines of text
            if(opt.text.length()>0)
                painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
                                  Qt::AlignRight|Qt::AlignVCenter, QString::number(opt.text.toFloat(),'f',2));

        }
        else{
            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        }





        //        QRect rect = opt.rect;
        //        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        //        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        //            cg = QPalette::Inactive;

        //        // set pen color
        //        if (opt.state & QStyle::State_Selected)
        //            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        //        else
        //            painter->setPen(opt.palette.color(cg, QPalette::Text));

        //        // draw 2 lines of text
        //        painter->drawText(QRect(rect.left(), rect.top(), rect.width()/2, rect.height()/2),
        //                          opt.displayAlignment, line0);
        //        painter->drawText(QRect(rect.left()+rect.width()/2, rect.top(), rect.width()/2, rect.height()/2),
        //                          Qt::AlignRight, line2);

        //        painter->drawText(QRect(rect.left(), rect.top()+rect.height()/2, rect.width(), rect.height()/2),
        //                          opt.displayAlignment, line1);
    }


};



#endif // BILLWISEREPORTDELEGATE_H
