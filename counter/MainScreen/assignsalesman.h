#ifndef ASSIGNSALESMAN_H
#define ASSIGNSALESMAN_H

#include <QWidget>
#include <QSignalMapper>
#include <QScrollArea>
#include <QScrollBar>

#include "customwidgets/qtbutton.h"
#include "datamodels/hr/userprofiledatamodel.h"
#include "database/hr/userprofiledatabasehelper.h"
#include "database/inventory/salesorder/salesorderdatabasehelper.h"
#include "datamodels/transaction/generalvoucherdataobject.h"

namespace Ui {
class AssignSalesman;
}

class AssignSalesman : public QWidget
{
    Q_OBJECT

public:
    explicit AssignSalesman(GeneralVoucherDataObject *v,QWidget *parent = 0);
    ~AssignSalesman();

    UserProfileDatabaseHelper *dbHelper;
    SalesOrderDatabaseHelper *sHelper;
    QList<int> avlEmps;
    QString groupName;
    void setGroupName(const QString &value);

    QMap<QString,int> empsShort;

    void setTable();
    QMap<int,QString> emps;
    void keyPressEvent(QKeyEvent *e);
    GeneralVoucherDataObject *v;

public slots:
    void empSelected(int empID);
signals:
    void selectEmployeeID(int);
private slots:
    void on_toolButton_clicked();

private:
    Ui::AssignSalesman *ui;

};

#endif // ASSIGNSALESMAN_H
