#ifndef GMWIDGETSEARCHLISTWIDGETLITE_H
#define GMWIDGETSEARCHLISTWIDGETLITE_H

#include <QWidget>
#include <QSqlQueryModel>

#include "database/hr/userprofiledatabasehelper.h"

#include "gmwidgetsearchproxymodel.h"
#include "Widgetsearchdelegate.h"

#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QSqlRecord>

namespace Ui {
class GMWidgetSearchListWidgetLite;
}

class GMWidgetSearchListWidgetLite : public QWidget
{
    Q_OBJECT

public:
    explicit GMWidgetSearchListWidgetLite(QSqlQueryModel *model,QWidget *parent = nullptr);
    ~GMWidgetSearchListWidgetLite();

    UserProfileDatabaseHelper empHelper;
    GMWidgetSearchProxyModel *sort;


    QSqlQueryModel *model;

    QStringList itemNames;
    int lLoc =0;

    void setFocusExpl(QString text);
private:
    Ui::GMWidgetSearchListWidgetLite *ui;
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

    void selectedID(int,QString);

};

#endif // GMWIDGETSEARCHLISTWIDGETLITE_H
