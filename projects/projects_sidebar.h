#ifndef PROJECTS_SIDEBAR_H
#define PROJECTS_SIDEBAR_H

#include <QWidget>
#include "customwidgets/defaultdashboard.h"
#include "customwidgets/section.h"
#include <QDebug>
#include <QToolButton>
#include "projects/projectwidget.h"
#include "projects/addproject.h"
#include "login/loginvalues.h"

#include "projects/quotations/quotations.h"
#include "projects/Draftsman/draftsmanview.h"
#include "projects/PendingApprovals/pendingapprovalslist.h"


#include "projects/History/previousquotationslist.h"
//#include "inventory/transactions/salesorders/salesorders.h"
//#include "inventory/transactions/deliverynote/deliverynotes.h"
//#include "finance/transactions/payments/payment.h"
//#include "finance/transactions/receipts/receiptentry.h"
#include "projects/requirements/requirementslist.h"
//#include "finance/transactions/sales/salesvoucherlist.h"

namespace Ui {
class Projects_SideBar;
}

class Projects_SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit Projects_SideBar(QWidget *parent = 0);
    ~Projects_SideBar();
    DefaultDashBoard *dash;

    QToolButton *CreateProjectButton;
    QToolButton *ActiveProjects;
    QToolButton *QuotesButton;
    QToolButton *SalesOrdersButton;
    QToolButton *SalesInvoicesButton;
    QToolButton *ExpensesButton;
    QToolButton *DeliveryNotesButton;
    QToolButton *ReceiptsButton;
    QToolButton *CostEstimatesButton;
    QToolButton *PaymentsButton;
    QToolButton *SalesOrdersPending;
    QToolButton *PendingApprovals;
    QToolButton *Requirements;
    QToolButton *Drawings;
    QToolButton *Documents;
    QToolButton *Foreman;
    QToolButton *History;

    QToolButton *MyTasks;
    QToolButton *WorkSheet;


    AddProject* AddProjectWidget;
    ProjectWidget* ProjectsListWidget;

    Quotations* QuotationsWidget;
//    salesOrders* salesOrdersWidget;
//    deliveryNotes* deliveryNotesWidget;
//    Payment* PaymentWidget;
//    Payment* ExpenseWidget;
//    salesOrders* SalesOrdersPendingWidget;
//    ReceiptEntry* ReceiptEntryWidget;
    RequirementsList* RequirementsListWidget;
//    SalesVoucherList* SalesVoucherListWidget;

    draftsManview* draftsManviewWidget;
//    WorkOrderView* WorkOrderViewWidget;
    PendingApprovalsList* PendingApprovalsWidget;

    PreviousQuotationsList* PreviousQuotationsListWidget;

    QString projectID;



private slots:

    void on_dashboardPushButton_clicked();

    void on_myTasksPushButton_clicked();

    void on_reportsPushButton_clicked();

    void CreateProjectButtonClicked();
    void ActiveProjectsClicked();
    void QuotesButtonClicked();
    void SalesOrdersButtonClicked();
    void SalesInvoicesButtonClicked();
    void ExpensesButtonClicked();
    void DeliveryNotesButtonClicked();
    void ReceiptsButtonClicked();
    void CostEstimatesButtonClicked();
    void PaymentsButtonClicked();
    void SalesOrdersPendingClicked();
    void PendingApprovalsClicked();
    void RequirementsClicked();
    void DrawingsClicked();
    void DocumentsClicked();
    void ForemanClicked();
    void HistoryClicked();

    void MyTasksClicked();
    void WorkSheetClicked();



    void clearLayout();

    void clearSideBar();

    void resetColorsDashboard();
    void resetColorsProjects();
    void resetColorsMyTasks();
    void resetColorsMyReports();

    void loadDashBoardSideBar();
    void loadProjectSideBar();
    void loadMytasksSideBar();
    void loadReportsSideBar();

    void on_transactionsPushButton_clicked();

    void setTransactionUAC();
    void setProjectsUAC();
    void setReportsUac();


private:
    Ui::Projects_SideBar *ui;
};

#endif // PROJECTS_SIDEBAR_H
