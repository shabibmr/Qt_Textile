#include "possalesitemsdelegate.h"
#include <QMouseEvent>
#include <QPainter>

PosSalesItemsDelegate::PosSalesItemsDelegate(int width,int height,QObject *parent)
    : QStyledItemDelegate(parent)
{

    this->width = width;
    this->height = height;
}

PosSalesItemsDelegate::~PosSalesItemsDelegate()
{

}

void PosSalesItemsDelegate::paint(QPainter *painter,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const
{

    QRect rect = option.rect;

    painter->setBrush(QBrush(QColor(224, 247, 250)));

    QRect srect = QRect(rect.left()+5,
                        rect.top()+5,
                        width * 1.2 - 5,
                        height * 1.3);



    QPen pen;
    QBrush br;

    QFont fnt = painter->font();

    fnt.setPointSize(11);
    painter->setFont(fnt);

    QRect rtext;

    painter->drawRoundedRect(srect,
                             10,10 );
    painter->drawText(srect,
                      Qt::AlignCenter|Qt::TextWordWrap,
                      index.model()->data(index, Qt::DisplayRole).toString(),
                      &rtext
                      );




//    QStyledItemDelegate::paint(painter,option,index);
}

bool PosSalesItemsDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(event->type() == QEvent::MouseButtonRelease){
        QMouseEvent * e = (QMouseEvent *)event;
        int clickX = e->x();
        int clickY = e->y();

        QString itemID = model->data(index, Qt::UserRole).toString();
        emit clicked(itemID);

    }

    return QStyledItemDelegate::editorEvent(event, model, option,  index);
}

void PosSalesItemsDelegate::updateEditorGeometry(QWidget *editor,
                                                 const QStyleOptionViewItem &option,
                                                 const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


QSize PosSalesItemsDelegate::sizeHint(const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const
{
    QSize result = QStyledItemDelegate::sizeHint(option, index);
    result.setHeight(result.height()*2);
    return QSize(width*1.3,height*1.5);
}

