#include "shiftwisereport.h"
#include "ui_shiftwisereport.h"

#include <QDoubleValidator>

ShiftWiseReport::ShiftWiseReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShiftWiseReport)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->toDateDateEdit->setDate(QDate::currentDate());
    tableView = new ReportsTableView("");
    ui->tableLayout->addWidget(tableView);

//    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    connect(tableView, &ReportsTableView::doubleClicked, this,&ShiftWiseReport::on_amountWiseTableView_clicked);

    ui->dateFromEdit_Abr->setDate(QDate(QDate::currentDate().year(),QDate::currentDate().month(),1));
    ui->dateToEdit_Abr->setDate(QDate::currentDate());


    dbHelper = new LedgerHelper;
    shifthelper = new RestaurantShiftDatabaseHelper;
    ui->tabWidget->removeTab(0);
    ui->editButton->hide();
    setTable();
    ui->mailabridge->hide();
    ui->printButtonAbridge->hide();

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
//    shareButton->setColor(QColor(255,250,0));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked,
                     this,&ShiftWiseReport::showShareOptions);

    ui->horizontalLayout->addWidget(shareButton);
    ui->excelButton->hide();
    ui->printButton->hide();
    ui->prefixCombo->addItem("%");
    ui->prefixCombo->addItem("A");
    ui->prefixCombo->addItem("B");
    ui->prefixCombo->addItem("C");
    ui->prefixCombo->addItem("D");
    ui->prefixCombo->addItem("E");


}

ShiftWiseReport::~ShiftWiseReport()
{
    delete ui;
}

void ShiftWiseReport::setup()
{

}

void ShiftWiseReport::setDeleteAllowed(bool value)
{
    deleteAllowed = value;
}

void ShiftWiseReport::on_OKButton_clicked()
{
    setTable();
}

void ShiftWiseReport::on_excelButton_clicked()
{
    qDebug()<<"create Excel";

    QDate fDate = ui->fromDateDateEdit->date();
    QDate tDate = ui->toDateDateEdit->date();
    QString fromDate = fDate.toString("yyyy-MM-dd");
    QString toDate = tDate.toString("yyyy-MM-dd");
    QString wwaitername ="";

    QString reportName = "Shiftwise Report";

    QDate curDate = QDate::currentDate();
    QString dateNow = curDate.toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QStringList cols; cols<<"30"<<"20"<<"20"<<"20";
    reportName = "Billwise Sales Report "+wwaitername;
    QList<int> avlCols; avlCols<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15;
    ex->GenericReportFromTableView(fileName,reportName,cols,tableView,fromDate,toDate,avlCols);

}

void ShiftWiseReport::on_printButton_clicked()
{
    if(shiftID>0){
        ShiftDataModel shift = shifthelper->getShiftDetailByshiftID(shiftID);
        qDebug()<<Q_FUNC_INFO<<shift.denominations->total;
        CounterPrinterHelper *printer = new CounterPrinterHelper;
        printer->printShiftReport(&shift, false, false);

    }
}

void ShiftWiseReport::on_backButton_clicked()
{
    this->close();
}

void ShiftWiseReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbHelper->getShiftModel(fromDateTime,toDateTime);

    tableView->setModel(model);
    tableView->hideColumn(0);

}

void ShiftWiseReport::on_amountWiseTableView_doubleClicked(const QModelIndex &index)
{
    return;
    int row = index.row();

    if(row<0){
        return;
    }

    qDebug()<<"_id= "<<model->record(row).value(0).toInt();

    int _id = model->record(row).value(0).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,"Delete Shift","Are you sure you want to delete Shift Entry?",
                                  QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes && deleteAllowed){
        shifthelper->deleteShift(_id);
        setTable();
    }
}

void ShiftWiseReport::on_amountWiseTableView_clicked(const QModelIndex &index)
{
    int row = index.row();

    if(row<0){
        return;
    }

    shiftID = model->record(row).value(0).toInt();
    cashincounter = model->record(row).value(14).toFloat();

    ui->editButton->show();
}

void ShiftWiseReport::on_editButton_clicked()
{
    QWidget* editWidget = new QWidget(this);
    editWidget->setMaximumSize(250,250);

    QLabel *label = new QLabel("Enter New Withdrawal Amount  : ");
    lineedit = new QLineEdit(this);

    lineedit->setValidator(new QDoubleValidator(0,999,0,this));
    QGridLayout* lay = new QGridLayout(editWidget);
    lay->addWidget(label,0,0);
    lay->addWidget(lineedit,0,1);

    editWidget->setLayout(lay);
    editWidget->setWindowFlag(Qt::Window);

    QPushButton *okButton = new QPushButton("Update");
    QPushButton *cancelButton = new QPushButton("Cancel");

    lay->addWidget(okButton,2,1);
    lay->addWidget(cancelButton,2,2);

    editWidget->show();

    QObject::connect(okButton,&QPushButton::clicked,[=](){
        float withdraw = lineedit->text().toFloat();
        float balance = cashincounter-withdraw;
        shifthelper->updateShiftWithdrawAmt(shiftID,withdraw,balance);
        editWidget->close();
    });

    QObject::connect(cancelButton,&QPushButton::clicked,[=](){
        editWidget->close();
    });
}


// - 2 abridge report ;

void ShiftWiseReport::on_mailabridge_clicked()
{
    Calc.sendMail();
}

void ShiftWiseReport::on_printButtonAbridge_clicked()
{
    Calc.printReport();

}

void ShiftWiseReport::on_okButton_clicked()
{

    QDate fromDate = ui->dateFromEdit_Abr->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->dateToEdit_Abr->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
    Calc.shift->voucherPrefix=ui->prefixCombo->currentText();
    Calc.setDate(fromDateTime,toDateTime);
    Calc.CalculateValues();
    QString str = Calc.getShiftasHTML();
    ui->textEdit->clear();
    ui->textEdit->setHtml(str);
    ui->mailabridge->show();
    ui->printButtonAbridge->show();

}

void ShiftWiseReport::showShareOptions()
{
    qDebug()<<Q_FUNC_INFO;
    shareOptionsWidget = new ShareTools("Shiftwise Report", shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->setAttribute(Qt::WA_DeleteOnClose);
    shareOptionsWidget->show();
    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ShiftWiseReport::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ShiftWiseReport::printTable);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ShiftWiseReport::on_excelButton_clicked);


}

void ShiftWiseReport::printReport(QPrinter *printer)
{
    qDebug()<<Q_FUNC_INFO;

    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);
    if(shiftID>0){
        ShiftDataModel shift = shifthelper->getShiftDetailByshiftID(shiftID);
        CounterPrinterHelper printer;
        printer.printShiftReport(&shift);
    }

}

void ShiftWiseReport::printTable(QPrinter *printer)
{
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);


    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
 //   QString total = ui->totalLineEdit->text();
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
  //  extraBottom<< "Total: " <<total;
    QList<int> avlCols; avlCols<<1<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13<<14<<15;
    widthList <<7<<6<<6<<6<<7<<6<<6<<7<<7<<7<<7<<7<<7<<7<<7;
    AlignmentList<< 0<< 1 << 0 <<1;

//    printerHelper-> printReportGeneral(ui->amountWiseTableView
//                                       ,widthList,"Shift List",fromDate,toDate,AlignmentList
//                                       ,extraTop,extraBottom,avlCols);

}
