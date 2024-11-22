#include "measurementseditorform.h"


MeasurementsEditorForm::MeasurementsEditorForm(ContactMeasurement *measurement,
                                               MeasurementsDatabaseHelper *dbHelper,
                                               QWidget *parent)
    : QWidget{parent}
{
    this->dbHelper=dbHelper;
    this->measurement =measurement;
    form = new QFormLayout();
    this->setLayout(form);
    qDebug()<<__LINE__;
    setFormWidgets();
    qDebug()<<"FORM CONST";

}

MeasurementsEditorForm::~MeasurementsEditorForm()
{

}

void MeasurementsEditorForm::setMeasurement(ContactMeasurement *newMeasurement)
{
    measurement = newMeasurement;
    edited = false;
    emit editted(edited);
    setFormWidgets();
}

void MeasurementsEditorForm::setFormWidgets()
{
    // qDebug()<<"Setting lables - Master "<< measurement->measurement->measurementName <<" lbls ln : "<<measurement->measurement->lables.length();
    if (!form) return;

    // Loop through all items in the layout in reverse order
    while (form->count() > 0) {
        // Get the item at form 0
        QLayoutItem *item = form->takeAt(0);

        // If the item has a widget, delete it
        if (item->widget()) {
            delete item->widget();
        }

        // Delete the layout item itself
        delete item;
    }

    qDebug()<<"FORM CLEARED";
    if(measurement->measurement == nullptr)
        return;
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<measurement->valueMap.keys().length();
    qDebug()<<"LOOP"<< "NOT NULL : "<<measurement->measurement->measurementName<<measurement->measurement->lables.length();
    for(int i=0;i<measurement->measurement->lables.length();i++){
        QLineEdit *line = new QLineEdit(this);
        line->setMinimumSize(80,40);
        QString lableID = measurement->measurement->lables[i].labelID;
        line->setProperty("key",lableID);
        line->setProperty("name",measurement->measurement->lables[i].labelName);

        if(measurement->valueMap.keys().contains(lableID)){
            line->setText(measurement->valueMap[lableID].toString());
        }
        form->addRow(measurement->measurement->lables[i].labelName,line);
        connect(line,&QLineEdit::textChanged,this,&MeasurementsEditorForm::setText);
    }

}

void MeasurementsEditorForm::setText(QString text)
{
    edited = true;
    emit editted(edited);
    qDebug()<<"New Text at "<<text;
    QString key = sender()->property("key").toString();
    qDebug()<<"Key : "<<key;
    measurement->valueMap[key] = text;
    bool flag = false;
    qDebug()<<"lables :"<<measurement->lables.length();
    for(int i=0;i<measurement->lables.length();i++){
        qDebug()<<measurement->lables[i].lable.labelID;
        if(measurement->lables[i].lable.labelID == key){
            qDebug()<<"Setting value  to :"<<i<<text;
            measurement->lables[i].value = text;
            flag = true;
            break;
        }
    }
    if(flag == false){
        ContactMeasurementLable lbl;
        lbl.lable.labelID = key;
        lbl.value = text;
        measurement->lables.append(lbl);
    }

}
