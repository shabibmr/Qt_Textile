#include "measurementsmastereditor.h"

#include <QPushButton>

#include <customwidgets/Material/components/qtmaterialfab.h>


MeasurementsMasterEditor::MeasurementsMasterEditor(MeasurementMaster *measure, MeasurementsDatabaseHelper *dbHelper, QWidget *parent)
    : QWidget{parent}
{
    this->measure = measure;
    this->dbHelper = dbHelper;
    qDebug()<<"Meaas ure EDITOER";
    setWidgets();
}

void MeasurementsMasterEditor::setWidgets()
{
    QVBoxLayout *vLay = new QVBoxLayout;
    this->setLayout(vLay);
    QLineEdit *measurementNameLine = new QLineEdit(this);
    measurementNameLine->setText(measure->measurementName);
    QLabel *nameLable = new QLabel("Measurement",this);

    QLabel *newLable = new QLabel("New Lable Name",this);

    QLabel *listLable = new QLabel("Lables",this);

    QLabel *itemLabel = new QLabel("Item",this);

    connect(measurementNameLine ,&QLineEdit::textChanged,this,[=](QString text){
        measure->measurementName=text;
    });

    list = new QListView(this);

    QLineEdit *lableNameLine = new QLineEdit(this);
    connect(lableNameLine ,&QLineEdit::editingFinished,this,[=](){
        QString text = lableNameLine->text();
        if(text.length()>0){
            MeasurementLabel mlable;
            mlable.labelName = text;
            measure->lables.append(mlable);
            refreshList();
            lableNameLine->clear();
        }

    });

    QPushButton *addButton = new QPushButton(this);
    addButton->setText("Add");
    connect(addButton,&QPushButton::clicked,this,[=]{
        QString text = lableNameLine->text();
        if(text.length()>0){
            MeasurementLabel mlable;
            mlable.labelName = text;
            measure->lables.append(mlable);
            refreshList();
            lableNameLine->clear();
        }
    });

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(lableNameLine);
    hlay->addWidget(addButton);

    vLay->addWidget(nameLable);
    vLay->addWidget(measurementNameLine);
    vLay->addWidget(newLable);

    vLay->addLayout(hlay);
    vLay->addWidget(listLable);

    vLay->addWidget(list);

    QtMaterialFloatingActionButton *saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save");

    connect(saveButton,&QPushButton::clicked,this,&MeasurementsMasterEditor::saveMeasure);
    qDebug()<<"Widgets..";
    refreshList();
}

void MeasurementsMasterEditor::saveMeasure()
{
    if(measure->measurementMasterID==""){
        dbHelper->insertMeasurementMaster(measure);
    }
    else{
        dbHelper->updateMeasurementMaster(measure);
    }
    emit closing();
    emit saved();
    this->close();
}

void MeasurementsMasterEditor::deleteMeasure()
{
    dbHelper->deleteMeasurementMaster(measure);
}

void MeasurementsMasterEditor::refreshList()
{
    model = new QStandardItemModel(this);
    for(int i=0;i<measure->lables.length();i++){
        QStandardItem *item = new QStandardItem(measure->lables[i].labelName);
        model->appendRow(item);

    }
    connect(model,&QStandardItemModel::dataChanged,this,[=]
            (const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>()){
                int x1 = topLeft.row();
                int y1 = topLeft.column();
                int x2 = bottomRight.row();
                int y2 = bottomRight.column();
                qDebug()<<x1<<x2<<y1<<y2<<topLeft.model()->data(topLeft).toString();

            });
    list->setModel(model);
}
