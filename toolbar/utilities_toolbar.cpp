#include "utilities_toolbar.h"
#include "ui_utilities_toolbar.h"
#include <QListWidgetItem>
#include "folder/lineediticon.h"
#include <QLineEdit>
#include <QIcon>
#include <QHBoxLayout>
#include <QLinearGradient>
#include <QCompleter>

Utilities_ToolBar::Utilities_ToolBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Utilities_ToolBar)
{
    myParent = parent;
    ui->setupUi(this);

//    ui->sharePushButton->hide();
    ui->contactsPushButton->hide();
    ui->eventsPushButton->hide();
    ui->userAccountPushButton->hide();

    ui->lineEdit->setStyleSheet("background-color:rgba(255,255,255,255);");
    ui->lineEdit->setPlaceholderText("Search...");
//    ui->lineEdit->hide();
    uiHelper = new UiSettingsDatabaseHelper;
    widModel = uiHelper->getAllScreensForSearchModel();

    widSearch  = new GMWidgetSearchLite();
    widSearch->setStyleSheet("background-color:white");
    ui->searchLay->addWidget(widSearch);
    connect(widSearch, &GMWidgetSearchLite::uiSelected, this, [=](int ui_id, bool configInDb) {
       emit uiSelected(ui_id, configInDb);
        widSearch->setText("");
        widSearch->clear();
        widSearch->clearFocus();

    });
    widSearch->hide();


    searchCompleter = new QCompleter(ui->lineEdit);
    searchCompleter->setModel(widModel);
    searchCompleter->setCompletionMode(QCompleter::PopupCompletion);
    searchCompleter->setFilterMode(Qt::MatchContains);
    searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    searchCompleter->setCompletionColumn(1);
    searchCompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    searchCompleter->popup()->installEventFilter(this);
    connect(searchCompleter, SIGNAL(activated(QString)), this, SLOT(selectUiBySearch(QString)));
    connect(searchCompleter, QOverload<const QString&>::of(&QCompleter::activated), this, [=](const QString& val){
        ui->lineEdit->clear();
        ui->lineEdit->clearFocus();
    }, Qt::QueuedConnection);
//    QObject::connect(searchCompleter, SIGNAL(activated(const QString&)),
//                     ui->lineEdit, SLOT(clear()),
//                     Qt::QueuedConnection);


    ui->lineEdit->setCompleter(searchCompleter);

//    ui->sharePushButton->setToolTip("Share");
//    ui->sharePushButton->setToolTipDuration(2000);

    ui->contactsPushButton->setToolTip("Contacts");
    ui->eventsPushButton->setToolTip("Events");
    ui->notificationsPushButton->setToolTip("Notification");
    ui->userAccountPushButton->setToolTip("User Profile");

    ui->contactsPushButton->setToolTipDuration(2000);
    ui->eventsPushButton->setToolTipDuration(2000);
    ui->notificationsPushButton->setToolTipDuration(2000);
    ui->userAccountPushButton->setToolTipDuration(2000);


    dbSwitchButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "domain"));
    dbSwitchButton->setParent(this);
    dbSwitchButton->setColor(QColor(255,255,255));
    dbSwitchButton->setIconSize(QSize(30,30));



    QObject::connect(dbSwitchButton,&QtMaterialIconButton::clicked,
                     this, [=](bool Checked){
        emit companySwitchClicked();
    });

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(255,255,255));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked, this, &Utilities_ToolBar::showShareOptions);

    QtMaterialIconButton *reportButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "report"));
    reportButton->setParent(this);
    reportButton->setColor(QColor(255,255,255));
    reportButton->setIconSize(QSize(30,30));
    reportButton->hide();

    QFile inputFile2("debugdev.txt");
    if (inputFile2.open(QIODevice::ReadOnly))
    {
        reportButton->show();
    }

    connect(reportButton, &QtMaterialIconButton::clicked,
            this, [=](bool checked){

            UiController *uiCtrl = new UiController();
            bool ok;
            QString text = QInputDialog::getText(this, tr("Enter UI ID"),
                                                 tr("ID : "), QLineEdit::Normal,
                                                 "0", &ok);
            qDebug()<<"report:"<<text;
            if (ok && !text.isEmpty())
            {
                if(uiCtrl->setUiData(text.toInt(), true)){
                    qDebug()<<"opening";
//                    if(text.toInt() == 3)
//                        uiCtrl->setDefaultFilters("f1", GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher));
                    uiCtrl->reportWid->showMaximized();
                    qDebug()<<"showed";
                }
            }
    });

    ui->dbSwitchLayout->addWidget(shareButton);
    ui->dbSwitchLayout->addWidget(dbSwitchButton);
    ui->dbSwitchLayout->addWidget(reportButton);

    ui->lineEdit->setFocus();



}

Utilities_ToolBar::~Utilities_ToolBar()
{
    delete ui;
}

void Utilities_ToolBar::setCompanyLabel(QString companyName)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<companyName;
    ui->companyLabel->setStyleSheet("QLabel{color:#ffffff}");
    ui->companyLabel->setText("Selected company: " + companyName);
}

void Utilities_ToolBar::on_eventsPushButton_clicked()
{

}

void Utilities_ToolBar::on_contactsPushButton_clicked()
{
    AdressBook_DisplayWidget = new AdressBook_Display(this);
    AdressBook_DisplayWidget->setWindowFlags(Qt::Window);
    AdressBook_DisplayWidget->show();
}

void Utilities_ToolBar::on_lineEdit_textChanged(const QString &arg1)
{
    emit searchText(arg1);
//    emit uiSelected(widModel->record(searchCompleter->currentRow()).value(0).toInt());
}

void Utilities_ToolBar::on_notificationsPushButton_clicked()
{
    emit uiSelected(1005, false);
}

void Utilities_ToolBar::selectUiBySearch(QString uiName)
{
    qDebug()<<Q_FUNC_INFO<<ui->lineEdit->text();
    ui->lineEdit->setText("");
    qDebug()<<Q_FUNC_INFO<<ui->lineEdit->text();

    QSqlQueryModel *model = uiHelper->getScreenIdByName(uiName);

//    if(uiId == 0)
        emit uiSelected(model->record(0).value(0).toInt(), model->record(0).value(1).toBool());
//    else {
//        emit uiSelected(uiId, uiName);
//    }
}


void Utilities_ToolBar::showShareOptions()
{
 //    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
//    shareOptionsWidget->show();

//    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
//                     this,&ReportWidget::printReport);
//    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
//                     this,&ReportWidget::printReport);
//    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
    //                     this,&ReportWidget::exportToExcel);
}

//bool Utilities_ToolBar::eventFilter(QObject *watched, QEvent *event)
//{
//    QAbstractItemView* view = qobject_cast<QAbstractItemView*>(watched);
//    if (event->type() == QEvent::KeyPress)
//    {
//       QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
//       if (keyEvent->key() == Qt::Key_Return ||
//           keyEvent->key() == Qt::Key_Enter)
//       {
//          ui->lineEdit->clear();
//          view->hide();
//          return true;
//       }
//    }
//    return false;
//}
