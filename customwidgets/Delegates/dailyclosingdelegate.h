#ifndef DAILYCLOSINGDELEGATE_H
#define DAILYCLOSINGDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QDate>

class DailyClosingDelegate : public QStyledItemDelegate
{
public:
    DailyClosingDelegate();protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option2, const QModelIndex& index) const override
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

        //Check data Type and do alignment


        if(colType == QVariant::Int||colType == QVariant::LongLong||
                 colType == QVariant::UInt||colType == QVariant::ULongLong )
        {
            //            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
            //                              Qt::AlignRight|Qt::AlignVCenter, opt.text);
            opt.rect.setWidth(opt.rect.width()-3);
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            //            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        }
        else  if(index.model()->data(index).type() == QVariant::Double){
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
                        opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                        opt.text = QString::number(opt.text.toDouble(),'f',2);
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
                        //            painter->fillRect(opt.rect,opt.backgroundBrush);
                        //            painter->drawText(QRect(rect.left(), rect.top(), rect.width()-3, rect.height()),
                        //                              Qt::AlignRight|Qt::AlignVCenter,QString::number(opt.text.toFloat(),'f',2));

                        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
                        return;
                    }
        if(index.model()->data(index).type() == QVariant::Double){
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
            opt.text = QString::number(opt.text.toDouble(),'f',2);
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
            //            painter->fillRect(opt.rect,opt.backgroundBrush);
            //            painter->drawText(QRect(rect.left(), rect.top(), rect.width()-3, rect.height()),
            //                              Qt::AlignRight|Qt::AlignVCenter,QString::number(opt.text.toFloat(),'f',2));

            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
            return;
        }

        else if (index.model()->data(index).type()  == QVariant::Date){
            opt.text = index.model()->data(index).toDate().toString("dd-MM-yyyy");
        }

//        qDebug()<<Q_FUNC_INFO<<__LINE__<<"last column"<<index.row()<<index.column()<<index.model()->columnCount()-1;
//        if(index.column() == index.model()->columnCount() -1){
//            if(index.row() == 0|| index.row() == 1 || index.row() == 2 || index.row() == 17 ||
//                    index.row() == 18 || index.row() == 23)
//                opt.text = "";
////            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
//        }

        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

    }


};



#endif // DAILYCLOSINGDELEGATE_H
