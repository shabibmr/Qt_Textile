#ifndef POSSALESITEMSDELEGATE_H
#define POSSALESITEMSDELEGATE_H

#include <QStyledItemDelegate>

class PosSalesItemsDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit PosSalesItemsDelegate(int width,int height,QObject *parent=nullptr);
    ~PosSalesItemsDelegate() override;

    int width;
    int height;
    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
signals:

    void clicked(QString itemID);


    // QAbstractItemDelegate interface
public:
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    // QAbstractItemDelegate interface
public:
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // POSSALESITEMSDELEGATE_H
