#ifndef GMGODOWNSEARCHLISTWIDGETLITE_H
#define GMGODOWNSEARCHLISTWIDGETLITE_H

#include <QWidget>
#include <QSqlQueryModel>
#include "database/inventory/godown/godowndatabasehelper.h"
#include "gmgodownsearchproxymodel.h"
#include "godownsearchdelegate.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QSqlRecord>

namespace Ui {
class GMGodownSearchListWidgetLite;
}

class GMGodownSearchListWidgetLite : public QWidget
{
    Q_OBJECT

public:
    explicit GMGodownSearchListWidgetLite(QWidget *parent = 0);
    ~GMGodownSearchListWidgetLite();

    GodownDatabaseHelper itemHelper;
    GMGodownSearchProxyModel *sort;


    QSqlQueryModel *model;

    QStringList itemNames;
    int lLoc =0;

    void setFocusExpl(QString text);


private:
    Ui::GMGodownSearchListWidgetLite *ui;

protected:
    bool eventFilter(QObject* obj, QEvent *event);

public slots:
    void filterData(QString);

    void setData(QSqlQueryModel* model);

    void checkData();

private slots:

    void onTablewidgetEnterpressed(QModelIndex i);

    void on_tableWidget_clicked(const QModelIndex &index);

signals:

    void selectedID(QString,QString);
    void nonSelected();

};

#endif // GMGODOWNSEARCHLISTWIDGETLITE_H
