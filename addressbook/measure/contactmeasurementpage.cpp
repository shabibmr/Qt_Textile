#include "contactmeasurementpage.h"
#include <QStandardItemModel>
#include <QSqlRecord>

ContactMeasurementPage::ContactMeasurementPage(ContactMeasurement *measurement,
                                               MeasurementsDatabaseHelper *dbHelper,QWidget *parent)
    : QWidget{parent}
{
    this->dbHelper = dbHelper;
    this->measurement = measurement;

    setWidgets();

    qDebug()<<"Pages Cons Completed";
}

void ContactMeasurementPage::setListData()
{
    list->setData();

    // model = dbHelper->getMeasurementReferencesByContact(measurement->contact);
    // qDebug()<<"At here";
    // list->setModel(model);
    // qDebug()<<"HEloo ros : "<<model->rowCount();
    // list->setModelColumn(2);
}

void ContactMeasurementPage::setNewMeasurement()
{
    QString contactID = measurement->contactID;
    measurement = new ContactMeasurement();
    measurement->contactID = contactID;
    measurement->measurement = nullptr;
    measurement->reference = "";
    measurement->contactMeasureID = "";
    measurement->lables.clear();
    measurement->valueMap.clear();
    setFormWidget();
}

void ContactMeasurementPage::setWidgets()
{
    qDebug()<<"Setting Page";
    QHBoxLayout *pageLayout = new QHBoxLayout();
    pageLayout->setContentsMargins(10,10,10,10);

    QWidget* refWidget = new QWidget(this);
    QVBoxLayout *refLay = new QVBoxLayout();
    refWidget->setLayout(refLay);


    QWidget* editorWidget = new QWidget(this);
    editorLay = new QVBoxLayout();
    editorLay->setContentsMargins(10,10,10,10);
    editorWidget->setLayout(editorLay);

    pageLayout->addWidget(refWidget);
    pageLayout->addWidget(editorWidget);

    pageLayout->setStretch(0,1);
    pageLayout->setStretch(1,4);

    this->setLayout(pageLayout);

    // ContactMeasurementEditor *e = new ContactMeasurementEditor(measurement,dbHelper,this);
    // editorLay->addWidget(e);
    // e->hide();

    list = new ContactMeasurementReferences ( measurement, dbHelper);


    connect(list,&ContactMeasurementReferences::referenceSelected,this,&ContactMeasurementPage::openMesurementEditor);

    QPushButton *newButton = new QPushButton(this);
    newButton->setText("Add Measurement");
    connect(newButton,&QPushButton::clicked,this,[=](){
        setNewMeasurement();
    });

    refLay->addWidget(newButton);
    refLay->addWidget(list);

    e = new ContactMeasurementEditor(measurement,dbHelper,this);
    editorLay->addWidget(e);
    connect(e,&ContactMeasurementEditor::saved,this,ContactMeasurementPage::saved);

}

void ContactMeasurementPage::setMeasurement(ContactMeasurement *newMeasurement)
{
    measurement = newMeasurement;
    // setWidgets();
}

void ContactMeasurementPage::openMesurementEditor(QString id)
{

    measurement->contactMeasureID = id;

    measurement->lables.clear();
    measurement->valueMap.clear();
    measurement = dbHelper->getContactMeasurement(measurement);
    qDebug()<<"Page Length of Details : "<<measurement->lables.length()<<measurement->reference;
    qDebug()<<"Measure Name :"<<measurement->measurement->measurementName;
    setFormWidget();

    /*
     * 1. On Refence Selected
     * 2. Get Mesurement from DB
     * 3. Open Measurement
     *
     *
     * */
    // ContactMeasurementEditor *editor = new ContactMeasurementEditor(measurement,dbHelper,this);
    // editor->setWindowFlags(Qt::Window);
    // editor->show();
}

void ContactMeasurementPage::resetMeasurementEditor()
{

}

void ContactMeasurementPage::setFormWidget()
{

    // clearEditor();

    e->setMeasure(measurement);
    // e->setWindowFlags(Qt::Window);

    // e->show();
    qDebug()<<"Editor Set";
}

void ContactMeasurementPage::clearEditor()
{
    if (!editorLay) return;

    while (editorLay->count() > 0) {
        // Get the item at form 0
        QLayoutItem *item = editorLay->takeAt(0);

        // If the item has a widget, delete it
        if (item->widget()) {
            delete item->widget();
        }

        // Delete the layout item itself
        delete item;
    }

}

void ContactMeasurementPage::saved()
{
    setListData();
    qDebug()<<"Saving page lbl : "<<measurement->lables[0].lable.labelName;
    emit measurementSelected(measurement);
    // setNewMeasurement();
    this->close();
}
