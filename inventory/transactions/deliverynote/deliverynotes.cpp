#include "deliverynotes.h"
#include "ui_deliverynotes.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "customwidgets/Delegates/reportcommondelegate.h"
deliveryNotes::deliveryNotes(bool hideCreate, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::deliveryNotes)
{
    ui->setupUi(this);
     ui->createNewPushButton->setHidden(hideCreate);
    ui->fromDateEdit->setDate(QDate::currentDate());
    ui->toDateEdit->setDate(QDate::currentDate());
    dbhelper = new DeliveryNoteDatabaseHelper();

    ui->tableView->setItemDelegate(new ReportCommonDelegate());

    setTableView();
}

deliveryNotes::~deliveryNotes()
{
    delete ui;
}

void deliveryNotes::setHeading(QString label)
{
    ui->titleLabel->setText(label);
}

void deliveryNotes::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void deliveryNotes::setTableView()
{
    QDate fromDate = ui->fromDateEdit->date();
    QDateTime fromDateTime = QDateTime(fromDate);
    fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));

    QDate toDate = ui->toDateEdit->date().addDays(CounterSettingsDataModel::EndDay);
    QDateTime toDateTime = QDateTime(toDate);
    toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));

    model = dbhelper->getVoucherListByDateModel(fromDate,toDate);
    sort = new GMItemSortFilterProxyModel(this);
    sort->setDynamicSortFilter(true);
    sort->setSourceModel(model);

    ui->tableView->setModel(sort);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->hideColumn(3);

    float total =0;
    for(int i=0;i<ui->tableView->model()->rowCount();i++){
        total+= model->record(i).value(6).toFloat();
    }
    ui->totalAmountLineEdit->setText(QString::number(total,'f',2));
//    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);

}

void deliveryNotes::on_createNewPushButton_clicked()
{
    AddDeliveryNoteWidget = new DeliveryNoteEditor(this);
    AddDeliveryNoteWidget->setWindowFlags(Qt::Window);
    AddDeliveryNoteWidget->showMaximized();
    QObject::connect(AddDeliveryNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}



void deliveryNotes::on_okPushButton_clicked()
{
    setTableView();
}

void deliveryNotes::delButtonClicked(bool)
{
    int x= sender()->property("row").toInt();
    qDebug()<<x;
}

void deliveryNotes::on_tableView_doubleClicked(const QModelIndex &index)
{
    int row = sort->mapToSource(index).row();
    QString voucherNo = model->record(row).value(1).toString();
    QString voucherPrefix = model->record(row).value(3).toString();
    qDebug()<<"at 0 "<<voucherNo;
    GeneralVoucherDataObject *voucher = dbhelper->getVoucherByVoucherNoPtr(voucherNo, voucherPrefix);

        AddDeliveryNoteWidget = new DeliveryNoteEditor(this);
        AddDeliveryNoteWidget->setWindowFlags(Qt::Window);
        AddDeliveryNoteWidget->showVoucher(voucher);
        AddDeliveryNoteWidget->showMaximized();
        QObject::connect(AddDeliveryNoteWidget,SIGNAL(closing()),this,SLOT(setTableView()));
}
