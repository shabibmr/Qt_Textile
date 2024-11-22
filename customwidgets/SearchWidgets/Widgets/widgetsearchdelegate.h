#ifndef WIDGETSEARCHDELEGATE_H
#define WIDGETSEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>


class WidgetSearchDelegate : public QStyledItemDelegate
{
public:
    WidgetSearchDelegate();

protected:
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString line1 = index.model()->data(index.model()->index(index.row(), 3)).toString();
        QString line2 = index.model()->data(index.model()->index(index.row(), 6)).toString();


        // draw correct background
        opt.text = "";
        QStyle *style = opt.widget ? opt.widget->style() :  opt.widget->style();
        style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);

        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected)
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        else
            painter->setPen(opt.palette.color(cg, QPalette::Text));

        // draw 2 lines of text
        painter->drawText(QRect(rect.left(), rect.top(), rect.width()/2, rect.height()/2),
                          opt.displayAlignment, line0);
        painter->drawText(QRect(rect.left()+rect.width()/2, rect.top(), rect.width()/2, rect.height()/2),
                          Qt::AlignRight, line2);

        painter->drawText(QRect(rect.left(), rect.top()+rect.height()/2, rect.width(), rect.height()/2),
                          opt.displayAlignment, line1);
    }

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        result.setHeight(result.height()*2);
        return result;
    }
};


#endif // WIDGETSEARCHDELEGATE_H
