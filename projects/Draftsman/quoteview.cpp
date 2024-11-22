#include "quoteview.h"
#include "ui_quoteview.h"
#include <QMessageBox>
#include "datamodels/projects/Quotation/quotationstatus.h"

QuoteView::QuoteView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuoteView)
{
    ui->setupUi(this);

    if(!LoginValues::model.Projects_Add)
        ui->tabWidget->removeTab(1);
    ui->tabWidget->setCurrentIndex(0);
    ui->voucherNumber->setReadOnly(true);
    ui->revWidget->hide();
    ui->uploadWidget->hide();
    dbHelper = new QuotationDatabaseHelper();
    ui->tableWidget->setColumnCount(5);
    int i =0;

    printButton = new QToolButton(this);
    printButton ->setText("Print");
    printButton ->setIcon(QIcon(":/images/printer1.ico"));
    printButton ->setIconSize(QSize(32,32));
    printButton ->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    printButton ->setAutoRaise(true);
    QObject::connect(printButton,SIGNAL(clicked(bool)),this,SLOT(printClicked(bool)));

    CompletedButton = new QToolButton(this);
    CompletedButton->setAutoRaise(true);
    CompletedButton->setText("Mark Completed");
    CompletedButton->setIcon(QIcon(":/icons/check.ico"));
    CompletedButton->setIconSize(QSize(32,32));
    CompletedButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    CompletedButton->setHidden(true);

    ui->horizontalLayout->addWidget(CompletedButton);
    ui->horizontalLayout->addWidget(printButton);

    DraftsApprovedButton = new QToolButton(this);
    DraftsApprovedButton->setAutoRaise(true);
    DraftsApprovedButton->setText("Approve Drafts\nBy Client");
    DraftsApprovedButton->setIcon(QIcon(":/icons/check.ico"));
    DraftsApprovedButton->setIconSize(QSize(32,32));
    DraftsApprovedButton->setHidden(true);
    DraftsApprovedButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    ui->horizontalLayout->addWidget(DraftsApprovedButton);


    QObject::connect(CompletedButton,SIGNAL(clicked(bool)),this,SLOT(completedClicked(bool)));

    QObject::connect(DraftsApprovedButton,SIGNAL(clicked(bool)),this,SLOT(DraftsApprovedButtonClicked(bool)));

    CompletedButton->setHidden(true);

    ui->fileName->setReadOnly(true);
    QTableWidgetItem* SLNO = new QTableWidgetItem;
    SLNO->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,SLNO);

    QTableWidgetItem* ItemName = new QTableWidgetItem;
    ItemName->setText("Item Name");
    ui->tableWidget->setHorizontalHeaderItem(i++,ItemName);

    QTableWidgetItem* Model = new QTableWidgetItem;
    Model->setText("Model");
    ui->tableWidget->setHorizontalHeaderItem(i++,Model);

    QTableWidgetItem* Brand = new QTableWidgetItem;
    Brand->setText("Brand");
    ui->tableWidget->setHorizontalHeaderItem(i++,Brand);

    QTableWidgetItem* Qty = new QTableWidgetItem;
    Qty->setText("Quantity");
    ui->tableWidget->setHorizontalHeaderItem(i++,Qty);

    QTableWidgetItem* deleteItem = new QTableWidgetItem;
    deleteItem->setText("#");
    ui->tableWidget->setHorizontalHeaderItem(i++,deleteItem);


    ui->filesTable->setColumnCount(6);
    i=0;

    QTableWidgetItem* SLNO1 = new QTableWidgetItem;
    SLNO1->setText("#");
    ui->filesTable->setHorizontalHeaderItem(i++,SLNO1);


    QTableWidgetItem* fileName = new QTableWidgetItem;
    fileName->setText("File");
    ui->filesTable->setHorizontalHeaderItem(i++,fileName);

    QTableWidgetItem* filesDescription = new QTableWidgetItem;
    filesDescription->setText("File Description");
    ui->filesTable->setHorizontalHeaderItem(i++,filesDescription);

    QTableWidgetItem* status = new QTableWidgetItem;
    status->setText("Status");
    ui->filesTable->setHorizontalHeaderItem(i++,status);

    QTableWidgetItem* View = new QTableWidgetItem;
    View->setText("Download");
    ui->filesTable->setHorizontalHeaderItem(i++,View);

    QTableWidgetItem* deleteItem1 = new QTableWidgetItem;
    deleteItem1->setText("Delete");
    ui->filesTable->setHorizontalHeaderItem(i++,deleteItem1);

    ui->filesTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    ui->filesTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::ResizeToContents);

    ui->filesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);



    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);




    settable();

    setFilesTable();
}

QuoteView::~QuoteView()
{
    delete ui;
}



void QuoteView::on_tableWidget_clicked(const QModelIndex &index)
{

}

void QuoteView::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    int r = index.row();
    ItemDetailViewWidget = new ItemDetailView(this);
    ItemDetailViewWidget->setWindowFlags(Qt::Window);
    ItemDetailViewWidget->setQuoteParams(Voucher.voucherNumber,Voucher.ProjectId);
    ItemDetailViewWidget->editItem(Voucher.InventoryItems[r]);
    ItemDetailViewWidget->showMaximized();
}

void QuoteView::settable()
{

    ui->tableWidget->setRowCount(0);

    int i=0;
    int cnt =0;
    for(CompoundItemDataObject obj:Voucher.InventoryItems){
        ui->tableWidget->insertRow(cnt);

        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(cnt+1));
        ui->tableWidget->setItem(cnt,0,SLNO);

        QTableWidgetItem* name = new QTableWidgetItem;
        name->setText(obj.BaseItem.ItemName);
        ui->tableWidget->setItem(cnt,1,name);

        QTableWidgetItem* model = new QTableWidgetItem;
        model->setText(obj.BaseItem.ItemCode);
        model->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(cnt,2,model);

        QTableWidgetItem* brand = new QTableWidgetItem;
        brand->setText(obj.BaseItem.brandName);
        brand->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(cnt,3,brand);

        QTableWidgetItem* qty = new QTableWidgetItem;
        qty->setText(QString::number(obj.BaseItem.quantity));
        qty->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->tableWidget->setItem(cnt,4,qty);
        cnt++;
    }
}

void QuoteView::setFilesTable()
{
    int i=0;
    ui->filesTable->setRowCount(0);
    //    docObj = dbHelper->getALLFilesWithReqID(invItem.BaseItem.requirementItemId);
    docObj = dbHelper->getALLFilesWithQuoteNo(Voucher.voucherNumber);
    for(DocumentsDataModel obj:docObj){
        ui->filesTable->insertRow(i);
        QTableWidgetItem* SLNO = new QTableWidgetItem;
        SLNO->setText(QString::number(i+1));
        ui->filesTable->setItem(i,0,SLNO);


        QTableWidgetItem* filename = new QTableWidgetItem;
        filename->setText(obj.filename);
        ui->filesTable->setItem(i,1,filename);

        QTableWidgetItem* filedesc = new QTableWidgetItem;
        filedesc->setText(obj.fileDescr);
        ui->filesTable->setItem(i,2,filedesc);


        QToolButton *upload = new QToolButton(this);
        upload->setIcon(QIcon(":/icons/trash.ico"));
        upload->setProperty("row",i);
        upload->setAutoRaise(true);
        ui->filesTable->setCellWidget(i,4,upload);
        QObject::connect(upload,SIGNAL(clicked(bool)),this,SLOT(downloadClicked(bool)));

        QToolButton *del = new QToolButton(this);
        del->setIcon(QIcon(":/icons/trash.ico"));
        del->setProperty("row",i);
        del->setAutoRaise(true);
        ui->filesTable->setCellWidget(i,5,del);
        QObject::connect(del,SIGNAL(clicked(bool)),this,SLOT(delButtonClicked(bool)));

        i++;
    }

}

void QuoteView::showVoucher(GeneralVoucherDataObject quote)
{
    Voucher = quote;

    qDebug()<<"Items Count = "<<Voucher.InventoryItems.size();

    ui->voucherNumber->setText(Voucher.voucherNumber);
    ui->dateEdit->setDate(Voucher.VoucherDate);
    if(Voucher.RevisionNo>0){
        ui->revWidget->show();
        ui->revisionNumber->setText(QString::number(Voucher.RevisionNo));
    }

    if(LoginValues::getModel().DraftsMan_Add && Voucher.status!=QuotationStatus::DraftsCompleted){
        CompletedButton->show();
    }

    if(LoginValues::getModel().clientApprove && Voucher.status==QuotationStatus::DraftsCompleted){
        DraftsApprovedButton->show();
    }


    //CompletedButton->setHidden(LoginValues::getModel().);
    settable();
    setFilesTable();
}

void QuoteView::completedClicked(bool t)
{
    //Mark Complted;
    QMessageBox::StandardButton repl;
    QMessageBox::StandardButton started;

    repl = QMessageBox::question(this,"Mark Completed","Are You Sure All Files Uploaded?",QMessageBox::Yes|QMessageBox::No);
    if(repl == QMessageBox::Yes){
        dbHelper->setQuotationStatus(QuotationStatus::DraftsCompleted,Voucher.voucherNumber);
        emit closing();
        this->close();
    }
    else{
        dbHelper->setQuotationStatus(QuotationStatus::DraftsStarted,Voucher.voucherNumber);
    }

}

void QuoteView::DraftsApprovedButtonClicked(bool x)
{
    QMessageBox::StandardButton repl;
    QMessageBox::StandardButton started;

    repl = QMessageBox::question(this,"Mark Completed","Are You Sure all Drafts Approved are marked?",
                                 QMessageBox::Yes|QMessageBox::No);
    if(repl == QMessageBox::Yes){
        dbHelper->setQuotationStatus(QuotationStatus::DraftsApproved,Voucher.voucherNumber);
        emit closing();
        this->close();
    }
    else{
        dbHelper->setQuotationStatus(QuotationStatus::DraftsCompleted,Voucher.voucherNumber);
    }

}

void QuoteView::on_browseButton_clicked()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         "Select a File for ",
                                         QDir::homePath(),
                                         tr("Image Files (*.png *.jpg *.bmp *.pdf)"));

    ui->fileName->setText(fileName);

}

void QuoteView::on_UploadButton_clicked()
{
    QFile f(ui->fileName->text());
    if(f.open(QIODevice::ReadOnly))
    {
        QFileInfo fileInfo(f.fileName());
        QString filename(fileInfo.fileName());

        DocumentsDataModel doc;
        doc.fileObj = f.readAll();
        f.close();
        doc.filename = filename;
        doc.fileType = fileInfo.completeSuffix();
        doc.fileDescr = ui->fileDescreption->text();
        doc.addedByID = LoginValues::getUserID();
        doc.ReqID = "";
        doc.projectID = Voucher.ProjectId;
        doc.quoteNo = Voucher.voucherNumber;
        QMessageBox::StandardButton reply;
        reply=QMessageBox::question(this,"Upload File","Are You Sure you want to upload File?",QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){
            doc.id= dbHelper->insertQImage(doc);
            docObj.append(doc);
            setFilesTable();
        }

    }
    else{
        QMessageBox box;
        box.setText("Cannot Open File!!!");
        box.exec();
    }
    ui->uploadWidget->hide();
}

void QuoteView::on_save_clicked()
{

}

void QuoteView::on_cancelButton_clicked()
{
    this->close();
}

void QuoteView::delButtonClicked(bool)
{
    int row = sender()->property("row").toInt();
    QMessageBox::StandardButton reply;
    reply=QMessageBox::question(this,"Delete File","Are You Sure you want to Delete File?",QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes){
        dbHelper->deleteQImage(docObj[row].id);
        docObj.removeAt(row);
        setFilesTable();
    }
}

void QuoteView::downloadClicked(bool)
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

void QuoteView::printClicked(bool)
{
    PrinterHelper pHelper;
    pHelper.printWorkOrder(Voucher);
}

void QuoteView::on_fileName_textChanged(const QString &arg1)
{
    ui->uploadWidget->show();

}
