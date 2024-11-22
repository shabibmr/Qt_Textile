#include "itemdetailview.h"
#include "ui_itemdetailview.h"
#include <QMessageBox>
#include <QProgressBar>
#include <QCheckBox>
#include <QToolButton>

ItemDetailView::ItemDetailView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ItemDetailView)
{
    ui->setupUi(this);

    dbHelper = new QuotationDatabaseHelper();
    ui->itemName->setReadOnly(true);
    ui->length->setReadOnly(true);

    ui->dimensionLineEdit->setReadOnly(true);
    ui->narration->setReadOnly(true);
    ui->description->setReadOnly(true);
    ui->filePath->setReadOnly(true);

    ui->tableWidget->setColumnCount(4);

    ui->dimensionWidget->hide();
    ui->UploadWidget->hide();

    ui->progressBar->setValue(0);
    // setImagesTable();



}

ItemDetailView::~ItemDetailView()
{
    delete ui;
}

void ItemDetailView::setTable()
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




        if(obj.id>0){

            QToolButton* check = new QToolButton;

            check->setCheckable(true);
            check->setText("Approve");
            check->setProperty("check",0);
            check->setProperty("id",i);
            if(obj.status==1){
                check->setText("");
                check->setChecked(true);
                check->setIcon(QIcon(":/icons/check.ico"));
                check->setIconSize(QSize(16,16));
                check->setToolButtonStyle(Qt::ToolButtonIconOnly);
                check->setAutoRaise(true);
                check->setProperty("check",1);

            }

            QObject::connect(check,SIGNAL(toggled(bool)),this,SLOT(checkToggle(bool)));
            //            QWidget *wid = new QWidget;
            //            QHBoxLayout *lay = new QHBoxLayout();
            //            wid->setLayout(lay);
            //            lay->addWidget(check);
            //            check->setStyleSheet("width:50px;height:50px");
            ui->tableWidget->setCellWidget(i,3,check);

        }

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

    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);


}

void ItemDetailView::on_cancelButton_clicked()
{
    this->close();
}

void ItemDetailView::on_saveButton_clicked()
{

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Save?","Are You Sure You want to Save Changes",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        for(int i =0;i<ui->tableWidget->rowCount();i++){


            QToolButton *box =   qobject_cast <QToolButton*> (ui->tableWidget->cellWidget(i,5));
            qDebug()<<"Setting for "<<docObj[i].id << " is "<<box->property("check").toInt();

            if(box->property("check").toInt()==1){
                qDebug()<<"Setting for "<<docObj[i].id;
                dbHelper->setImageApprovalStatusByID(docObj[i].id,docObj[i].status);
            }
            else{
                qDebug()<<"Not Checked "<<docObj[i].id;
                dbHelper->setImageApprovalStatusByID(docObj[i].id,docObj[i].status);

            }
        }
        this->close();
    }
}


void ItemDetailView::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Select a image for "+ui->itemName->text()
                                                    ,
                                                    QDir::homePath(),
                                                    tr("Image Files (*.png *.jpg *.bmp *.pdf)"));

    ui->filePath->setText(fileName);
}

void ItemDetailView::on_uploadButton_clicked()
{
    QFile f(ui->filePath->text());
    if(f.open(QIODevice::ReadOnly))
    {
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());

        DocumentsDataModel doc;
        doc.fileObj = f.readAll();
        f.close();
        doc.filename = filename;
        doc.fileType = fileInfo.completeSuffix();
        doc.fileDescr = ui->fileDescription->text();
        doc.addedByID = LoginValues::getUserID();
        doc.TagType = DocumentType::Drafts;
        doc.ReqID = QString::number(invItem.BaseItem.requirementItemIdOld);
        qDebug()<<"Requi ID"<<invItem.BaseItem.requirementItemIdOld;
        doc.projectID = projectID;
        doc.quoteNo = quoteNo;
        QMessageBox::StandardButton reply;
        reply=QMessageBox::question(this,"Upload File","Are You Sure you want to upload File?",QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            doc.id= dbHelper->insertQImage(doc);
            docObj.append(doc);


            setTable();
        }

    }
    else{
        QMessageBox box;
        box.setText("Cannot Open File!!!");
        box.exec();
    }
    ui->UploadWidget->hide();
}

void ItemDetailView::on_filePath_textChanged(const QString &arg1)
{
    ui->UploadWidget->show();
}

void ItemDetailView::downloadClicked(bool val)
{
    int row = sender()->property("row").toInt();
    qDebug()<<docObj[row].id;
    QByteArray ba = dbHelper->getFilewithId(docObj[row].id);
    qDebug()<<"image size "<<ba.size();
    QString path = QFileDialog::getExistingDirectory(this,"Select folder",QDir::currentPath());
    QFile file(path+"/"+docObj[row].filename);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.writeRawData(ba,ba.size());
    //file.write(ba);
    file.close();


}

void ItemDetailView::delButtonClicked(bool val)
{
    int row = sender()->property("row").toInt();
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete File","Are You Sure you want to Delete File?",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        dbHelper->deleteQImage(docObj[row].id);
        docObj.removeAt(row);
        setTable();
    }

}

void ItemDetailView::setImagesTable()
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

    if(!LoginValues::model.Projects_Add)
        ui->tableWidget->hideColumn(5);


    setTable();
}

void ItemDetailView::checkToggle(bool val)
{
    QToolButton* butt = qobject_cast <QToolButton*> (sender());
    qDebug()<<"Value = "<<val;
    int r = butt->property("id").toInt();

    if(val){
        docObj[r].status=1;
        butt->setText("");
        //        butt->setStyleSheet("background-color:#05ff04");
        qDebug()<<"Value = "<<butt->isChecked();
        butt->setIcon(QIcon(":/icons/check.ico"));
        butt ->setIconSize(QSize(16,16));
        butt ->setToolButtonStyle(Qt::ToolButtonIconOnly);
        butt ->setAutoRaise(true);
        butt ->setProperty("check",1);
        qDebug()<<"Chenged check to 1";
        qDebug()<<"New Value"<<butt->property("check").toInt();

    }
    else{
        docObj[r].status=0;

        butt->setStyleSheet("background-color:#ececec");
        butt->setText("Approve");
        butt ->setToolButtonStyle(Qt::ToolButtonTextOnly);
        butt->setIcon(QIcon(":/iconxs/"));
        butt->setProperty("check",0);
        qDebug()<<"Chenged check to 0";
    }
}

void ItemDetailView::editItem(CompoundItemDataObject item)
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

    docObj = dbHelper->getALLFilesWithReqID(invItem.BaseItem.requirementItemIdOld);
    setImagesTable();
}

void ItemDetailView::setQuoteParams(QString quoteNo, QString projID)
{
    this->quoteNo=quoteNo;
    this->projectID=projID;
}
