#ifndef INVENTORYITEM_H
#define INVENTORYITEM_H

#include <QWidget>
#include "additems.h"
#include <QKeyEvent>
#include <QToolButton>
#include "customwidgets/test/gmitemsortfilterproxymodel.h"

#include "customwidgets/Delegates/reportcommondelegate.h"

namespace Ui {
class inventoryItem;
}

class inventoryItem : public QWidget
{
    Q_OBJECT

public:
    explicit inventoryItem(QWidget *parent = 0);
    ~inventoryItem();
    QSqlQueryModel *sqlModel;
    GMItemSortFilterProxyModel* sort;

    QString x;
    AddItems *additemWidget;

    SalesInventoryItemDatabaseHelper *dbHelper;
    QList<QSharedPointer<inventoryItemDataModel >> itemsList;
    int getBackFlag = 0;

    void deleteitemAdd();

public slots:

    void searchTextChanged(QString);

    void setFocusonSearchLine();

    void setLineFocus();

private slots:

    void on_createNewPushButton_clicked();

    void keyPressEvent(QKeyEvent *e);

    void settable();

    void delButtonClicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_tableWidget_clicked(const QModelIndex &index);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_uploadPLU_clicked();

    void on_printPLU_clicked();



signals:
    void AddNewItemClicked();

    void SelectedItem(CompoundItemDataObject);



private:
    Ui::inventoryItem *ui;
};

#endif // INVENTORYITEM_H
