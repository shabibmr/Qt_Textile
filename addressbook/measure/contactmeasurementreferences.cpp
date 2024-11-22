#include "contactmeasurementreferences.h"

#include <QVBoxLayout>
#include <QSqlRecord>
ContactMeasurementReferences::ContactMeasurementReferences( ContactMeasurement *measurement,MeasurementsDatabaseHelper *dbHelper, QWidget *parent)
    : QWidget{parent}
{
    this->dbHelper = dbHelper;
    this->measurement = measurement;
    QVBoxLayout *layout = new QVBoxLayout();
    list = new QListView();
    QFont fnt;
    fnt.setPointSize(12);
    list->setFont(fnt);
    layout->addWidget(list);
    connect(list,QListView::clicked,this,&ContactMeasurementReferences::refSelected);
    this->setLayout(layout);
    setData();
}

void ContactMeasurementReferences::refSelected(QModelIndex index)
{
    QString id = model->record(index.row()).value(0).toString();
    emit referenceSelected(id);
}

void ContactMeasurementReferences::setData()
{
    model = dbHelper->getMeasurementReferencesByContact(measurement->contactID);
    qDebug()<<"At here";
    list->setModel(model);
    qDebug()<<"HEloo ros : "<<model->rowCount();
    list->setModelColumn(2);
}
