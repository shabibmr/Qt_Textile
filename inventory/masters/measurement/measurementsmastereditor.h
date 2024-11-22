#ifndef MEASUREMENTSMASTEREDITOR_H
#define MEASUREMENTSMASTEREDITOR_H

#include <QWidget>
#include "database/inventory/measurements/measurementsdatabasehelper.h"
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QListView>
#include <QStandardItemModel>

class MeasurementsMasterEditor : public QWidget
{
    Q_OBJECT
public:
    explicit MeasurementsMasterEditor(MeasurementMaster *measure = new MeasurementMaster(),
                                      MeasurementsDatabaseHelper *dbHelper=new MeasurementsDatabaseHelper(),
                                      QWidget *parent = nullptr);


    void setWidgets();

    QListView *list;
    QStandardItemModel *model ;

public slots:

    void saveMeasure();
    void deleteMeasure();

    void refreshList();
private:
    MeasurementsDatabaseHelper *dbHelper;
    MeasurementMaster *measure;
signals:
    void closing();
    void saved();
};

#endif // MEASUREMENTSMASTEREDITOR_H
