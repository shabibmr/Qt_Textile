#include "payslipreport.h"
#include "ui_payslipreport.h"

PayslipReport::PayslipReport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PayslipReport)
{
    ui->setupUi(this);
    dbHelper = new AttendanceDatabaseHelper;
    payslipHelper = new PayslipDatabaseHelper;
    model = new QSqlQueryModel();

    ui->fromDateDateEdit_2->setDate(QDate::currentDate().addDays(- QDate::currentDate().day()));

    ui->toDateDateEdit_2->setDate(QDate::currentDate());

    tableView = new ReportsTableView;
    connect(tableView, &QTableView::doubleClicked, this, &PayslipReport::setPaymentVoucher);
    ui->tableLay->addWidget(tableView);


    setTable();
}

PayslipReport::~PayslipReport()
{
    delete ui;
}

void PayslipReport::setTable()
{
    QDate fromDate = ui->fromDateDateEdit_2->date();
    QDate toDate = ui->toDateDateEdit_2->date();

    model = dbHelper->getMonthlyPayslipReportModel(fromDate, toDate);
    sort = new GMItemSortFilterProxyModel(this);
    tableView->setItemDelegate(new ReportCommonDelegate());
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    tableView->setModel(sort);
    tableView->setModelQry(model);

    QList<int> avlCols; avlCols<<2<<3<<4<<5<<6<<7<<8<<9<<10<<11<<12<<13;
    tableView->setAvlcols(avlCols);
    tableView->setColumns();

    tableView->setSortingEnabled(true);
    tableView->hideColumn(0);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void PayslipReport::setPaymentVoucher(QModelIndex index)
{
    int row = sort->mapToSource(index).row();

    PaymentVoucherEditor *editor = new PaymentVoucherEditor(new PaymentVoucherDatabaseHelper);
    editor->setWindowFlags(Qt::Window);
    QString empId = model->record(row).value(0).toString();
    qDebug()<<"Doubl clickedd on "<<empId;
    float amount = model->record(row).value(13).toFloat();

    PayslipDataModel *payslipModel = payslipHelper->getPayslipVoucherByEmployee(empId,
                                                                                   ui->fromDateDateEdit_2->date(),
                                                                                   ui->toDateDateEdit_2->date());
    if(payslipModel->empId.length()>0)
    {
        qDebug()<<"voucher exists";
        PaymentVoucherDatabaseHelper *paymentVoucherHelper = new PaymentVoucherDatabaseHelper;
        editor->setVoucher(paymentVoucherHelper->getVoucherByVoucherNoPtr(payslipModel->voucherNo,
                                                                             payslipModel->voucherPrefix));
        editor->showVoucher();
    }

    else{
        qDebug()<<"new voucher";
        EmployeeSalaryDataModel *emp = payslipHelper->getEmpSalaryDetailsAsPtr(empId);
        qDebug()<<"emp salary"<<emp->salaryLedgerId<<empId;
        LedgerMasterDatabaseHelper *ledHelper = new LedgerMasterDatabaseHelper;
        LedgerMasterDataModel led = ledHelper->getLedgerObjectByID(emp->salaryLedgerId);
        led.amount = amount;
        led.drAmount = amount;



        editor->appendLedgersList(led);
        editor->setNarration("Payslip for period from :" + ui->fromDateDateEdit_2->date().toString("yyyy-MM-dd")
                             + " , to: " + ui->toDateDateEdit_2->date().toString("yyyy-MM-dd"));
        connect(editor, &PaymentVoucherEditor::VoucherSaved, this, [=]
                (QString vNo, QString vPrefix, QString vType){

            PayslipDataModel *payslipModel = new PayslipDataModel;
            payslipModel->empId = empId;
            payslipModel->voucherNo = vNo;
            payslipModel->voucherType = vType;
            payslipModel->voucherPrefix = vPrefix;
            payslipModel->Period_StartDate = ui->fromDateDateEdit_2->date();
            payslipModel->Period_EndDate = ui->toDateDateEdit_2->date();
            payslipModel->Date_Created = QDate::currentDate();

            payslipHelper->insertPayslip(payslipModel);

        });
    }
    editor->show();




}
