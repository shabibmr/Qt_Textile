#include "propertylist.h"
#include "ui_propertylist.h"
#include <QSqlRecord>

PropertyList::PropertyList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyList)
{
    ui->setupUi(this);
    dbHelper = new PropertyDatabaseHelper();
    settable();
}

PropertyList::~PropertyList()
{
    delete ui;
}

void PropertyList::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void PropertyList::on_createNewPushButton_clicked()
{
    addpropertyWidget = new AddProperty(this);
    addpropertyWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addpropertyWidget->show();
    QObject::connect(addpropertyWidget,SIGNAL(closing()),this,SLOT(settable()));
}


void PropertyList::settable()
{
    model = dbHelper->getPropertyMasterList();
    ui->tableWidget->setModel(model);
    ui->tableWidget->hideColumn(0);


}

void PropertyList::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();

    int id = model->record(r).value(0).toInt();
    PropertyDataModel *prop = dbHelper->getPropertyById(id);
    addpropertyWidget = new AddProperty(this);
    addpropertyWidget->setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint);
    addpropertyWidget->setTitle("Update  Property");
    addpropertyWidget->edit(prop);
    addpropertyWidget->show();
    QObject::connect(addpropertyWidget,SIGNAL(closing()),this,SLOT(settable()));
}

