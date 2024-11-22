#include "projects_sidebar.h"
#include "ui_projects_sidebar.h"

Projects_SideBar::Projects_SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Projects_SideBar)
{
    ui->setupUi(this);

    ui->reportsPushButton->setHidden(true);
    ui->scrollArea->setStyleSheet("background-color:qlineargradient(spread:pad, x1:0.5, y1:0, x2:0.5, y2:0.4, stop:0 #424242, stop:1 #424242)");

    ui->verticalLayout_3->setAlignment(Qt::AlignTop);

    ui->topWidget->setStyleSheet("background-color:#c2c2f2");
    QFont fnt; fnt.setPointSize(10);
    ui->topWidget->setFont(fnt);
    ui->dashboardPushButton->setStyleSheet("QPushButton{color:#494eff;background:url(:/images/pause_nor.png);border:0px;}"
                                                 "QPushButton:hover{background:url(:/images/backblue.png);border:1px}"
                                                 "QPushButton:pressed{background:url(:/images/pause_over.png); position: "
                                           "relative;top: 1px; left: 1px;}");
    ui->transactionsPushButton->setStyleSheet("QPushButton{color:#494eff;background:url(:/images/pause_nor.png);border:0px;}"
                                              "QPushButton:hover{background:url(:/images/backblue.png);border:1px}"
                                              "QPushButton:pressed{background:url(:/images/pause_over.png); position: "
                                        "relative;top: 1px; left: 1px;}");;
    ui->myTasksPushButton->setStyleSheet("QPushButton{color:#494eff;background:url(:/images/pause_nor.png);border:0px;}"
                                         "QPushButton:hover{background:url(:/images/backblue.png);border:1px}"
                                         "QPushButton:pressed{background:url(:/images/pause_over.png); position: "
                                   "relative;top: 1px; left: 1px;}");;
    ui->reportsPushButton->setStyleSheet("QPushButton{color:#494eff;background:url(:/images/pause_nor.png);border:0px;}"
                                         "QPushButton:hover{background:url(:/images/backblue.png);border:1px}"
                                         "QPushButton:pressed{background:url(:/images/pause_over.png); position: "
                                   "relative;top: 1px; left: 1px;}");;
    clearSideBar();
    loadDashBoardSideBar();

    dash = new DefaultDashBoard(this);
    ui->widgetLayout->addWidget(dash);

}

Projects_SideBar::~Projects_SideBar()
{
    delete ui;
}

void Projects_SideBar::on_dashboardPushButton_clicked()
{
    loadDashBoardSideBar();
}

void Projects_SideBar::on_myTasksPushButton_clicked()
{
    loadMytasksSideBar();
}

void Projects_SideBar::on_reportsPushButton_clicked()
{
    loadReportsSideBar();
}

void Projects_SideBar::CreateProjectButtonClicked()
{
    resetColorsDashboard();
    CreateProjectButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    AddProjectWidget = new AddProject(this);
    ui->widgetLayout->addWidget(AddProjectWidget);
    AddProjectWidget->setFocus();
}

void Projects_SideBar::ActiveProjectsClicked()
{
    resetColorsDashboard();
    ActiveProjects->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    ProjectsListWidget = new ProjectWidget(this);
    ui->widgetLayout->addWidget(ProjectsListWidget);
    ProjectsListWidget->setFocus();
}

void Projects_SideBar::QuotesButtonClicked()
{
    resetColorsProjects();
    QuotesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    QuotationsWidget = new Quotations(this);
    ui->widgetLayout->addWidget(QuotationsWidget);
    QuotationsWidget->setFocus();
}

void Projects_SideBar::SalesOrdersButtonClicked()
{
    resetColorsProjects();
    SalesOrdersButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    salesOrdersWidget = new salesOrders(this);
//    ui->widgetLayout->addWidget(salesOrdersWidget);
//    salesOrdersWidget->setFocus();
}

void Projects_SideBar::SalesInvoicesButtonClicked()
{
    resetColorsProjects();
    SalesInvoicesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    SalesVoucherListWidget = new SalesVoucherList(this);
//    ui->widgetLayout->addWidget(SalesVoucherListWidget);
//    SalesVoucherListWidget->setFocus();
}

void Projects_SideBar::ExpensesButtonClicked()
{
    resetColorsProjects();
    ExpensesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    ExpenseWidget = new Payment(this);
//    ExpenseWidget->setHeading("Expenses");
//    ui->widgetLayout->addWidget(ExpenseWidget);
//    ExpenseWidget->setFocus();
}

void Projects_SideBar::DeliveryNotesButtonClicked()
{
    resetColorsProjects();
    DeliveryNotesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    deliveryNotesWidget = new deliveryNotes(this);
//    ui->widgetLayout->addWidget(deliveryNotesWidget);
//    deliveryNotesWidget->setFocus();
}

void Projects_SideBar::ReceiptsButtonClicked()
{
    resetColorsProjects();
//    ReceiptsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    ReceiptEntryWidget =new ReceiptEntry(this);
//    ui->widgetLayout->addWidget(ReceiptEntryWidget);
//    ReceiptEntryWidget->setFocus();
}

void Projects_SideBar::CostEstimatesButtonClicked()
{
    resetColorsProjects();
    CostEstimatesButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");

}

void Projects_SideBar::PaymentsButtonClicked()
{
    resetColorsProjects();
//    PaymentsButton->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    PaymentWidget = new Payment(this);
//    ui->widgetLayout->addWidget(PaymentWidget);
//    PaymentWidget->setFocus();
}

void Projects_SideBar::SalesOrdersPendingClicked()
{
    resetColorsProjects();
//    SalesOrdersPending->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    SalesOrdersPendingWidget = new salesOrders(this);
//    SalesOrdersPendingWidget ->setHeading("Sales Orders Pending");
//    ui->widgetLayout->addWidget(SalesOrdersPendingWidget);
//    SalesOrdersPendingWidget->setFocus();
}

void Projects_SideBar::PendingApprovalsClicked()
{
    resetColorsProjects();
    PendingApprovals->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PendingApprovalsWidget = new PendingApprovalsList(this);
    ui->widgetLayout->addWidget(PendingApprovalsWidget);
}

void Projects_SideBar::RequirementsClicked()
{
    resetColorsProjects();
    Requirements->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    RequirementsListWidget = new RequirementsList(this);
    ui->widgetLayout->addWidget(RequirementsListWidget);
    RequirementsListWidget->setFocus();
}

void Projects_SideBar::DrawingsClicked()
{
    resetColorsProjects();
    Drawings->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    draftsManviewWidget = new draftsManview(this);
    ui->widgetLayout->addWidget(draftsManviewWidget);
}

void Projects_SideBar::DocumentsClicked()
{
    resetColorsProjects();
    Documents->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");

}

void Projects_SideBar::ForemanClicked()
{
    resetColorsProjects();
    Foreman->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
//    WorkOrderViewWidget = new WorkOrderView(this);
//    ui->widgetLayout->addWidget(WorkOrderViewWidget);

}

void Projects_SideBar::HistoryClicked()
{
    resetColorsProjects();
    History->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
    PreviousQuotationsListWidget = new PreviousQuotationsList(this);
    ui->widgetLayout->addWidget(PreviousQuotationsListWidget);
    PreviousQuotationsListWidget->setFocus();
}

void Projects_SideBar::MyTasksClicked()
{
    resetColorsMyTasks();
    MyTasks->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");

}

void Projects_SideBar::WorkSheetClicked()
{
    resetColorsMyTasks();
    WorkSheet->setStyleSheet("color:#ffa02f;Text-align:left;background-color:rgba(255,255,255,0);QToolButton {border: none;}");
}

void Projects_SideBar::clearLayout()
{
    if (ui->widgetLayout != NULL )
    {
        qDebug()<<"clearing Inventory Layout";

        QLayoutItem* item;
        while (( item = ui->widgetLayout->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

}

void Projects_SideBar::clearSideBar()
{
    if (ui->verticalLayout_3 != NULL )
    {

        QLayoutItem* item;
        while (( item = ui->verticalLayout_3->takeAt(0)) != NULL )
        {
            delete item->widget();
            delete item;
        }
    }

}

void Projects_SideBar::resetColorsDashboard()
{

    CreateProjectButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ActiveProjects->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    clearLayout();

}

void Projects_SideBar::resetColorsProjects()
{
    QuotesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    SalesOrdersButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    SalesInvoicesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ExpensesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    DeliveryNotesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    ReceiptsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    CostEstimatesButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PaymentsButton->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    SalesOrdersPending->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    PendingApprovals->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Requirements->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Drawings->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Documents->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    Foreman->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    History->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");

    clearLayout();
}

void Projects_SideBar::resetColorsMyTasks()
{
    MyTasks->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");
    WorkSheet->setStyleSheet("Text-align:left;background-color:rgba(255,255,255,0);color:#f1f1f1;");

}

void Projects_SideBar::resetColorsMyReports()
{

}



void Projects_SideBar::loadDashBoardSideBar()
{
    clearSideBar();

    CreateProjectButton= new QToolButton(this);
    ActiveProjects= new QToolButton(this);

    CreateProjectButton->setText("Add New Project");
    ActiveProjects->setText("Active Projects");

    QFont font;
    font.setPointSize(10);
    CreateProjectButton->setFont(font);
    ActiveProjects->setFont(font);

    CreateProjectButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ActiveProjects->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->verticalLayout_3->addWidget(CreateProjectButton);
    ui->verticalLayout_3->addWidget(ActiveProjects);
    resetColorsDashboard();

    setProjectsUAC();
    QObject::connect(CreateProjectButton,SIGNAL(clicked(bool)),this,SLOT(CreateProjectButtonClicked()));
    QObject::connect(ActiveProjects,SIGNAL(clicked(bool)),this,SLOT(ActiveProjectsClicked()));
}

void Projects_SideBar::loadProjectSideBar()
{
    clearSideBar();

    Requirements= new QToolButton(this);
    QuotesButton= new QToolButton(this);
    SalesOrdersButton= new QToolButton(this);
    DeliveryNotesButton= new QToolButton(this);
    SalesInvoicesButton= new QToolButton(this);
    ExpensesButton= new QToolButton(this);
    ReceiptsButton= new QToolButton(this);
    CostEstimatesButton= new QToolButton(this);
    PaymentsButton= new QToolButton(this);
    SalesOrdersPending= new QToolButton(this);
    PendingApprovals= new QToolButton(this);
    History= new QToolButton(this);
    Drawings= new QToolButton(this);
    Documents= new QToolButton(this);
    Foreman= new QToolButton(this);

    QuotesButton->setText("Quotes");
    SalesOrdersButton->setText("Sales Orders");
    SalesInvoicesButton->setText("Sales Invoices");
    ExpensesButton->setText("Expenses");
    DeliveryNotesButton->setText("Delivery Notes");
    ReceiptsButton->setText("Receipts");
    CostEstimatesButton->setText("Cost Estimates");
    PaymentsButton->setText("Payments");
    SalesOrdersPending->setText("Sales Orders Pending");
    PendingApprovals->setText("Approvals Pending");
    History->setText("History");
    Requirements->setText("Requirements");
    Drawings->setText("Draftsman");
    Documents->setText("Documents");
    Foreman->setText("Work Order");


    ui->verticalLayout_3->addWidget(Requirements);
    ui->verticalLayout_3->addWidget(QuotesButton);
    ui->verticalLayout_3->addWidget(Drawings);
    ui->verticalLayout_3->addWidget(SalesOrdersButton);
    ui->verticalLayout_3->addWidget(Foreman);
    ui->verticalLayout_3->addWidget(DeliveryNotesButton);
    ui->verticalLayout_3->addWidget(SalesInvoicesButton);
    ui->verticalLayout_3->addWidget(ExpensesButton);
    ui->verticalLayout_3->addWidget(PaymentsButton);
    ui->verticalLayout_3->addWidget(ReceiptsButton);
    ui->verticalLayout_3->addWidget(SalesOrdersPending);
    ui->verticalLayout_3->addWidget(PendingApprovals);
    ui->verticalLayout_3->addWidget(Documents);
    ui->verticalLayout_3->addWidget(CostEstimatesButton);
    ui->verticalLayout_3->addWidget(History);

    QFont font;
    font.setPointSize(10);
    QuotesButton->setFont(font);
    SalesOrdersButton->setFont(font);
    SalesInvoicesButton->setFont(font);
    ExpensesButton->setFont(font);
    DeliveryNotesButton->setFont(font);
    ReceiptsButton->setFont(font);
    CostEstimatesButton->setFont(font);
    PaymentsButton->setFont(font);
    SalesOrdersPending->setFont(font);
    PendingApprovals->setFont(font);
    Requirements->setFont(font);
    Drawings->setFont(font);
    Documents->setFont(font);
    Foreman->setFont(font);
    History->setFont(font);
    QuotesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    SalesOrdersButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    SalesInvoicesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ExpensesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    DeliveryNotesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ReceiptsButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    CostEstimatesButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PaymentsButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    SalesOrdersPending->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PendingApprovals->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Requirements->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Drawings->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Documents->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    Foreman->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    History->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    resetColorsProjects();
    QObject::connect(QuotesButton,SIGNAL(clicked(bool)),this,SLOT(QuotesButtonClicked()));
    QObject::connect(SalesOrdersButton,SIGNAL(clicked(bool)),this,SLOT(SalesOrdersButtonClicked()));
    QObject::connect(SalesInvoicesButton,SIGNAL(clicked(bool)),this,SLOT(SalesInvoicesButtonClicked()));
    QObject::connect(ExpensesButton,SIGNAL(clicked(bool)),this,SLOT(ExpensesButtonClicked()));
    QObject::connect(DeliveryNotesButton,SIGNAL(clicked(bool)),this,SLOT(DeliveryNotesButtonClicked()));
    QObject::connect(ReceiptsButton,SIGNAL(clicked(bool)),this,SLOT(ReceiptsButtonClicked()));
    QObject::connect(CostEstimatesButton,SIGNAL(clicked(bool)),this,SLOT(CostEstimatesButtonClicked()));
    QObject::connect(PaymentsButton,SIGNAL(clicked(bool)),this,SLOT(PaymentsButtonClicked()));
    QObject::connect(SalesOrdersPending,SIGNAL(clicked(bool)),this,SLOT(SalesOrdersPendingClicked()));
    QObject::connect(PendingApprovals,SIGNAL(clicked(bool)),this,SLOT(PendingApprovalsClicked()));
    QObject::connect(Requirements,SIGNAL(clicked(bool)),this,SLOT(RequirementsClicked()));
    QObject::connect(Drawings,SIGNAL(clicked(bool)),this,SLOT(DrawingsClicked()));
    QObject::connect(Documents,SIGNAL(clicked(bool)),this,SLOT(DocumentsClicked()));
    QObject::connect(Foreman,SIGNAL(clicked(bool)),this,SLOT(ForemanClicked()));
    QObject::connect(History,SIGNAL(clicked(bool)),this,SLOT(HistoryClicked()));

    setTransactionUAC();

}

void Projects_SideBar::loadMytasksSideBar()
{
    clearSideBar();

    MyTasks= new QToolButton(this);
    WorkSheet= new QToolButton(this);

    MyTasks->setText("My Tasks");
    WorkSheet->setText("TimeSheet");

    QFont font;
    font.setPointSize(10);
    MyTasks->setFont(font);
    WorkSheet->setFont(font);

    MyTasks->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    WorkSheet->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

    ui->verticalLayout_3->addWidget(MyTasks);
    ui->verticalLayout_3->addWidget(WorkSheet);
    resetColorsMyTasks();

    QObject::connect(MyTasks,SIGNAL(clicked(bool)),this,SLOT(MyTasksClicked()));
    QObject::connect(WorkSheet,SIGNAL(clicked(bool)),this,SLOT(WorkSheetClicked()));
}

void Projects_SideBar::loadReportsSideBar()
{
    clearSideBar();
    loadProjectSideBar();
}

void Projects_SideBar::on_transactionsPushButton_clicked()
{
    loadReportsSideBar();
}

void Projects_SideBar::setTransactionUAC()
{
    QuotesButton->setHidden(!LoginValues::model.Quotes_View);;
    SalesOrdersButton->setHidden(!LoginValues::model.Sales_Orders_View);;
    SalesInvoicesButton->setHidden(!LoginValues::model.Sales_Invoices_View);;
    ExpensesButton->setHidden(!LoginValues::model.Payments_View);;
    DeliveryNotesButton->setHidden(!LoginValues::model.Delivery_Notes_View);;
    ReceiptsButton->setHidden(!LoginValues::model.Receipts_View);;
    CostEstimatesButton->setHidden(!LoginValues::model.Payments_View);;
    PaymentsButton->setHidden(!LoginValues::model.Payments_View);;
    SalesOrdersPending->setHidden(!LoginValues::model.Sales_Orders_View);;
    PendingApprovals->setHidden(!LoginValues::model.Finances);;
    Requirements->setHidden(!LoginValues::model.Requirements_View);;
    Drawings->setHidden(!LoginValues::model.DraftsMan_View);;
    Documents->setHidden(!LoginValues::model.Doucuments_View);;
    Foreman->setHidden(!LoginValues::model.WorkOrder_View);;
    History->setHidden(!LoginValues::model.Quotes_Add);;




}

void Projects_SideBar::setProjectsUAC()
{
    CreateProjectButton->setHidden(!LoginValues::model.Projects_Add);
    ActiveProjects->setHidden(!LoginValues::model.Projects_View);;

}

void Projects_SideBar::setReportsUac()
{

}

