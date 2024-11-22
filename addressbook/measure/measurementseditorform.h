#ifndef MEASUREMENTSEDITORFORM_H
#define MEASUREMENTSEDITORFORM_H

#include <QWidget>
#include "database/inventory/measurements/measurementsdatabasehelper.h"
#include <QLineEdit>
#include <QFormLayout>
class MeasurementsEditorForm : public QWidget
{
    Q_OBJECT
public:
    explicit MeasurementsEditorForm(ContactMeasurement *measurement = new ContactMeasurement,
                                    MeasurementsDatabaseHelper *dbHelper = new MeasurementsDatabaseHelper(),
                                    QWidget *parent = nullptr);
    ~MeasurementsEditorForm();

    void setMeasurement(ContactMeasurement *newMeasurement);

private :
    ContactMeasurement *measurement;
    MeasurementsDatabaseHelper *dbHelper;
    QFormLayout *form;
    QList<QLineEdit> valueLines;

    void setFormWidgets();

    bool edited=false;
public slots:
    void setText(QString text);

signals:
    void saveClicked();

    void editted(bool);

};

#endif // MEASUREMENTSEDITORFORM_H
