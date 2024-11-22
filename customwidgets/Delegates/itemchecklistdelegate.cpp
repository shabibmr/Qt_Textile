#include "itemchecklistdelegate.h"
#include <QDebug>
#include <QApplication>
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"

ItemCheckListDelegate::ItemCheckListDelegate()
{

}

void ItemCheckListDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QRect rect = option.rect;
    if(index.column()==11){
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
            button.icon= QtMaterialTheme::icon("navigation", "check");
            button.iconSize = QSize(20,20);
            button.rect = QRect(x,y,w,h);
            button.text = "";//no text . since if text will be given then it will push the icon to left side based on the coordinates .
            button.state = QStyle::State_Enabled;
            //QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter);
            QApplication::style()->drawControl( QStyle::CE_PushButtonLabel, &button, painter);//To make the Button transparent .
        }
    else{
        QPalette::ColorGroup cg = option.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;

        if (option.state & QStyle::State_Selected)
        {
            painter->setPen(QPen(QColor(242,139,47),4,Qt::SolidLine,Qt::RoundCap));
            painter->drawRect(rect);
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));

        }
        else
        {
            painter->setPen(option.palette.color(cg, QPalette::Text));
        }


        painter->drawText(QRect(rect.left()+3, rect.top(), rect.width()-10, rect.height()),
                          index.model()->data(index, Qt::TextAlignmentRole).toInt(),index.model()->data(index, Qt::DisplayRole).toString());

    }

}

void ItemCheckListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    qDebug()<<Q_FUNC_INFO;

    QStyledItemDelegate::setModelData(editor, model, index);

}
