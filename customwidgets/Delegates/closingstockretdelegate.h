#ifndef CLOSINGSTOCKRETDELEGATE_H
#define CLOSINGSTOCKRETDELEGATE_H

#include <QStyledItemDelegate>
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QDate>

class ClosingStockRetDelegate : public QStyledItemDelegate
{
public:
    ClosingStockRetDelegate(bool showStockVarValues);
    bool showStockVariance;


protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option2, const QModelIndex& index) const
    {
        QStyleOptionViewItem opt = option2;
        initStyleOption(&opt, index);


        //        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        //        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();
        //        QString line2 = index.model()->data(index.model()->index(index.row(), 0)).toString();
        //        int col = index.column();

        QVariant colType = index.model()->data(index).type();
        int col = index.column();
        //        int row = index.row();
        if(showStockVariance){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<col;
            float invUsage = index.model()->data(index.model()->index(index.row(), 5)).toFloat();
            float stkAdj = index.model()->data(index.model()->index(index.row(), 6)).toFloat(); //stock adjustment = actual stock - stock by software
            float salesVal = invUsage - stkAdj;
//            float invUsage = ( index.model()->data(index.model()->index(index.row(), 3)).toFloat() + //opStk
//                               index.model()->data(index.model()->index(index.row(), 4)).toFloat()) //inward
//                    - index.model()->data(index.model()->index(index.row(), 7)).toFloat() ;  // clsStock
            float variance = stkAdj; //salesVal - invUsage;
            float lpp = index.model()->data(index.model()->index(index.row(), 9)).toFloat();
            float variancePerc = variance / (salesVal ); // not multiplying by lpp
            if(col == 10){ //vairance
                            qDebug()<<Q_FUNC_INFO<<__LINE__<<variance*lpp<<variance<<lpp;
                opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                opt.text = QString::number(variance * lpp,'f',2);

                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
                return;
            }
            else if(col == 11){ // variance %
                opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                opt.text = QString::number(variancePerc,'f',2);

                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
                return;
            }
            else if(col == 12){ // pour cost
                opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                opt.text = QString::number(invUsage / salesVal,'f',2);

                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
                return;
            }
            else if(col == 13){ //parlevel
                opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
                opt.text = QString::number(salesVal / 30,'f',2);

                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
                return;
            }
        }
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
        else if(opt.text.endsWith("Dr") || opt.text.endsWith("Cr")){
            opt.displayAlignment = Qt::AlignRight|Qt::AlignVCenter;
        }
        else if (index.model()->headerData(index.column(),Qt::Horizontal).toString() == "Status" ) {
            opt.text = QuotationStatus::getStatusString(opt.text.toInt());

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


};



#endif // CLOSINGSTOCKRETDELEGATE_H

