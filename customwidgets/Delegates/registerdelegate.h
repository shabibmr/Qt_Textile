#ifndef REGISTERDELEGATE_H
#define REGISTERDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QDate>

#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

class RegisterDelegate : public QStyledItemDelegate
{
public:

    RegisterDelegate(int vNoPos);
    int vNoPos = 5;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option2, const QModelIndex& index) const
    {
        QStyleOptionViewItem opt = option2;
        initStyleOption(&opt, index);

        int col = index.column();
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

        if(index.column() == vNoPos) // col of vno
        {
            qDebug()<<"adding voucher prefix to"<<opt.text;
            bool prefixFlag = ConfigurationSettingsDatabaseHelper::getValue(showPrefixInRegister, false).toBool();
            if(prefixFlag){
                int row = index.row();

                QModelIndex ind1 = index.model()->index(row,2); // index of vprefix
//                qDebug()<<Q_FUNC_INFO<<__LINE__<<ind1;
                QString vNo =  index.model()->data(ind1).toString() + "-" + index.model()->data(index).toString();
                opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                opt.text = vNo;
                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

                return;
            }
        }

        //Check data Type and do alignment

        if(index.model()->data(index).type() == QVariant::Double){
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            opt.text = QString::number(opt.text.toDouble(),'f',2);
            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
            return;
        }

        else  if(colType == QVariant::Int||colType == QVariant::LongLong||
                 colType == QVariant::UInt||colType == QVariant::ULongLong )
        {
            //            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
            //                              Qt::AlignRight|Qt::AlignVCenter, opt.text);
            opt.rect.setWidth(opt.rect.width()-3);
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            //            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        }
        else if (index.model()->data(index).type()  == QVariant::Date){
            opt.text = index.model()->data(index).toDate().toString("dd-MM-yyyy");
        }

        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

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


    
    // QAbstractItemDelegate interface

};



#endif // REGISTERDELEGATE_H
