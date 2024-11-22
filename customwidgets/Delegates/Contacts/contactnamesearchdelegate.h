
#ifndef CONTACTNAMESEARCHDELEGATE_H
#define CONTACTNAMESEARCHDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>


class ContactNameSearchDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ContactNameSearchDelegate(QObject *parent = nullptr);

    QFont fontBold;
    QFont fontItalic;
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        QStyleOptionViewItemV4 opt = option;
        initStyleOption(&opt, index);

        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();

        int val =2;

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


        painter->drawText(QRect(rect.left(), rect.top()+rect.height()/val, rect.width(), rect.height()/val),
                          opt.displayAlignment, line1);

    }

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const
    {
        QSize result = QStyledItemDelegate::sizeHint(option, index);
        result.setHeight(result.height()*2);
        return result;
    }
};

#endif // CONTACTNAMESEARCHDELEGATE_H
