#ifndef ITEMCOMPLETER_H
#define ITEMCOMPLETER_H

#include <QObject>
#include <QCompleter>


class ItemCompleter: public QCompleter {
    Q_OBJECT
public:
    ItemCompleter(QObject *p = 0): QCompleter(p) {
        connect(this, SIGNAL(activated(QModelIndex)), SLOT(generateIndexSignal(QModelIndex)));
    }

    ItemCompleter(QAbstractItemModel *model, QObject *p = 0): QCompleter(model, p) {
        connect(this, SIGNAL(activated(QModelIndex)), SLOT(generateIndexSignal(QModelIndex)));
    }

    ItemCompleter(const QStringList& strings, QObject *p = 0): QCompleter(strings, p) {
        connect(this, SIGNAL(activated(QModelIndex)), SLOT(generateIndexSignal(QModelIndex)));
    }

signals:
    void selectedSourceRow(int index);

private slots:
    void generateIndexSignal(const QModelIndex& index)
    {
        QAbstractItemModel * const baseModel = model();
        QModelIndexList indexList = baseModel->match(
                    baseModel->index(0, completionColumn(), QModelIndex()),
                    completionRole(),
                    index.data(),
                    1,
                    Qt::MatchExactly);
        if (!indexList.isEmpty()) {
            emit selectedSourceRow(indexList.at(0).row());
        }
    }
};
#endif // ITEMCOMPLETER_H
