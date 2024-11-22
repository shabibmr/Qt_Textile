#ifndef GMGODOWNSEARCHLITE_H
#define GMGODOWNSEARCHLITE_H


#include <QObject>
#include <QLineEdit>
#include "datamodels/inventory/godown/godowndataobject.h"
#include "database/inventory/godown/godowndatabasehelper.h"

#include "QSqlQueryModel"
#include "gmgodownsearchlistwidgetlite.h"

class GMGodownSearchLite: public QLineEdit
{
    Q_OBJECT
public:
    explicit GMGodownSearchLite(QSqlQueryModel *model,QLineEdit *parent=Q_NULLPTR);

    QStringList list;
    GodownDatabaseHelper *itemsHelper;

    QSqlQueryModel* itemsModel;

    GMGodownSearchListWidgetLite *listWidget;

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void itemSelected(QString,QString);
    void noneSelected();

signals:
    void SelectedGodown(GodownDataObject );
    void clearItem();

};

#endif // GMGODOWNSEARCHLITE_H
