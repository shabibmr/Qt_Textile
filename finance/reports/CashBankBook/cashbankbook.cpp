#include "cashbankbook.h"
#include "ui_cashbankbook.h"

#include "voucherOperations/vouchereditorselector.h"
#include "customwidgets/test/ledgerreportdelegate.h"
#include "counter/PrinterHelper/counterprinterhelper.h"

CashBankBook::CashBankBook(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CashBankBook)
{
    ui->setupUi(this);
    LedgerMasterDatabaseHelper* db= new  LedgerMasterDatabaseHelper();
    dbhelp = new LedgerHelper();
    QStringList salesGroups;
    salesGroups.append("0x5x23");
    salesGroups.append("0x5x19");
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());

//    wid= new GMLedgerLineMaterial(db,db->getLedgerQueryString(salesGroups),led);

//    wid->setLabel("Customer Name");
//    wid->setParent(this);
//    wid->setStyleSheet("GMLedgerLineMaterial{background-color:#e0f7fa;}");
//    wid->setFixedWidth(200);
//    ui->horizontalLayout_2
//    connect(wid,&GMLedgerLineMaterial::itemSelectedwithItem,
//            this,[=](LedgerMasterDataModel led){
////        voucher->ledgerObject = led;
////        if(led.LedgerID.length()<1)
////            return;
//        this->led = &led;
//        qDebug()<<"Ledger selected "<<this->led->LedgerName;
//    });

    GMLedgerSearchLite *ledSearch= new GMLedgerSearchLite(db->getLedgerQueryString());
    ledSearch->setParent(this);
    ledSearch->setPlaceholderText("Search Ledger...");
    ledSearch->setFixedWidth(200);
    QWidget *SearchWid = new QWidget(this);
    QHBoxLayout *lay = new QHBoxLayout(SearchWid);
    QLabel *ledgerLabel = new QLabel("Ledger");
    lay->addWidget(ledgerLabel);
    lay->addWidget(ledSearch);
    SearchWid->setLayout(lay);

    connect(ledSearch,&GMLedgerSearchLite::selectedLedger,
            this,[=](LedgerMasterDataModel led){
        qDebug()<<"ledger : "<<led.LedgerName;
        this->led = led;
        qDebug()<<"updated "<<this->led.LedgerName;
        setTableView();
    });


    ui->tableView->setItemDelegate(new LedgerReportDelegate());
    ui->horizontalLayout_2->addWidget(SearchWid);


}

CashBankBook::~CashBankBook()
{
    delete ui;
}

void CashBankBook::on_OKButton_clicked()
{
    qDebug()<<Q_FUNC_INFO;
    qDebug()<<led.LedgerName;
    setTableView();

}

void CashBankBook::setTableView()
{
    qDebug()<<Q_FUNC_INFO<<led.LedgerName;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    model= dbhelp->getLedgerReportModel(led.LedgerID,fromDate,toDate);
    //  ledgerListModel = ledgerReportDbHelper->getDayBookReportModel(fromDate,toDate);

    ui->tableView->setModel(model);

    ui->tableView->hideColumn(6);
    ui->tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
}

void CashBankBook::on_excelButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();
    QString reportName = "Cash/Bank Book";

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Excel file"),"F:\\excel\\"+reportName+"_"+dateNow+".xlsx",tr("Excel Files (*.xls)"));
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;

    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    widthList,ui->tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    avlCols);
}



void CashBankBook::on_printButton_clicked()
{
    QList<int> widthList;
    QList<int> AlignmentList;
    QStringList extraTop;
    QStringList extraBottom;
    QDate fromDate = ui->fromDateDateEdit->date();
    QDate toDate = ui->toDateDateEdit->date();


    widthList <<60<<20<<20<<20<<20<<20;
    AlignmentList<<0<<0<<0<<0<<1<<1;
    CounterPrinterHelper *printer = new CounterPrinterHelper;
    QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
    printer-> printReportGeneral(ui->tableView
                                 ,widthList,"Cash/Bank Book",fromDate,toDate,AlignmentList
                                 ,extraTop,extraBottom,avlCols);
}

void CashBankBook::on_backButton_clicked()
{

}

void CashBankBook::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    VoucherEditorSelector* vSel = new VoucherEditorSelector();
    QString vno = model->record(row).value(2).toString();
    QString vpref = model->record(row).value(3).toString();
    QString vType = model->record(row).value(6).toString();
    vSel->openEditor(vno,vpref,vType);
}
