#include "kotreportdelegate.h"


#include "customwidgets/Material/components/lib/qtmaterialtheme.h"
#include <QApplication>
#include <QMouseEvent>
#include "datamodels/projects/Quotation/quotationstatus.h"
#include <QBitmap>

void KOTReportDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 4){
        QStyleOptionViewItem opt = option;


        initStyleOption(&opt, index);

        QRect rect = option.rect;
        QRect newRect = QRect(rect.left()+3, rect.top()+3, rect.width()-5, rect.height()-5);

        //            QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        //            if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
        //                cg = QPalette::Active;

        //            // set pen color
        //            if (opt.state & QStyle::State_Selected)
        //                painter->setPen(opt.palette.color(cg, QPalette::Text));
        //            else
        //                painter->setPen(opt.palette.color(cg, QPalette::Text));

        // draw 2 lines of text
        //        qDebug()<<"Mod: "<<model->data(index).toString().toInt();
        int d = model->data(index).toString().toInt();
        //        qDebug()<<"Stat :"<<QuotationStatus::getStatusString(d);
        //        if(option.text.length()>0)
        QTextOption op;
        op.setWrapMode(QTextOption::WordWrap);
        op.setAlignment(Qt::AlignRight|Qt::AlignVCenter);

        opt.text = QuotationStatus::getStatusString(d);

        QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
//        painter->drawText(QRect(rect.left(), rect.top(), rect.width()-5, rect.height()),
//                          /*Qt::AlignRight|Qt::AlignVCenter,*/ QuotationStatus::getStatusString(d),op);

    }
    else if(index.column()==5){
        //        qDebug()<<"Paint col 11";
        QStyleOptionButton button;
        QRect r = option.rect;//getting the rect of the cell
        int x,y,w,h;
        x = r.left() + r.width() - 20;//the X coordinate
        x = r.left()+ (r.width() -20)/2;
        y = r.top();//the Y coordinate
        y = r.top()+ (r.height() -20)/2;
        w = 15;//button width(based on the requirement)
        h = 15;//button height(based on the requirement)
        button.icon= QtMaterialTheme::icon("navigation", "arrow_forward");

        button.iconSize = QSize(20,20);
        button.rect = QRect(x,y,w,h);
        button.text = "";//no text . since if text will be given then it will push the icon to left side based on the coordinates .
        if (option.state & QStyle::State_Selected)
        {

        }
        button.state = QStyle::State_Enabled;
        //QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
        QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
    }

    else
        QStyledItemDelegate::paint(painter,option,index);
}

bool KOTReportDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonPress){
        if(index.column() == 5  )
        {
            //                {
            emit updateStatus(index.row());
            // model()->data(index, Qt::UserRole+1) index.row();
            return true;
            //                }
        }

        else{
            return QStyledItemDelegate::editorEvent(event,model,option,index);
        }


    }
    else{
        return QStyledItemDelegate::editorEvent(event,model,option,index);
    }
}
