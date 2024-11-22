#include "workorderitemdetail.h"
#include "ui_workorderitemdetail.h"
#include <QMessageBox>

WorkOrderItemDetail::WorkOrderItemDetail(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkOrderItemDetail)
{
    ui->setupUi(this);

    dbHelper = new SalesOrderDatabaseHelper();
    qHelper = new QuotationDatabaseHelper();
    ui->itemName->setReadOnly(true);
    ui->length->setReadOnly(true);

    ui->dimensionLineEdit->setReadOnly(true);
    ui->narration->setReadOnly(true);
    ui->description->setReadOnly(true);

    ui->tableWidget->setColumnCount(4);

    ui->dimensionWidget->hide();

    // setImagesTable();

}

WorkOrderItemDetail::~WorkOrderItemDetail()
{
    delete ui;
}

void WorkOrderItemDetail::setTable()
{
    int i=0;
    ui->tableWidget->setRowCount(0);
    //    docObj = dbHelper->getALLFilesWithReqID(invItem.BaseItem.requirementItemId);

    for(DocumentsDataModel obj:docObj){
        ui->tableWidget->insertRow(i);
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        ui->tableWidget->setItem(i,0,SLNO);

        QTableWidgetItem* filename = new QTableWidgetItem;
        filename->setText(obj.filename);
        ui->tableWidget->setItem(i,1,filename);

        QTableWidgetItem* filedesc = new QTableWidgetItem;
        filedesc->setText(obj.fileDescr);
        ui->tableWidget->setItem(i,2,filedesc);

        QTableWidgetItem* status = new QTableWidgetItem;
        QString stt = obj.status==1 ? "Approved": "Not Approved";
        status->setText(stt);
        ui->tableWidget->setItem(i,3,status);


        QToolButton *upload = new QToolButton(this);
        upload->setIcon(QIcon(":/images/dowload.png"));
        upload->setProperty("row",i);
        upload->setAutoRaise(true);
        ui->tableWidget->setCellWidget(i,4,upload);
        QObject::connect(upload,SIGNAL(clicked(bool)),this,SLOT(downloadClicked(bool)));

        QToolButton *del = new QToolButton(this);
        del->setIcon(QIcon(":/icons/trash.ico"));
        del->setProperty("row",i);
        del->setAutoRaise(true);
        ui->tableWidget->setCellWidget(i,5,del);
        QObject::connect(del,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        i++;
    }

}

void WorkOrderItemDetail::on_cancelButton_clicked()
{
    this->close();
}

void WorkOrderItemDetail::on_saveButton_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Save?","Are You Sure You want to Save Changes",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){

        QString serial = ui->serialLineEdit->text();
//        dbHelper->setSerialNumber(serial,invItem.BaseItem.requirementItemIdOld,invItem.BaseItem.quantity,quoteNo);
        emit closing();
        this->close();

    }
}

void WorkOrderItemDetail::downloadClicked(bool val)
{
    int row = sender()->property("row").toInt();
    qDebug()<<docObj[row].id;
    QByteArray ba = qHelper->getFilewithId(docObj[row].id);
    qDebug()<<"image size "<<ba.size();
//    QString path = QFileDialog::getExistingDirectory(this,"Select folder",QDir::currentPath());
//    QFile file(path+"/"+docObj[row].filename);
//    file.open(QIODevice::WriteOnly);
//    QDataStream out(&file);
//    out.writeRawData(ba,ba.size());
//    //file.write(ba);
//    file.close();

    docObj[row].fileObj=ba;

    PrinterHelper pHelper;
    pHelper.printWorkOrderDrafts(invItem,docObj[row]);


}

void WorkOrderItemDetail::delButtonClicked(bool val)
{
    int row = sender()->property("row").toInt();
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete File","Are You Sure you want to Delete File?",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
//        dbHelper->deleteQImage(docObj[row].id);
        docObj.removeAt(row);
        setTable();
    }

}

void WorkOrderItemDetail::setImagesTable()
{
    ui->tableWidget->setColumnCount(6);

    int i=0;

    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);


    QTableWidgetItem* fileName = new QTableWidgetItem;
    fileName->setText("File Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,fileName);

    QTableWidgetItem* filesDescription = new QTableWidgetItem;
    filesDescription->setText("File Description");
    ui->tableWidget->setHorizontalHeaderItem(i++,filesDescription);


    QTableWidgetItem* status = new QTableWidgetItem;
    status->setText("Status");
    ui->tableWidget->setHorizontalHeaderItem(i++,status);

    QTableWidgetItem* View = new QTableWidgetItem;
    View->setText("View");
    ui->tableWidget->setHorizontalHeaderItem(i++,View);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setText("Delete");
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableWidget->setColumnWidth(0,25);
    ui->tableWidget->setColumnWidth(4,60);
    ui->tableWidget->setColumnWidth(5,60);

    ui->tableWidget->hideColumn(5);

    setTable();
}

void WorkOrderItemDetail::editItem(CompoundItemDataObject item)
{
    invItem = item;
    if(item.BaseItem.isCustomItem){
        ui->dimensionWidget->show();
        ui->length->setText(QString::number(item.BaseItem.length));
        ui->dimensionLineEdit->setText(item.BaseItem.Dimension);
    }

    ui->itemName->setText(invItem.BaseItem.ItemName);
    ui->narration->setPlainText(invItem.BaseItem.narration);
    ui->description->setPlainText(invItem.BaseItem.ItemDescription);
    ui->serialLineEdit->setText(invItem.BaseItem.SerailNumber);

    docObj = qHelper->getALLFilesWithReqID(invItem.BaseItem.requirementItemIdOld);

    setImagesTable();
}

void WorkOrderItemDetail::setQuoteParams(QString quoteNo, QString projID)
{
    this->quoteNo=quoteNo;
    this->projectID=projID;
}
