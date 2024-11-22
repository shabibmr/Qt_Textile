#ifndef CONTACTMEASUREMENTREFERENCES_H
#define CONTACTMEASUREMENTREFERENCES_H

#include <QWidget>
#include <QListView>

#include <database/inventory/measurements/measurementsdatabasehelper.h>

class ContactMeasurementReferences : public QWidget
{
    Q_OBJECT
public:
    explicit ContactMeasurementReferences(
        ContactMeasurement *measurement,
        MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper(),QWidget *parent = nullptr);

    QSqlQueryModel *model;
    ContactMeasurement *measurement;
    MeasurementsDatabaseHelper *dbHelper;
    QListView *list;


public slots:
    void refSelected(QModelIndex index);
    void setData();
signals:

    void referenceSelected(QString id);
};

#endif // CONTACTMEASUREMENTREFERENCES_H
