#ifndef GMEMPLOYEESEARCHLISTWIDGETLITE_H
#define GMEMPLOYEESEARCHLISTWIDGETLITE_H

#include <QWidget>
#include <QSqlQueryModel>

#include "database/hr/userprofiledatabasehelper.h"

#include "gmemployeesearchproxymodel.h"
#include "employeesearchdelegate.h"

#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QSqlRecord>

namespace Ui {
class GMEmployeeSearchListWidgetLite;
}

class GMEmployeeSearchListWidgetLite : public QWidget
{
    Q_OBJECT

public:
    explicit GMEmployeeSearchListWidgetLite(QSqlQueryModel *model,QWidget *parent = nullptr);
    ~GMEmployeeSearchListWidgetLite();

    UserProfileDatabaseHelper empHelper;
    GMEmployeeSearchProxyModel *sort;


    QSqlQueryModel *model;

    QStringList itemNames;
    int lLoc =0;

    void setFocusExpl(QString text);
private:
    Ui::GMEmployeeSearchListWidgetLite *ui;
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

#endif // GMEMPLOYEESEARCHLISTWIDGETLITE_H
