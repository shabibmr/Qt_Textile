#include "shiftentrywidget.h"
#include "ui_shiftentrywidget.h"
#include "datamodels/Counter/countersettingsdatamodel.h"
#include "database/Settings/ConfigurationSettingsDatabaseHelper.h"

ShiftEntryWidget::ShiftEntryWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShiftEntryWidget)
{
    ui->setupUi(this);

    dbHelper = new RestaurantShiftDatabaseHelper;

    initShift();
    setWidgets();
    initExtraActions();
}

ShiftEntryWidget::~ShiftEntryWidget()
{
    delete ui;
}

void ShiftEntryWidget::disablSave()
{
    saveButton->hide();
}

void ShiftEntryWidget::setWidgets()
{
    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setColor(QColor(245,0,87));
    backButton->setIconSize(QSize(30,30));
    connect(backButton, &QtMaterialIconButton::clicked, this, [=](){

           this->close();

    });

    ui->verticalLayout->addWidget(backButton);

    denominations = new Denominations(shift);
    shifteditor = new ShiftValuesEditor(shift);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(denominations);
    hLayout->addWidget(shifteditor);

    ui->verticalLayout->addLayout(hLayout);
//    ui->verticalLayout->addWidget(saveButton, 0, Qt::AlignRight);

}

void ShiftEntryWidget::initShift()
{
    shift = new ShiftDataModel;
    shift->cashID = CounterSettingsDataModel::DefaultCash;
    shift->voucherPrefix = LoginValues::voucherPrefix;
    shift->ShiftStartDateTime = dbHelper->getlastShiftDetailByVoucherPrefix(
                shift->voucherPrefix).ShiftEndDateTime;

    if(shift->ShiftStartDateTime.toString("yyyy").length()<1 ){
        QString nulldate = "2017-01-01 00:00:00";
        shift->ShiftStartDateTime = QDateTime::fromString(nulldate,"yyyy-MM-dd hh:mm:ss");
    }

    shift->ShiftEndDateTime = QDateTime::currentDateTime();

    shift->BillsFrom =  dbHelper->getFirstBillinShift(shift);

    shift->BillsTo = dbHelper->getLastBill(shift);

}

void ShiftEntryWidget::initExtraActions()
{
    saveButton = new QtMaterialFloatingActionButton(QtMaterialTheme::icon("content", "save"));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");
    connect(saveButton, &QPushButton::clicked, this, [=](){
        qDebug()<<"save clicked"<<shifteditor->actualTotal<<shifteditor->systemTotal<<shifteditor->diffTotal;
//        if(shifteditor->systemTotal == 0)
//        {
//            QMessageBox box; box.setText("No Sales in current Shift");box.exec();
//            return;
//        }
//        else if( shifteditor->actualTotal == 0 && shifteditor->systemTotal > 0)
//        {
//            QMessageBox box; box.setText("Enter Values");box.exec();
//            return;
//        }
//       else if(qFabs( shifteditor->diffTotal) > ConfigurationSettingsDatabaseHelper::getValue(shiftCashLimit,"999999").toInt())
//       {
//           QMessageBox box; box.setText("Entered difference greater than limit. Cannot proceed!");box.exec();
//           return;
//       }
//       else {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this,"Close Shift","Are you sure you want to close Shift",
                                          QMessageBox::Yes|QMessageBox::No);
            if(reply==QMessageBox::Yes){
                emit shiftClosingStarted(shift);
                this->close();
            }
            else {
                this->close();
            }
//       }
    });




}

