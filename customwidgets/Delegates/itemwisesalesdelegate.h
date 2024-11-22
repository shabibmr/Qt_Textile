#ifndef ITEMWISESALESDELEGATE_H
#define ITEMWISESALESDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>

class ItemwiseSalesDelegate : public QStyledItemDelegate
{
public:
    ItemwiseSalesDelegate();
protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItem opt = option;


        initStyleOption(&opt, index);


        //        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        //        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();
        //        QString line2 = index.model()->data(index.model()->index(index.row(), 0)).toString();

        int col = index.column();



        // draw correct background
        //        opt.text = "";

        if(col== 1 || col ==2){
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

#endif // ITEMWISESALESDELEGATE_H
