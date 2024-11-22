#ifndef ITEMSEARCHDELEGATE_H
#define ITEMSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>


class ItemSearchDelegate : public QStyledItemDelegate
{
public:
    ItemSearchDelegate();

    QFont fontBold;
    QFont fontItalic;

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString line1 = index.model()->data(index.model()->index(index.row(), 3)).toString();
        QString line2 = index.model()->data(index.model()->index(index.row(), 6)).toString();
        QString line3 = index.model()->data(index.model()->index(index.row(), 2)).toString();

        int val = 2;
        if(line3.length()>0)
            val = 3;

        // draw correct background
        opt.text = "";
        QStyle *style = opt.widget ? opt.widget->style() :  opt.widget->style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected){
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
            QPen orangePen; orangePen.setColor(QColor(255,91,00));

            painter->setPen(orangePen);
            painter->setFont(fontBold);

        }
        else
            painter->setPen(opt.palette.color(cg, QPalette::Text));


        // draw 2 lines of text
        painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()/val),
                          opt.displayAlignment, line0);

        painter->drawText(QRect(rect.left()+rect.width()/2, rect.top()+rect.height()/val, rect.width()/2, rect.height()/val),
                          Qt::AlignRight, line2);

        painter->drawText(QRect(rect.left(), rect.top()+rect.height()/val, rect.width()/2, rect.height()/val),
                          opt.displayAlignment, line1);
        if(val ==3)
        {
            painter->setFont(fontItalic);
            painter->drawText(QRect(rect.left()+10, rect.top()+rect.height()/val*2, rect.width()/2, rect.height()/val),
                              opt.displayAlignment, line3);
        }
    }

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        int len = index.model()->data(index.model()->index(index.row(), 2)).toString().length();
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        int val = 2;
        if(len>0)
            val = 3;
        result.setHeight(result.height()*val);
        return result;
    }
};


#endif // ITEMSEARCHDELEGATE_H
