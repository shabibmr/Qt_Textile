#ifndef GMBANKRECONCILIATIONDELEGATE_H
#define GMBANKRECONCILIATIONDELEGATE_H

#include <QStyledItemDelegate>
#include "customwidgets/SearchWidgets/Inventoryitem/Completer/gminventoryitemsearch.h"
#include "QVBoxLayout"
#include <QMessageBox>
#include "customwidgets/SearchWidgets/Uom/uomsearchwidget.h"
#include <QToolButton>
#include <QApplication>

class GMBankReconciliationDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit GMBankReconciliationDelegate(QSqlQueryModel *model);
    virtual ~GMBankReconciliationDelegate() override;


    // QAbstractItemDelegate interface
public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
    {
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);


        //        QString line0 = index.model()->data(index.model()->index(index.row(), 1)).toString();
        //        QString line1 = index.model()->data(index.model()->index(index.row(), 2)).toString();
        //        QString line2 = index.model()->data(index.model()->index(index.row(), 0)).toString();

        int col = index.column();

        bool ok;

        float fValue  = opt.text.toFloat(&ok);

        // draw correct background
        // opt.text = "";

        // qDebug()<<index.model()->data(index).type()<<col;

        QRect rect = opt.rect;
        QPalette::ColorGroup cg = opt.state & QStyle::State_Enabled ? QPalette::Normal : QPalette::Disabled;
        if (cg == QPalette::Normal && !(opt.state & QStyle::State_Active))
            cg = QPalette::Inactive;

        // set pen color
        if (opt.state & QStyle::State_Selected){
            painter->fillRect(rect,QBrush(QColor(0,0,255)));
            painter->setPen(opt.palette.color(cg, QPalette::HighlightedText));
        }
        else{
            painter->setPen(opt.palette.color(cg, QPalette::Text));
        }
        // draw 2 lines of text
        if(index.model()->data(index).type() == QVariant::Int||index.model()->data(index).type() == QVariant::LongLong||
                index.model()->data(index).type()== QVariant::UInt||index.model()->data(index).type()== QVariant::ULongLong )
        {
            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
                              Qt::AlignRight|Qt::AlignVCenter, opt.text);
        }
        else if(index.model()->data(index).type()== QVariant::Double){
            painter->drawText(QRect(rect.left(), rect.top(), rect.width(), rect.height()),
                              Qt::AlignRight|Qt::AlignVCenter,QString::number(opt.text.toFloat(),'f',2));
        }
        else{
            QStyle *style = opt.widget ? opt.widget->style() :  QApplication::style();
            style->drawControl(QStyle::CE_ItemViewItem, &opt, painter, opt.widget);
        }


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

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

signals:
    void showItemDetailClicked(int);

private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);
    void commitAndCloseUOMEditor(QModelIndex index);


private slots:



};

#endif // GMBANKRECONCILIATIONDELEGATE_H
