#include "contactmeasurementeditor.h"
#include <QLayout>
#include "measurementseditorform.h"
#include <QMessageBox>
#include "counter/PrinterHelper/counterprinterhelper.h"
#include <QListWidget>
#include <QCompleter>

ContactMeasurementEditor::ContactMeasurementEditor(ContactMeasurement *measure,
                                                   MeasurementsDatabaseHelper *dbHelper ,
                                                   QWidget *parent)
    : QWidget{parent}
{
    this->dbHelper = dbHelper;
    this->measure = measure;
    initWidgets();
}


void ContactMeasurementEditor::initDBHelpers()
{

}

void ContactMeasurementEditor::initWidgets()
{
    /* 1. Reference Line
     * 2. Date
     * 4. Table
     *
     *
     *
     *
     * */

    QVBoxLayout *editorLay = new QVBoxLayout();
    editorLay->setContentsMargins(10,10,10,10);
    QSqlQueryModel *model = dbHelper->getAllMeasurements();
    qDebug()<<"Master Size : "<<model->rowCount()<<model->columnCount();
    measureMaster = new QComboBox(this);
    measureMaster->addItem("","");
    // QComboBox *combo = new QComboBox(this);
    for(int i=0;i<model->rowCount();i++){
        qDebug()<<"ADDING MMASTER : "<<model->record(i).value(1).toString();
        measureMaster->addItem(model->record(i).value(1).toString(),model->record(i).value(0).toString());
    }

    if(measure->measurement!=nullptr){
        measureMaster->setCurrentText(measure->measurement->measurementName);
    }

    formEditor = new MeasurementsEditorForm(measure,dbHelper,this);

    connect(measureMaster,static_cast<void (QComboBox::*)( int )>(&QComboBox::activated)
            ,this,[=](int index){
                qDebug()<<"Combo Updated to "<<index;
                if(index>0){
                    int row = index;
                    QString mId = model->record(row-1).value(0).toString();
                    measure->measurement = dbHelper->getMeasurementMasterByID(mId);
                    //refreshForm();
                    formEditor->setMeasurement(measure);
                }
            });

    connect(formEditor,&MeasurementsEditorForm::editted,this,&ContactMeasurementEditor::setEdited);


    referenceLine = new QLineEdit(this);
    referenceLine->setText(measure->reference);
    referenceLine->setPlaceholderText("Enter A Reference Here...");
    connect(referenceLine,&QLineEdit::textChanged,this,[=](QString text){
        measure->reference = text;
        setEdited(true);
    });


    editorLay->addWidget(measureMaster);
    editorLay->addWidget(referenceLine);
    editorLay->addWidget(formEditor);


    saveButton = new QtMaterialRaisedButton(this);
    saveButton ->setText("Select");
    connect(saveButton,&QPushButton::clicked,this,&ContactMeasurementEditor::saveObject);
    editorLay->addWidget(saveButton);

    this->setLayout(editorLay);
    qDebug()<<"EDITOR IS SET"<<__LINE__;

    printButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("action", "print"));
    printButton->setMini(true);
    printButton->setXOffset(10);
    printButton->setYOffset(10);
    printButton->setParent(this);
    printButton->setToolTip("Print");

    connect(printButton,&QPushButton::clicked,this,&ContactMeasurementEditor::printMeasurement);
}

void ContactMeasurementEditor::setWidgets()
{

}

void ContactMeasurementEditor::saveObject()
{
    if(measure->reference.length()==0){
        QMessageBox box;
        box.setText("Reference Cannot Be Empty!!!");
        box.exec();
        return;
    }

    if(measure->contactID == ""){
        QMessageBox box;
        box.setText("Contact is Not Set!!!");
        box.exec();
        return;
    }
    if(edited){

        if(measure->contactMeasureID==""){
            qDebug()<<"Inserting Measurement llen : "<<measure->lables.length();
            dbHelper->insertContactMeasurement(measure);
        }
        else{
            dbHelper->updateContactMeasurement(measure);
        }
    }

    setEdited(false);

    emit saved();

}

void ContactMeasurementEditor::deleteObject()
{
    dbHelper->deleteContactMeasurement(measure);
}

void ContactMeasurementEditor::printMeasurement()
{
    CounterPrinterHelper printer;
    AddressBookDatabaseHelper *adHelper;

    AddressBookDataModel contact = adHelper->getContactByAddressID(measure->contactID);
    printer.printMeasurementContact(measure,contact);
}

void ContactMeasurementEditor::setMeasure(ContactMeasurement *newMeasure)
{
    edited = false;
    measure = newMeasure;
    qDebug()<<"Editor lbl len : "<<measure->lables.length()<<Q_FUNC_INFO<<__LINE__;
    referenceLine->blockSignals(true);
    referenceLine->setText(measure->reference);
    referenceLine->blockSignals(false);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    if(measure->measurement!=nullptr){
        measureMaster->setCurrentText(measure->measurement->measurementName);
        qDebug()<<"Current text Set with lables : "<<measure->lables.length();
        formEditor->setMeasurement(measure);
    }
    else{
        qDebug()<<"Measure Master Is NULL";
        measureMaster->setCurrentIndex(0);
    }
}

void ContactMeasurementEditor::setEdited(bool newEdited)
{
    edited = newEdited;
    if(edited){
        saveButton->setText("Save & Select");
    }
    else{
        saveButton->setText("Select");
    }
}
