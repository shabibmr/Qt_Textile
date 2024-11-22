#ifndef CONTACTMEASUREMENTEDITOR_H
#define CONTACTMEASUREMENTEDITOR_H

#include <QWidget>
#include "database/inventory/measurements/measurementsdatabasehelper.h"
#include "customwidgets/SearchWidgets/Contact/gmcontactlinematerial.h"
#include "measurementseditorform.h"

#include "customwidgets/Material/components/qtmaterialraisedbutton.h"
#include "customwidgets/Material/components/qtmaterialfab.h"
#include <QComboBox>

class ContactMeasurementEditor : public QWidget
{
    Q_OBJECT
public:
    explicit ContactMeasurementEditor(ContactMeasurement *measure = new ContactMeasurement(),
                                      MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper(),
                                      QWidget *parent = nullptr);
    //~ContactMeasurementEditor() override;
    QLineEdit *referenceLine;
    QComboBox *measureMaster;

    MeasurementsEditorForm *formEditor;
    QtMaterialRaisedButton *saveButton;
    QtMaterialFloatingActionButton *printButton;

    void initDBHelpers();

    void initWidgets();

    void setWidgets();

    void setMeasure(ContactMeasurement *newMeasure);
    bool edited = false;


public slots:

    void setEdited(bool newEdited);

    void saveObject();
    void deleteObject();

    void printMeasurement();



private:
    ContactMeasurement *measure;
    MeasurementsDatabaseHelper *dbHelper;


signals:

    void saved();

};

#endif // CONTACTMEASUREMENTEDITOR_H
