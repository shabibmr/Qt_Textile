#ifndef CONTACTMEASUREMENTPAGE_H
#define CONTACTMEASUREMENTPAGE_H
#include <QLayout>

#include <QWidget>
#include "contactmeasurementeditor.h"
#include "contactmeasurementreferences.h"
#include <QListView>

class ContactMeasurementPage : public QWidget
{
    Q_OBJECT
public:
    explicit ContactMeasurementPage(ContactMeasurement *measurement = new ContactMeasurement(),
                                    MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper(),
                                    QWidget *parent = nullptr);


    void setWidgets();

    void setMeasurement(ContactMeasurement *newMeasurement);


    QSqlQueryModel *model;
    QVBoxLayout *editorLay;
    ContactMeasurementEditor *e;

    void setListData();
    ContactMeasurementReferences *list;


public slots:

    void setNewMeasurement();

    void openMesurementEditor(QString id);

    void resetMeasurementEditor();

    void setFormWidget();

    void clearEditor();

    void saved();

private:
    ContactMeasurement *measurement;
    MeasurementsDatabaseHelper *dbHelper;
signals:

    void measurementSelected(ContactMeasurement *measurement);
};

#endif // CONTACTMEASUREMENTPAGE_H
