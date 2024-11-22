#ifndef LEDGERREPORTDELEGATE_H
#define LEDGERREPORTDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include <QDate>
#include "datamodels/transaction/gmvouchertypes.h"

class LedgerReportDelegate : public QStyledItemDelegate
{
public:
    LedgerReportDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QString vType = index.model()->data(index.model()->index(index.row(), 3)).toString();


        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);
        int col = index.column();

        QString ledger = index.model()->data(index.model()->index(index.row(), 4)).toString();
        QString invoiceNo ="";
        if(index.model()->data(index.model()->index(index.row(), 11)).toString().length() > 0)
            invoiceNo = "  (" + index.model()->data(index.model()->index(index.row(), 11)).toString() + ") " ;

        QString invoiceDate = "";
        if(index.model()->data(index.model()->index(index.row(), 12)).toString().length() > 0)
            invoiceDate = " ("  +  index.model()->data(index.model()->index(index.row(), 12)).toString() + ")";
        QString narration = index.model()->data(index.model()->index(index.row(), 13)).toString();
        int val = 1;
        if(invoiceNo.length() > 0)
            val++;
        if(narration.length() > 0)
            val ++;


//        qDebug()<<Q_FUNC_INFO<<__LINE__<<val<<invoiceNo<<narration;

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

        if(index.model()->data(index).type() == QVariant::Double){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<opt.text;
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

        if(col ==5){
            opt.text = opt.text + invoiceDate;


        }
        if(col ==6){
            opt.text = opt.text + invoiceNo;
        }
        // below code for adding new lines in cel
//        else {
//            if(val > 1){

//                //                opt.rect = QRect(rect.left(), rect.top(), rect.width(), rect.height()/val);
//                opt.displayAlignment = Qt::AlignLeft|Qt::AlignTop;
//                //                opt.text = narration;
//                //                opt.palette.setColor(QPalette::Foreground,Qt::red);

//                //                QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
//                //                style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);


//                qDebug()<<Q_FUNC_INFO<<__LINE__<<rect.height();
//                if(col == 4){
//                    opt.text = "";
//                    QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
//                    style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
//                    painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()/val),
//                                      Qt::AlignLeft|Qt::AlignVCenter, ledger);

//                    if(invoiceNo.length() > 0){
//                        painter->drawText(QRect(rect.left(), rect.top()+rect.height()/val, rect.width(), rect.height()/val),
//                                          Qt::AlignLeft, invoiceNo);
//                        if(narration.length() > 0)
//                            painter->drawText(QRect(rect.left(), rect.top()+2 * rect.height()/val, rect.width(), rect.height()/val),
//                                              Qt::AlignLeft, narration);
//                    }
//                    else {
//                        if(narration.length() > 0)
//                            painter->drawText(QRect(rect.left(), rect.top()+ rect.height()/val, rect.width(), rect.height()/val),
//                                              Qt::AlignLeft, narration);
//                    }
//                    return;
//                }



//            }
//        }

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

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QString vType = index.model()->data(index.model()->index(index.row(), 3)).toString();
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        QString invoiceNo = index.model()->data(index.model()->index(index.row(), 11)).toString();
        QString narration = index.model()->data(index.model()->index(index.row(), 13)).toString();

        int val = 1;
        if(invoiceNo.length() > 0)
            val++;
        if(narration.length() > 0)
            val ++;
        result.setHeight(result.height()*val);
        qDebug()<<Q_FUNC_INFO<<result.height()<<index.row()<<val;
        return result;

    }
};

#endif // LEDGERREPORTDELEGATE_H
