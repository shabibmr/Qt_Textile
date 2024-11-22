#include "attendanceeditor.h"
#include "ui_attendanceeditor.h"

AttendanceEditor::AttendanceEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttendanceEditor)
{
    ui->setupUi(this);


    this->setWindowTitle(windowTitle);
    this->setObjectName("background");
    this->setStyleSheet("QWidget#background{background-color:#e0f7fa}");

    qDebug()<<"Attendance editor";
    initDbHelpers();
    initVoucher();
    initGlobalObjects();
    setWidgets();
    initExtraActions();
}

AttendanceEditor::~AttendanceEditor()
{
    delete ui;
}

void AttendanceEditor::initExtraActions()
{
    saveButton = new QtMaterialFloatingActionButton((QtMaterialTheme::icon("content", "save")));
    saveButton->setMini(true);
    saveButton->setXOffset(10);
    saveButton->setYOffset(10);
    saveButton->setParent(this);
    saveButton->setToolTip("Save Voucher");
    connect(saveButton, &QtMaterialFloatingActionButton::clicked, this, &AttendanceEditor::saveVoucher);

}

void AttendanceEditor::initDbHelpers()
{
    dbHelper = new AttendanceDatabaseHelper;
    userHelper = new UserProfileDatabaseHelper;

}

void AttendanceEditor::initVoucher()
{
    qDebug()<<"init voucher";
    attendanceList = new AttendanceListDataModel();

    attendanceList->date = QDate::currentDate();
}

void AttendanceEditor::initGlobalObjects()
{
    lis<<"#"<<"Employee"<<"Start Time"<<"End Time"<<"Working Hours"<<"OverTime"<<"Half day";
    DataTableWidget = new EmployeeTableView(attendanceList);
    DataTableWidget->setEnabled(true);
    DataTableWidget->setItemDelegate(new GMAttendanceDelegate(
                                         userHelper,userHelper->getEmployeeQueryForSearch()
                                         ));

    model = new GMEmployeeModel(attendanceList,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();

    voucherDateWidget = new GMDateEdit(&attendanceList->date);

    QObject::connect(voucherDateWidget,&GMDateEdit::editFinished,
                     this,[=]{
        qDebug()<<"new date"<<voucherDateWidget->date();
        attendanceList = dbHelper->getAttendanceListByDate(voucherDateWidget->date());
        resetWidgets();
    });

}

void AttendanceEditor::setWidgets()
{
    qDebug()<<"set widgets";
    QLabel *windowLabel = new QLabel(windowTitle);

    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), Qt::white);
    windowLabel->setPalette(palette);

    windowLabel->setFont(QFont("Roboto", 18, QFont::Normal));

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);

    QPointer<QHBoxLayout> voucherData = new QHBoxLayout;
    QPointer<QHBoxLayout> extrasLayout = new QHBoxLayout;
    QPointer<QVBoxLayout> tablesLayout = new QVBoxLayout;

    voucherData->setContentsMargins(20,0,0,0);
    mainLayout->addLayout(voucherData);
    mainLayout->addLayout(extrasLayout);
    mainLayout->addLayout(tablesLayout);

    tablesLayout->addWidget(DataTableWidget);


    voucherData->addWidget(new QLabel("Date"));
    voucherData->addWidget(voucherDateWidget);
    voucherData->addStretch(1);

    ui->verticalLayout->addWidget(Widget);

}

void AttendanceEditor::resetWidgets()
{
    delete model;
    attendanceList = dbHelper->getAttendanceListByDate(voucherDateWidget->date());
    qDebug()<<attendanceList->date;
    voucherDateWidget->setVariable(&attendanceList->date);
    qDebug()<<"reset";

    model = new GMEmployeeModel(attendanceList,lis.size(),this);
    model->setHeaderStrings(lis);
    DataTableWidget->setAttendanceList(attendanceList);
    DataTableWidget->setModel(model);
    DataTableWidget->setColumns();


}

void AttendanceEditor::showVoucher(AttendanceListDataModel *obj)
{

    attendanceList = obj;
    resetWidgets();

}

void AttendanceEditor::closeEditor()
{
    emit closing();
    this->close();

}

void AttendanceEditor::saveVoucher()
{
    if(attendanceList->newEntry){
        qDebug()<<"insert attendance";
        if(dbHelper->insertAttendance(attendanceList)){
            attendanceList->newEntry = false;
            QMessageBox box; box.setText("Saved" ); box.exec();
        }
    }
    else{
        qDebug()<<"updating attendance";
        if(dbHelper->updateAttendance(attendanceList)){
            QMessageBox box; box.setText("Updated"); box.exec();
        }
    }

    resetWidgets();

}

