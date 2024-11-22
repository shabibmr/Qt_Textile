#ifndef HSNREPORTDELEGATE_H
#define HSNREPORTDELEGATE_H

#include <QApplication>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QJsonObject>
#include <QDebug>

class HsnReportDelegate : public QStyledItemDelegate
{
public:
    HsnReportDelegate();
    QJsonObject hsnList;

public:

    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option2, const QModelIndex &index) const
    {
        QStyleOptionViewItem opt = option2;
        initStyleOption(&opt, index);

        int col = index.column();
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

        if(index.column() == 1)
        {
                int row = index.row();

                QModelIndex ind1 = index.model()->index(row,0); // index of HSN

//                qDebug()<<Q_FUNC_INFO<<__LINE__<<ind1;
                QString HSN =  index.model()->data(ind1).toString() ;
                QString desc = hsnList[HSN].toString();
//                qDebug()<<Q_FUNC_INFO<<__LINE__<<desc;
                opt.text = desc;
                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

                return;
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


        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);


    }

    // QStyledItemDelegate interface


};

#endif // HSNREPORTDELEGATE_H
