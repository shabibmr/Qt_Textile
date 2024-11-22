#include "reportwidget.h"

#include <QtMath>
#include "finance/transactions/BillwiseRecords/billwisemappingeditor.h"
#include <inventory/masters/sales_inventory_items/updateplulisttextfileclass.h>

ReportWidget::ReportWidget(DatabaseHelper *db, QString reportTitle, QWidget *parent) :
    QWidget(parent)
{
    this->reportName = reportTitle;
    this->db = db;

    this->setWindowTitle(reportName);
    this->setContentsMargins(0,0,0,0);
    vLayout = new QVBoxLayout;
    this->setLayout(vLayout);
    this->setObjectName("report");
    this->setStyleSheet("QWidget#report{background-color:" + bgcolor + "}");
    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor(bgcolor));
    setAutoFillBackground(true);
    setPalette(pal);
    //    resetWidgets();
    initDbHelpers();
    initGlobalObjects();
    setWidgets();

    installEventFilter(this);
    setFocus();
}

ReportWidget::~ReportWidget()
{
}

void ReportWidget::setFilterMap(const QMap<QString, QPair<QString, QString> > &value)
{
    filterMap = value;
}

void ReportWidget::setDbName(const QString &value)
{
    dbName = value;
}

void ReportWidget::setPermissions(UiAccessControlDataModel *value)
{
    permissions = value;

    if(!permissions->allowShare)
        shareButton->hide();
}

void ReportWidget::setUiModel(UiSettingsDataModel *value, bool switchTab)
{
    qDebug()<<"Calling setUI model for UI ID"<<value->ui_id;
    uiModel = value;
    isTabChange = switchTab;

    //    delete tableView;
    //    initTableView();
    //    tableLayout->addWidget(tableView);

    setUiValues();

}

void ReportWidget::setUiValues()
{
    reportName = uiModel->ui_name;
    this->setWindowTitle(reportName);
    windowLabel->setText(reportName);
    clearLayout(filterLayout);

    for(QJsonValue filter:uiModel->filters["filter by"].toArray()){
        if(filter["var-value"].toString() == "query")
            addQueryFilter(filter);
        else
            addFilter(filter["var-value"].toString(),
                    filter["var-name"].toString(),
                    filter["var-filter"].toString());
    }

    for(QJsonValue dateVar:uiModel->filters["dateTime"].toArray()){
        setDateFilter(dateVar.toString(), uiModel->filters["useTimeStamp"].toBool());
    }

    qDebug()<<"ui properties"<<reportName<<uiModel->totalCols<<uiModel->hideCols;

    if(permissions->allowCreate){
        if(uiModel->ui_type == "Registers")
            setRegisterLayout();
        else if(uiModel->ui_type == "Masters")
            setMasterLayout();
    }

    if(uiModel->ui_type == "Masters"){
        okButton->hide();
        tableView->lastRow->hide();
    }


    setAdditionalWidgets();
    setExtrasLayout();
    setTabs();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void ReportWidget::keyPressEvent(QKeyEvent *e)
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<e->key()<<Qt::Key_N<<e->modifiers().testFlag(Qt::ControlModifier);
    if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))
            && (uiModel->ui_type == "Registers" ))
    {
        createNewVoucher();
    }
    else if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))
             && (uiModel->ui_type == "Masters" ))
    {
        openMasterEditor();
    }
    else if ((e->key() == Qt::Key_W)  && (e->modifiers().testFlag(Qt::ControlModifier)))
    {
        this->closeWindow();
    }
    else if ((e->key() == Qt::Key_F)  && (e->modifiers().testFlag(Qt::ControlModifier)))
    {
        filterLineEdit->setFocus();
    }

    switch (e->key ()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:{
        redirectUi( tableView->currentIndex());
        break;
    }
    case Qt::Key_F5 :
    {
        setTableView();
        break;
    }
        //    case Qt::Key_F1 :{
        //        editQtyByKeyboard();
        //        break;
        //    }
        //    case  Qt::Key_F2 :{
        //        int row = ui->tableWidget->currentRow();
        //        openItemDetail(row);
        //        break;
        //    }
        //    case Qt::Key_Plus:{
        //        on_plusToolButton_clicked();
        //        break;
        //    }
        //    case Qt::Key_Minus:{
        //        on_minusToolButton_clicked();
        //        break;
        //    }
        //    case Qt::Key_Asterisk:{
        //        on_itemDeleteToolButton_clicked();
        //        break;
        //    }
        //    case Qt::Key_Escape:{
        //        this->closeWindow();
        //        break;
        //    }
    default:
        QWidget::keyPressEvent (e);
    }


}

void ReportWidget::addFilter(QString filterType,
                             QString filterVar,
                             QString searchFilter,
                             QString filterQuery)
{
    if(filterType == "group")
    {
        QSqlQueryModel* searchmodel = invGrpHelper->getSalesInventoryGroupDataAsModel();
        groupSearch = new GMItemSearchLite(searchmodel);
        groupSearch->setGroupFlag(true);
        groupSearch->setText("");
        groupSearch->setPlaceholderText("Search Group...");
        groupSearch->setFont(boldFont);
        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        grpLabel = new QLabel("Item Group");
        grpLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        //        lay->setContentsMargins(0,0,0,0);
        //        lay->setSpacing(0);
        lay->addWidget(grpLabel);
        grpLabel->hide();
        lay->addWidget(groupSearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMItemSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type Inventory Group");
        SearchWid->setFixedWidth(200);

        QObject::connect(groupSearch,&GMItemSearchLite::SelectedGroup,
                         this,[=](QString groupID){
            grpLabel->show();
            //            if()
            setFilterValue(filterVar,groupID, filterType);
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"0"});
    }

    else if(filterType == "item")
    {

        QSqlQueryModel* searchmodel = itemHelper->getInventoryItemsQueryModelForSearch(searchFilter.toInt());
        itemSearch = new GMItemSearchLite(searchmodel);
        itemSearch->setText("");
        itemSearch->setPlaceholderText("Search Item...");
        itemSearch->setFont(boldFont);
        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        itemLabel = new QLabel("Item");
        itemLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(itemLabel);
        itemLabel->hide();
        lay->addWidget(itemSearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMItemSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type Inventory Item");
        SearchWid->setFixedWidth(200);

        QObject::connect(itemSearch,&GMItemSearchLite::SelectedItemID,
                         this,[=](inventoryItemDataModel itemObj){
            itemLabel->show();
            itemSearch->setText(itemObj.ItemName);
            setFilterValue(filterVar,itemObj.ItemID, filterType);
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar,{filterType,"0"});
    }

    else if(filterType == "account_group")
    {

        QSqlQueryModel* searchmodel = accHelper->getAllAccountForSearchModel();
        accSearch = new GMLedgerSearchLite(searchmodel);
        accSearch->setText("");
        accSearch->setPlaceholderText("Search Account Group...");
        accSearch->setAccountsFlag(true);
        accSearch->setFont(boldFont);
        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        accountLabel = new QLabel("Account Group");
        accountLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(accountLabel);
        accountLabel->hide();
        lay->addWidget(accSearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMLedgerSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type Account");
        SearchWid->setFixedWidth(200);

        QObject::connect(accSearch,&GMLedgerSearchLite::SelectedAccount,
                         this,[=](AccountGroupDataModel accObj){
            accountLabel->show();
            setFilterValue(filterVar,accObj.groupID, filterType);
            setTableView();
            accGroupId = accObj.groupID;
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"0"});
    }


    else if(filterType == "ledger")
    {

        QStringList grpList = QStringList();
        if(searchFilter.length() > 0){
            grpList = searchFilter.split("|");
        }
        ledSearch= new GMLedgerSearchLite(ledHelper->getLedgerQueryString(grpList));
        ledSearch->setParent(this);
        ledSearch->setPlaceholderText("Search Ledger...");
        ledSearch->setStyleSheet("GMLedgerSearchLite {color : " + textColor + "; background-color : " + bgcolor + ";}");
        ledSearch->setFixedWidth(500);
        ledSearch->setFont(boldFont);
        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        ledgerLabel = new QLabel("Ledger");
        ledgerLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(ledgerLabel);
        ledgerLabel->hide();
        lay->addWidget(ledSearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMLedgerSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type Ledger");
        SearchWid->setFixedWidth(500);

        connect(ledSearch,&GMLedgerSearchLite::selectedLedger,
                this,[=](LedgerMasterDataModel led){
            ledgerLabel->show();
            setFilterValue(filterVar,led.LedgerID, filterType);
            ledgerName = led.LedgerName;
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"0"});
    }

    else if(filterType == "godown")
    {

        godownSearch= new GMGodownSearchLite(godownHelper->getGodownForSearchModel());
        godownSearch->setParent(this);
        godownSearch->setText("");
        godownSearch->setPlaceholderText("Search Godown...");
        godownSearch->setStyleSheet("GMGodownLineMaterial {color : " + textColor + "; background-color : " + bgcolor + ";}");
        godownSearch->setFixedWidth(200);
        godownSearch->setFont(boldFont);

        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        godownLabel = new QLabel("Godown");
        godownLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(godownLabel);
        godownLabel->hide();
        lay->addWidget(godownSearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMGodownSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type Godown");
        SearchWid->setFixedWidth(200);
        connect(godownSearch,&GMGodownSearchLite::SelectedGodown,
                this,[=](GodownDataObject godown){
            godownLabel->show();
            setFilterValue(filterVar,godown.GodownID, filterType);
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar,{filterType, "%"});
    }

    else if(filterType == "employee")
    {

        employeesearch = new GMEmployeeSearchLite(empHelper, empHelper->getEmployeeQueryForSearch());
        employeesearch->setText("");
        employeesearch->setPlaceholderText("Search Salesman...");
        employeesearch->setParent(this);
        employeesearch->setStyleSheet("GMEmployeeLineMaterial {color : " + textColor + "; background-color : " + bgcolor + ";}");
        employeesearch->setFixedWidth(200);
        employeesearch->setFont(boldFont);

        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        employeeLabel = new QLabel("Salesman");
        employeeLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(employeeLabel);
        employeeLabel->hide();
        lay->addWidget(employeesearch);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("GMEmployeeSearchLite {color : " + textColor + "}");
        SearchWid->setWhatsThis("Salesman");
        SearchWid->setFixedWidth(200);

        connect(employeesearch,&GMEmployeeSearchLite::SelectedEmpID,
                this,[=](UserProfileDataModel emp){
            employeeLabel->show();
            setFilterValue(filterVar,QString::number(emp._id), filterType);
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"0"});
    }

    else if(filterType == "priceList")
    {

        priceLists = priceHelper->getPriceListAsMap();

        priceListCombo = new QComboBox();
        priceListCombo->addItem("Select ");
        priceListCombo->addItems(priceLists.values());
        priceListCombo->setFont(boldFont);


        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        pricelistLabel = new QLabel("Price List");
        pricelistLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(pricelistLabel);
        pricelistLabel->hide();
        lay->addWidget(priceListCombo);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("QComboBox {color : " + textColor + "; border: none; } QFrame { border: none; }"
                                 + "QComboBox::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
                                 + "QComboBox::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }"
                                 );
        SearchWid->setWhatsThis("Price List");
        SearchWid->setFixedWidth(200);

        connect(priceListCombo,&QComboBox::currentTextChanged,
                this,[=](QString val){
            pricelistLabel->show();
            setFilterValue(filterVar,priceLists.key(val), filterType);
            setTableView();
        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"%"});
    }

//    else if(filterType == "route")
//    {

//        routes = routeHelper->getAllRouteAsMap();
//        qDebug()<<Q_FUNC_INFO<<__LINE__<<routes;

//        routeCombo = new QComboBox();
//        routeCombo->addItem("Select ");
//        routeCombo->addItems(routes.values());
//        routeCombo->setFont(boldFont);


//        QWidget *SearchWid = new QWidget(this);
//        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
//        routeLabel = new QLabel("Route");
//        routeLabel->setStyleSheet("QLabel{color:" + textColor + "}");
//        lay->addWidget(routeLabel);
//        //        routeLabel->hide();
//        lay->addWidget(routeCombo);
//        SearchWid->setLayout(lay);
//        SearchWid->setStyleSheet("QComboBox {color : " + textColor + "; border: none; } QFrame { border: none; }"
//                                 + "QComboBox::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
//                                 + "QComboBox::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }"
//                                 );
//        SearchWid->setWhatsThis("Route");
//        SearchWid->setFixedWidth(200);

//        connect(routeCombo,&QComboBox::currentTextChanged,
//                this,[=](QString val){

//            routeLabel->show();
//            setFilterValue(filterVar,routes.key(val), filterType);
//            setTableView();
//        });

//        filterLayout->addWidget(SearchWid);
//        filterMap.insert(filterVar, {filterType,"%"});
//        qDebug()<<Q_FUNC_INFO<<filterMap;
//    }


    else if(filterType == "voucherType")
    {

        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesVoucher);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::SalesOrder);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::itemwiseOrder);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseVoucher);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::CreditNote);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::DebitNote);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::DeliveryNote);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptNote);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::DefectiveVoucher);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::StockEntry);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::StockJournal);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::GodownTransfer);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::MaterialIn);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::MaterialOut);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::WorkOrder);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::IndentRequest);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::IndentIssue);
        voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::Quotation);

        if(uiModel->sub_type == "Finance"){
            voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::PaymentVoucher);
            voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::ReceiptVoucher);
            voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::ContraVoucher);
            voucherTypes<<GMVoucherTypes::getVoucherString(GMVoucherTypes::JournalVoucher);
        }
        //        qDebug()<<voucherTypes;

        voucherTypeCombo = new QComboBox();
        voucherTypeCombo->addItem("Select Voucher Type");
        voucherTypeCombo->addItems(voucherTypes);
        voucherTypeCombo->setFont(boldFont);
        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        voucherTypeLabel = new QLabel("Voucher Type");
        voucherTypeLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(voucherTypeLabel);
        //        Label->hide();
        lay->addWidget(voucherTypeCombo);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("QComboBox {color : " + textColor + "; border: none; } QFrame { border: none; }"
                                 + "QComboBox::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
                                 + "QComboBox::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }"
                                 );
        SearchWid->setWhatsThis("Select Voucher Type");
        SearchWid->setFixedWidth(200);

        connect(voucherTypeCombo, &QComboBox::currentTextChanged, this, [=](QString val){
            //            Label->show();
            if(voucherTypeCombo->currentIndex() == 0)
                setFilterValue(filterVar, "%", filterType);
            else
                setFilterValue(filterVar,val, filterType);
            setTableView();

        });

        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar, {filterType,"%"});
    }
    else
    {

        QLineEdit *lineEdit= new QLineEdit();
        lineEdit->setParent(this);
        lineEdit->setText("");
        lineEdit->setPlaceholderText("Enter " + filterType);
        lineEdit->setStyleSheet("QLineEdit {color : " + textColor + "; background-color : " + bgcolor + "; border : none}");
        lineEdit->setFixedWidth(200);

        QWidget *SearchWid = new QWidget(this);
        QVBoxLayout *lay = new QVBoxLayout(SearchWid);
        QLabel *textEditLabel = new QLabel(filterType.toUpper());
        textEditLabel->setStyleSheet("QLabel{color:" + textColor + "}");
        lay->addWidget(textEditLabel);
        //        textEditLabel->hide();
        lay->addWidget(lineEdit);
        SearchWid->setLayout(lay);
        SearchWid->setStyleSheet("QLineEdit {color : " + textColor + "}");
        SearchWid->setWhatsThis("Type ");
        SearchWid->setFixedWidth(200);

        connect(lineEdit,&QLineEdit::editingFinished, this,[=](){
            QString value = lineEdit->text();
            qDebug()<<"353 entered text:"<<value;
            textEditLabel->show();
            setFilterValue(filterVar,value, filterType);
            setTableView();
        });
        filterLayout->addWidget(SearchWid);
        filterMap.insert(filterVar,{filterType,""});
    }

}

void ReportWidget::addQueryFilter(QJsonValue filter)
{
    if(filter["var-type"].toString() == "flag")
    {
        QCheckBox *flagBox = new QCheckBox();
        flagBox->setText(filter["display-text"].toString());
        flagBox->setFont(normalFont);
        flagBox->setChecked(filter["default-state"].toBool());
        if(filter["default-state"].toBool())
            filterMap.insert(filter["var-name"].toString(),{"query", filter["queryString"].toString()});
        else
            filterMap.insert(filter["var-name"].toString(), {"query", filter["default"].toString()});
        //        flagBox->setStyleSheet("QCheckBox{color : " + textColor + "} "
        //                               + "QCheckBox::indicator {border: 1px solid " +textColor +"}"
        //                               + "QCheckBox::indicator:unchecked { image: url(:/images/checkbox_unchecked.png);}"
        //                               +"QCheckBox::indicator:unchecked:hover {image: url(:/images/checkbox_unchecked_hover.png);}"
        //                               + "QCheckBox::indicator:unchecked:pressed {image: url(:/images/checkbox_unchecked_pressed.png);}"
        //                               + "QCheckBox::indicator:checked {image: url(:/icons/icons/navigation/svg/production/ic_check_24px.svg);}"
        //                               + "QCheckBox::indicator:checked:hover {image: url(:/images/checkbox_checked_hover.png);}"
        //                               + "QCheckBox::indicator:checked:pressed {image: url(:/images/checkbox_checked_pressed.png);}");
        QPalette boxPalette = flagBox->palette();
        boxPalette.setColor(flagBox->backgroundRole(), QColor(textColor));
        boxPalette.setColor(flagBox->foregroundRole(), QColor(textColor));
        boxPalette.setColor(QPalette::Button, QColor(textColor));
        flagBox->setPalette(boxPalette);
        //        flagBox->

        connect(flagBox, &QCheckBox::stateChanged, this, [=] (int checked){
            if(checked == 2){
                setFilterValue(filter["var-name"].toString(),filter["queryString"].toString(), "query");
            }
            else {
                setFilterValue(filter["var-name"].toString(), filter["default"].toString(), "query");
            }
        });

        filterLayout->addWidget(flagBox);
    }
}

void ReportWidget::setDateFilter(QString dateVar, bool useTimeStamp)
{
    //    qDebug()<<Q_FUNC_INFO;
    bool showDateFilters = ConfigurationSettingsDatabaseHelper::getValue(showDateFiltersInReports, true).toBool();

    if(dateVar == "fromDateTime"){
        fromDateEdit->setDate(fromDate);
        QDateTime fromDateTime = QDateTime(fromDate);
        fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));
        if(useTimeStamp)
            filterMap.insert(dateVar,{"fromDateTime", fromDateTime.toString("yyyy-MM-dd hh:mm:ss")});
        else
            filterMap.insert(dateVar, {"fromDateTime", fromDate.toString("yyyy-MM-dd")});
        if(showDateFilters)
            fromDateWidget->show();
        connect(fromDateEdit,&GMDateEdit::editFinished, this, [=](){
            QDateTime fromDateTime = QDateTime(fromDate);
            fromDateTime.setTime(QTime(CounterSettingsDataModel::StartTime));
            if(useTimeStamp)
                setFilterValue(dateVar,fromDateTime.toString("yyyy-MM-dd hh:mm:ss"), "fromDateTime");
            else
                setFilterValue(dateVar,fromDate.toString("yyyy-MM-dd"), "fromDateTime");
        });
    }

    if(dateVar == "toDateTime"){
        toDateEdit->setDate(toDate);
        //            toDate = toDate.addDays(CounterSettingsDataModel::EndDay);
        QDateTime toDateTime = QDateTime(toDate.addDays(CounterSettingsDataModel::EndDay));
        toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
        if(useTimeStamp)
            filterMap.insert(dateVar,{"toDateTime", toDateTime.toString("yyyy-MM-dd hh:mm:ss")});
        else
            filterMap.insert(dateVar, {"toDateTime", toDate.toString("yyyy-MM-dd")});
        if(showDateFilters)
            toDateWidget->show();
        connect(toDateEdit,&GMDateEdit::editFinished, this, [=](){
            //                toDate = toDate.addDays(CounterSettingsDataModel::EndDay);
            QDateTime toDateTime = QDateTime(toDate.addDays(CounterSettingsDataModel::EndDay));
            toDateTime.setTime(QTime(CounterSettingsDataModel::EndTime));
            if(useTimeStamp)
                setFilterValue(dateVar,toDateTime.toString("yyyy-MM-dd hh:mm:ss"), "toDateTime");
            else
                setFilterValue(dateVar,toDate.toString("yyyy-MM-dd"), "toDateTime");
        });
    }
}

void ReportWidget::setAdditionalWidgets()
{
    clearLayout(additionalLay1);
    clearLayout(additionalLay2);

    //    qDebug()<<Q_FUNC_INFO<<uiModel->displayOptions["additional"];
    for(QJsonValue widget:uiModel->displayOptions["additional"].toArray()){
        qDebug()<<"wudget"<<widget["widget-name"].toString();

        if(widget["widget-type"].toString() == "QLabel"){
            //            qDebug()<<"label"<<widget["widget-name"];
            QLabel *label = new QLabel(widget["widget-name"].toString());

            label->setStyleSheet("QLabel{color:" + textColor + "}");
            label->setFont(normalFont);
            QLabel *valueLabel = new QLabel();
            valueLabel->setStyleSheet("QLabel{color:" + textColor + "}");
            valueLabel->setFont(boldFont);

            if(widget["value-from"].toString() == "query"){
                QString qStr = widget["query"].toString();
                QMapIterator<QString,QPair<QString, QString>> i(filterMap);
                while(i.hasNext()){
                    i.next();
                    qStr = qStr.replace("$" + i.key(),i.value().second);

                }

                qStr = qStr.replace("$trans_db", transDb);
                qStr = qStr.replace("$master_db", masterDb);
                QString text = DatabaseHelper::getSingletonQueryResult(qStr).toString();
                qDebug()<<Q_FUNC_INFO<<__LINE__<<widget["widget-name"].toString()<<text<<qStr;
                if(widget["widget-name"].toString() == "Opening Balance")
                    openingBalance = text.toFloat();
                else if(widget["widget-name"].toString() == "Closing Balance")
                    closingBalance = text.toFloat();
                if(widget["value-type"].toString() =="amount"){
                    if(text.toFloat() < 0)
                        text = QString::number( qFabs(text.toFloat()), 'f',2) + " Dr";
                    else
                        text += " Cr";

                }

                valueLabel->setText(text);
            }

            if(widget["layout"].toString() == "additionalLay1"){
                //                qDebug()<<"add layout"<<widget["layout"];
                if(widget["alignment"].toString() == "right")
                    additionalLay1->addStretch(1);
                additionalLay1->addWidget(label );
                additionalLay1->addWidget(valueLabel);
                if(widget["alignment"].toString() == "left")
                    additionalLay1->addStretch(1);
            }

            if(widget["layout"].toString() == "additionalLay2"){
                //                qDebug()<<"add layout"<<widget["layout"];
                if(widget["alignment"].toString() == "right")
                    additionalLay2->addStretch(1);
                additionalLay2->addWidget(label);
                additionalLay2->addWidget(valueLabel);
                if(widget["alignment"].toString() == "left")
                    additionalLay2->addStretch(1);
            }
        }
    }

}

void ReportWidget::setFilterValue(QString varName, QString value, QString filterType)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    qDebug()<<"current filter map before change"<<filterMap;
    qDebug()<<"adding"<<varName<<value<<filterType;
    filterMap[varName].second = value;
    resetWidgets();
}

void ReportWidget::resetWidgets()
{
    filterLineEdit->clear();

    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<filterMap.size();
    QMapIterator<QString,QPair<QString, QString>> filter(filterMap);
    while(filter.hasNext()){
        filter.next();
        QString filtertype = filter.value().first;
        QString value = filter.value().second;

        qDebug()<<Q_FUNC_INFO<<__LINE__<<filtertype<<value;

        if(filtertype == "group"){
            //   groupSearch->itemSelected(value, invGrpHelper->getGroupNameByID(value));
            groupSearch->setText(invGrpHelper->getGroupNameByID(value));
            grpLabel->show();
        }
        else if(filtertype == "item"){
            // itemSearch->itemSelected(value, itemHelper->getInventoryItemNameByID(value));
            itemSearch->setText( itemHelper->getInventoryItemNameByID(value));
            itemLabel->show();

        }
        else if(filtertype == "ledger"){
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<"set ledger"<<ledHelper->getLedgerNameByID(value);
            //            ledSearch->itemSelected(value, ledHelper->getLedgerNameByID(value));
            ledSearch->setText( ledHelper->getLedgerNameByID(value));
            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<"current"<<ledSearch->text();
            ledgerLabel->show();
        }

        else if(filtertype == "account_group"){
            accSearch->setText( accHelper->getGroupNameByID(value));
            accountLabel->show();
        }
        else if(filtertype == "godown"){
            godownSearch->setText( godownHelper->getGodownNameById(value));
            godownLabel->show();
        }
        else if(filtertype == "employee"){
            //            qDebug()<<"abcd";
            //            empSearch->setText(empHelper->getEmployeeNameFromID(value.toInt()));
            //            qDebug()<<"xyz";
            employeeLabel->show();
            //            qDebug()<<"12345";
        }
        else if(filtertype == "priceList"){
            qDebug()<<Q_FUNC_INFO<<__LINE__<<priceHelper->getPriceListNameByID(value);
            if(value != "%")
                priceListCombo->setCurrentText(priceHelper->getPriceListNameByID(value));
            pricelistLabel->show();
        }
//        else if(filtertype == "route"){
//            //            qDebug()<<Q_FUNC_INFO<<__LINE__<<priceHelper->getPriceListNameByID(value);
//            if(value != "%")
//                routeCombo->setCurrentText(routeHelper->getRouteNameByID(value));
//            routeLabel->show();
//        }
        else if(filtertype == "voucherType"){
            if(value != "%")
                voucherTypeCombo->setCurrentText(GMVoucherTypes::getVoucherString(GMVoucherTypes::getVoucherIdFromString(value)));
            voucherTypeLabel->show();
        }
        else if(filtertype == "fromDateTime"){
            fromDateEdit->setDate(QDate::fromString(value));
        }
        else if(filtertype == "toDateTime"){
            toDateEdit->setDate(QDate::fromString(value));
        }
        else if(filtertype == "query"){

        }
        else {
            //            lineEdit->setText(value);
        }
    }
}

void ReportWidget::initGlobalObjects()
{

    dateEditSS = "GMDateEdit{background-color:" + bgcolor + ";border: none; color: " + textColor + "} "
            + "QDateEdit::drop-down {border-style: solid; subcontrol-origin:margin; spacing: 5px; width:15px;height:10px; subcontrol-position: right center;}"
            + "QDateEdit::down-arrow {image: url(:/icons/icons/hardware/svg/production/ic_keyboard_arrow_down_24px.svg);height:15px;width:15px; }";

    initTableView();



    fromDateEdit = new GMDateEdit(&fromDate);
    fromDateEdit->setFixedWidth(80);
    fromDateEdit->setFrame(false);
    fromDateEdit->setStyleSheet(dateEditSS);
    fromDateEdit->setFont(normalFont);

    QLabel *fromDateLabel = new QLabel("From");
    fromDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *fromDateLay = new QVBoxLayout;
    fromDateLay->addWidget(fromDateLabel);
    fromDateLay->addWidget(fromDateEdit);

    fromDateWidget = new QWidget;
    fromDateWidget->setLayout(fromDateLay);
    fromDateWidget->hide();

    toDateEdit = new GMDateEdit(&toDate);
    toDateEdit->setFixedWidth(80);
    toDateEdit->setFrame(false);
    toDateEdit->setStyleSheet(dateEditSS);
    toDateEdit->setFont(normalFont);


    QLabel *toDateLabel = new QLabel("To");
    toDateLabel->setStyleSheet("QLabel{color:" + textColor + "}");

    QVBoxLayout *toDateLay = new QVBoxLayout;
    toDateLay->addWidget(toDateLabel);
    toDateLay->addWidget(toDateEdit);

    toDateWidget = new QWidget;
    toDateWidget->setLayout(toDateLay);
    toDateWidget->hide();

    backButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "arrow_back"));
    backButton->setParent(this);
    backButton->setColor(QColor(iconColor));
    backButton->setIconSize(QSize(30,30));
    //    backButton->hide();
    QObject::connect(backButton,&QtMaterialIconButton::clicked, this, &ReportWidget::goToPrevious);

    shareButton = new QtMaterialIconButton(QtMaterialTheme::icon("social", "share"));
    shareButton->setParent(this);
    shareButton->setColor(QColor(iconColor));
    shareButton->setIconSize(QSize(30,30));
    QObject::connect(shareButton,&QPushButton::clicked, this, &ReportWidget::showShareOptions);

    okButton = new QtMaterialIconButton(QtMaterialTheme::icon("navigation", "refresh"));
    okButton->setParent(this);
    okButton->setColor(QColor(iconColor));
    connect(okButton, &QPushButton::clicked, this, &ReportWidget::setTableView);

    filterLineEdit = new QLineEdit;
    filterLineEdit->setFrame(false);
    filterLineEdit->setPlaceholderText("Search.. ");


    connect(filterLineEdit, &QLineEdit::textChanged, this, [=](){

        QString split =filterLineEdit->text();//.replace(QRegularExpression("(.{1})"), "\\1 ");

        sort->setStr(filterLineEdit->text());
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<filterLineEdit->text();
        QRegExp regExp( split,Qt::CaseInsensitive );
        //        regExp.setPatternSyntax(QRegExp::Wildcard);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<regExp<<split;

        sort->setFilterRegExp(regExp);
        tableView->setColumns();
    });

    filterLayout = new QHBoxLayout;
    registerLayout = new QHBoxLayout;
    tabLayout = new QHBoxLayout;

    tabs = new QtMaterialTabs();
    tabs->setBackgroundColor(QColor("white"));
    tabs->setTextColor(QColor("#00bcd4"));
    tabs->setFixedHeight(50);
    tabLayout->addWidget(tabs);
    tabs->hide();
}

void ReportWidget::initDbHelpers()
{
    itemHelper = new SalesInventoryItemDatabaseHelper(db);
    invGrpHelper = new SalesInventoryGroupsDatabaseHelper(db);
    ledHelper = new LedgerMasterDatabaseHelper(db);
    accHelper = new AccountGroupMasterDatabaseHelper(db);
    godownHelper = new GodownDatabaseHelper(db);
    empHelper = new UserProfileDatabaseHelper(db);
    priceHelper = new PricelistDatabaseHelper(db);
//    routeHelper = new RouteDatabaseHelper(db);
}

void ReportWidget::initTableView()
{
    tableView = new ReportsTableView("");
    tableView->setItemDelegate(new ReportCommonDelegate());
//    tableView->verticalHeader()->show();
    tableView->setWordWrap(true);
    sort = new GMReportSortFilterProxyModel();
    connect(tableView,&QTableView::doubleClicked, this, &ReportWidget::redirectUi);


}

void ReportWidget::setWidgets()
{
    windowLabel = new QLabel(reportName);
    appBar = new QtMaterialAppBar();
    appBar->setBackgroundColor(QColor(appBarColor));

    QPalette palette = windowLabel->palette();
    palette.setColor(windowLabel->foregroundRole(), QColor(titleColor));
    palette.setColor(windowLabel->backgroundRole(), QColor(appBarColor));
    windowLabel->setPalette(palette);
    windowLabel->setFont(titleFont);

    QVBoxLayout *layout = new QVBoxLayout;
    QWidget *canvas = new QWidget;

    appBar->setFixedHeight(50);
    appBar->appBarLayout()->addWidget(backButton);
    //    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addWidget(windowLabel, 0, Qt::AlignCenter);
    appBar->appBarLayout()->addStretch(1);
    appBar->appBarLayout()->addLayout(registerLayout);
    appBar->appBarLayout()->addWidget(shareButton);


    layout->setContentsMargins(0, 0, 0, 0);

    layout = new QVBoxLayout;
    canvas->setLayout(layout);
    canvas->setMaximumHeight(60);
    layout->addWidget(appBar);

    QPointer<QWidget> Widget = new QWidget(this);
    QPointer<QVBoxLayout> mainLayout = new QVBoxLayout(Widget);
    Widget->setLayout(mainLayout);
    mainLayout->setContentsMargins(0,0,0,0);


    QHBoxLayout *fLayout = new QHBoxLayout;
    fLayout->setContentsMargins(0,0,0,0);
    extrasLayout = new QHBoxLayout;
    extrasLayout->setContentsMargins(0,0,0,0);
    fLayout->addWidget(fromDateWidget);
    fLayout->addWidget(toDateWidget);

    fLayout->addLayout(filterLayout);
    fLayout->addWidget(okButton);
    fLayout->addStretch(1);
    fLayout->addLayout(extrasLayout);
    fLayout->addWidget(filterLineEdit);

    additionalLay1 = new QHBoxLayout;
    additionalLay2 = new QHBoxLayout;
    diffLay = new QHBoxLayout;


    tableLayout = new QVBoxLayout;
    tableLayout->setContentsMargins(0,0,0,0);

    tableLayout->addWidget(tableView);

    mainLayout->addWidget(appBar);
    mainLayout->addLayout(fLayout);
    //    mainLayout->addLayout(tabLayout);
    mainLayout->addLayout(additionalLay1);
    mainLayout->addLayout(tableLayout);
    mainLayout->addLayout(diffLay);
    mainLayout->addLayout(additionalLay2);

    Widget->setStyleSheet("QWidget{background-color:" + bgcolor + "}");

    vLayout->addWidget(Widget);

}

void ReportWidget::goToPrevious()
{
    qDebug()<<"previous called******************"<<uiList.size();
    if(uiList.size()>1){
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"removing current"<<uiList.last().first.ui_id<<uiList.last().second;
        uiList.removeLast();
        qDebug()<<Q_FUNC_INFO<<__LINE__<<uiList.size()<<uiModel->ui_id<<uiList.last().first.ui_id;
        //        if(uiModel->ui_id != uiList.last().first.ui_id){
        delete tableView;
        initTableView();
        tableLayout->addWidget(tableView);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<"refresh";
        setUiModel(&uiList.last().first);
        //        }
        filterMap = uiList.last().second;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<filterMap;
        qDebug()<<Q_FUNC_INFO<<__LINE__<<uiModel->avlCols<<uiModel->hideCols<<uiModel->stretchCol;
        setTableView();
        resetWidgets();
        emit setTabTitle(uiModel->ui_name);
    }
    else{
        this->close();
        emit closing();
    }
}

void ReportWidget::setTableView()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__<<uiModel->ui_name;

    QPair<UiSettingsDataModel , QMap<QString, QPair<QString, QString>> > uiState;
    uiState.first = *uiModel;
    qDebug()<<Q_FUNC_INFO<<__LINE__<<uiState.first.avlCols;
    uiState.second = filterMap;

    if(uiList.size()==0 )
        uiList.append(uiState);
    else if (uiModel->ui_id != uiList.last().first.ui_id ||
             filterMap != uiList.last().second) // store only ui by id, no need of last filter
        uiList.append(uiState);

    QString qStr = uiModel->query;

    qStr = prepareQuery(qStr);

    //    qDebug()<<"get data";
    QFile inputFile2("debugdev.txt");
    //    if (inputFile2.open(QIODevice::ReadOnly))
    //    {
    //        QMessageBox box; box.setText("Running query : " + qStr); box.setTextInteractionFlags(Qt::TextSelectableByMouse); box.exec();
    //        box.setFocus();
    //    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
    model = db->getQueryModel(qStr);
    qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

    qDebug()<<Q_FUNC_INFO<<__LINE__<<model->rowCount();
    if(model->columnCount() > 0){
        if(uiModel->ui_id == 14){
            LedgerReportDelegate *delegate = new LedgerReportDelegate();
//            connect(delegate, &LedgerReportDelegate::voucherSelected, this, &ReportWidget::showMappingsOfVoucher);
            tableView->setItemDelegate(delegate);
            //            tableView->resizeColumnToContents(4);
        }

        else if(uiModel->ui_id == 115){
            tableView->setItemDelegate(new QStyledItemDelegate);
            //            tableView->resizeColumnToContents(4);
        }

        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

        //        if(uiModel->ui_type == "Registers"  || model->record(0).value(0).toString() == "V"){
        //            qDebug()<<Q_FUNC_INFO<<__LINE__;
        //            int col=5;
        //            if(model->record(0).value(0).toString() == "V"){
        //                for(int i=0; i<model->columnCount(); i++){
        //                    if(model->record().fieldName(i) == "Voucher No"){
        //                        col = i;
        //                        break;
        //                    }
        //                }
        //            }
        //            qDebug()<<Q_FUNC_INFO<<"voucherNo col is at"<<col;
        //            tableView->setItemDelegate(new RegisterDelegate(col));
        //        }

        sort->setAvlCols(uiModel->avlCols);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

        sort->setSourceModel(model);

        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime()<<sort->sourceModel()->rowCount();
        //        qDebug()<<"get totals"<<uiModel->totalCols;
        tableView->setAvlcols(uiModel->totalCols);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
//        tableView->setDiffcols(uiModel->diffCols);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
        //        qDebug()<<"sort";
//        if(uiModel->ui_id == 115 || uiModel->ui_id == 14)
//            tableView->setModel(model);
//        else
            tableView->setModel(sort);
        bool reportSortEnabledFlag = ConfigurationSettingsDatabaseHelper::getValue(reportSortEnabled, true).toBool();
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();
        tableView->setSortingEnabled(reportSortEnabledFlag);
        tableView->setModelQry(model);


        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

        //        qDebug()<<"set columns";
        tableView->setColumns();
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

        for(int col:uiModel->hideCols){
            qDebug()<<"hide"<<col;
            tableView->hideColumn(col);
        }

        qDebug()<<"stretch"<<uiModel->stretchCol;
        if(uiModel->stretchCol > -1 && !uiModel->hideCols.contains(uiModel->stretchCol))
            tableView->horizontalHeader()->setSectionResizeMode(uiModel->stretchCol, QHeaderView::Stretch);
        else{
            qDebug()<<uiModel->displayOptions["scroll"].toBool()<<Q_FUNC_INFO<<__LINE__;
            if(!uiModel->displayOptions["scroll"].toBool())
                tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        }
//        tableView->setVisible(false);
//        tableView->verticalHeader()->setVisible(true);
//        tableView->setVisible(true);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<QDateTime::currentDateTime();

//        tableView->setVisible(false);
//        tableView->resizeColumnsToContents();
//        tableView->setVisible(true);

        //        qDebug()<<"tableView"<<"800";
        if(uiModel->displayOptions["additional"].toArray().size() > 0)
            setAdditionalWidgets();

//        if(uiModel->diffCols.size() > 0)
//        {
//            qDebug()<<Q_FUNC_INFO<<__LINE__<<tableView->diffValue;
//            clearLayout(diffLay);
//            QLabel *label = new QLabel("Difference");

//            label->setStyleSheet("QLabel{color:" + textColor + "}");
//            label->setFont(normalFont);
//            QString valText = QString::number(qFabs(tableView->diffValue), 'f', 2);
//            if(tableView->diffValue > 0)
//                valText += " Dr";
//            else {
//                valText += " Cr";
//            }
//            QLabel *valueLabel = new QLabel(valText);
//            valueLabel->setStyleSheet("QLabel{color:" + textColor + "}");
//            valueLabel->setFont(boldFont);

//            diffLay->addStretch(1);
//            diffLay->addWidget(label);
//            diffLay->addWidget(valueLabel);
//        }

        //        tableView->setFocus();
    }

    else{
        if (inputFile2.open(QIODevice::ReadOnly))
        {
            showMessageBox("Query Error");
        }
    }

}

void ReportWidget::exportToExcel()
{

    QString dateNow = QDate::currentDate().toString("dd_MMM_yyyy");

    QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Excel file"),
                                                    dir+"/"+reportName+"_"+dateNow+".xlsx",
                                                    tr("Excel Files (*.xls)")
                                                    );

    qDebug()<<"File Name :"<<fileName;
    if (fileName.isEmpty())
    {
        return;
    }


    ExcelReportHelper *ex = new ExcelReportHelper;
    ex->GenerateReportFromTableView(fileName,reportName,
                                    uiModel->widthList,tableView,
                                    fromDate.toString("yyyy-MM-dd"),
                                    toDate.toString("yyyy-MM-dd"),
                                    uiModel->avlCols);

    QDesktopServices::openUrl("file:///" + fileName);
}

void ReportWidget::closeWindow()
{
    this->close();
    emit closing();
}

void ReportWidget::showShareOptions()
{
    shareOptionsWidget = new ShareTools(uiModel->ui_name, shareButton);
    shareOptionsWidget->setWindowFlags(Qt::FramelessWindowHint| Qt::Popup|Qt::WindowStaysOnTopHint);;
    shareOptionsWidget->show();

    QObject::connect(shareOptionsWidget,&ShareTools::printClicked,
                     this,&ReportWidget::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::pdfClicked,
                     this,&ReportWidget::printReport);
    QObject::connect(shareOptionsWidget,&ShareTools::excelClicked,
                     this,&ReportWidget::exportToExcel);
}

void ReportWidget::printReport(QPrinter *printer)
{
    int val = sender()->property("PrintType").toInt();
    qDebug()<<Q_FUNC_INFO<<__LINE__<<val;
    printer->setPrinterName("printerName");
    printerHelper = new CounterPrinterHelper;
    printerHelper->setPrinter(printer);



    QStringList extraTop;
    QStringList extraBottom;
    if(uiModel->ui_type != "Masters"){
        for(int i=0; i< model->columnCount(); i++){
            QString total = tableView->lastRow->item(0,i)->text();
            qDebug()<<Q_FUNC_INFO<<__LINE__<<total;
            extraBottom<<total;
        }
    }

    qDebug()<<Q_FUNC_INFO<<__LINE__<<extraBottom;


    if(reportName =="Ledger Report")
        printerHelper->printLedgerSimple(ledgerName,
                                         tableView,
                                         fromDate,
                                         toDate,
                                         openingBalance,
                                         closingBalance
                                         );
    else
        printerHelper-> printReportGeneral(tableView
                                          ,uiModel->widthList,reportName,
                                          fromDate,toDate,uiModel->AlignmentList
                                          ,extraTop,extraBottom,uiModel->avlCols);

}

void ReportWidget::clearLayout(QLayout *layout) {
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void ReportWidget::redirectUi(QModelIndex index)
{
    qDebug()<<"table doubleclicked";
    int row = sort->mapToSource(index).row();
//    if(uiModel->ui_id == 14)
//        row = index.row();
    QString ID = model->record(row).value(1).toString();
    QString rowType = model->record(row).value(0).toString();
    qDebug()<<"row type"<<rowType<<ID;


    if(uiModel->ui_type == "Masters"){
        //        qDebug()<<"redirect master"<<rowType<<ID;
        emit editMaster(rowType, ID);
    }

    else if(rowType=="G"){

        groupSearch->itemSelected(ID, invGrpHelper->getGroupNameByID(ID));
        //        groupSearch->setText(invGrpHelper->getGroupNameByID(ID));
        //        setFilterValue("group", ID);
        //  setTableView();
    }
    else if(rowType=="A"){

        accSearch->itemSelected(ID, accHelper->getGroupNameByID(ID));
        //        groupSearch->setText(invGrpHelper->getGroupNameByID(ID));
        //        setFilterValue("group", ID);
        //  setTableView();
    }
    else if(rowType =="V"){

        qDebug()<<model->record(row).value(1).toString()<<model->record(row).value(2).toString()<<model->record(row).value(3).toString()<<__LINE__;
        VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions,this);
        connect(vEditor, &VoucherEditorSelector::refresh, this, &ReportWidget::setTableView);
        connect(vEditor, &VoucherEditorSelector::saveCompleted, this, [=](){
            qDebug()<<Q_FUNC_INFO<<__LINE__;
            emit voucherSaved();
        });
        qDebug()<<"UI MODEL :"<<uiModel->sub_type;
        if(uiModel->sub_type != model->record(row).value(3).toString()){
            UserGroupDatabaseHelper *userGroupHelper = new UserGroupDatabaseHelper(db);
            vEditor->setPermissions(userGroupHelper->getPermissionsByUserGroupAndVoucherType(
                                        LoginValues::model._id,
                                        model->record(row).value(3).toString()));
        }
        vEditor->openEditor(model->record(row).value(1).toString(),
                            model->record(row).value(2).toString(),
                            model->record(row).value(3).toString());
    }

    else
    {
        //        qDebug()<<"go to "<<rowType<<ID;
        //        qDebug()<<uiModel->redirect_to;
        qDebug()<<Q_FUNC_INFO<<__LINE__;
        emit redirecting(row);
        //            this->hide();
    }
}

void ReportWidget::setTabs()
{
    //    qDebug()<<Q_FUNC_INFO<<uiModel->ui_id;
    tabs->show();
    tabMap.clear();
    //    qDebug()<<"941"<<tabMap.size();
    //    qDebug()<<"926"<<uiModel->displayOptions["tabs"]<<uiModel->displayOptions["tabs"].toArray().size();
    if(uiModel->displayOptions["tabs"].toArray().size()>0){


        QJsonArray redirectArray = uiModel->redirect_to["redirect-to"].toArray();
        for(QJsonValue val:redirectArray){
            QJsonArray tabList = val["tabs"].toArray();

            for(QJsonValue tab:tabList){
                //                qDebug()<<"inserting to map"<<tab["tab-pos"].toInt()<<tab["id"].toInt();
                tabMap.insert(tab["tab-pos"].toInt(), tab["id"].toInt())     ;
            }

            //            qDebug()<<"current map "<<tabMap;
        }


        //        qDebug()<<uiModel->displayOptions["tabs"].toArray();

        for(QJsonValue widget:uiModel->displayOptions["tabs"].toArray()){
            //            qDebug()<<"in loop for "<<widget;
            if(!isTabChange){
                tabs->addTab(widget["tab-name"].toString());
            }
            if(widget["current-tab"].toInt() == 1){
                //                qDebug()<<"changin tab to "<<widget["tab-pos"].toInt();
                tabs->setCurrentTab(widget["tab-pos"].toInt());
            }

        }

        connect(tabs, &QtMaterialTabs::currentChanged, this, [=](int index){
            //            qDebug()<<"966"<<tabMap[index]<<tabMap;
            if(uiModel->ui_id != tabMap[index])
                emit tabChanged(tabMap[index]);
        });
    }


}

void ReportWidget::createNewVoucher()
{
    VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
    vEditor->createNewVoucher(uiModel->sub_type);

    connect(vEditor, &VoucherEditorSelector::refresh, this, &ReportWidget::setTableView);
}

void ReportWidget::openMasterEditor()
{
    //    MastersEditor *vEditor = new MastersEditor();
    //    vEditor->openEditor(uiModel->sub_type);
    emit editMaster(uiModel->sub_type, "");

    //    connect(vEditor, &VoucherEditorSelector::refresh, this, &ReportWidget::setTableView);
}

void ReportWidget::setRegisterLayout()
{
    newButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "add"));
    newButton->setParent(this);
    newButton->setColor(QColor(iconColor));
    newButton->setIconSize(QSize(30,30));
    //    backButton->hide();
    QObject::connect(newButton,&QtMaterialIconButton::clicked, this, &ReportWidget::createNewVoucher);

    registerLayout->addWidget(newButton);

    //    if(!permissions->allowCreate)
    //        newButton->hide();
}

void ReportWidget::setExtrasLayout()
{
    //    qDebug()<<Q_FUNC_INFO<<__LINE__<<uiModel->extras;

    clearLayout(extrasLayout);
    for(QJsonValue extra:uiModel->extras){
        //        qDebug()<<Q_FUNC_INFO<<__LINE__<<extra;
        QJsonObject details = extra.toObject();
        if(details["widget-type"].toString() == "QtMaterialFlatButton"){
            QtMaterialFlatButton *button = new QtMaterialFlatButton(details["text"].toString());
            button->setForegroundColor(QColor(iconColor));
            connect(button, &QtMaterialFlatButton::clicked, this,[=](){
                callExtrasSlot(details["action"].toString(), details["col-pos"].toObject(), details["query"].toString());
            });
            extrasLayout->addWidget(button, 0, Qt::AlignRight);

        }
    }
}

void ReportWidget::clearUiStates()
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    uiList.clear();
    qDebug()<<Q_FUNC_INFO<<__LINE__;
}

void ReportWidget::callExtrasSlot(QString action, QJsonObject columns, QString query)
{
    if(action == "ExportToPurchaseOrder"){
        QList<CompoundItemDataObject> items;


        for(int i=0;i<model->rowCount();i++){
            QString itemid = model->record(i).value(columns["itemID"].toInt()).toString();
            CompoundItemDataObject item;
            item.BaseItem = itemHelper->getInventoryItemByID(itemid);
            //        item.BaseItem.quantity = item.BaseItem.moq;
            item.BaseItem.quantity = model->record(i).value(columns["stock"].toInt()).toFloat() - model->record(i).value(columns["reqTotal"].toInt()).toFloat();
            items.append(item);
        }

        GeneralVoucherDataObject *voucher = new GeneralVoucherDataObject;
        voucher->InventoryItems = items;

        VoucherEditorSelector *vEditor = new VoucherEditorSelector(permissions);
        connect(vEditor, &VoucherEditorSelector::refresh, this, &ReportWidget::setTableView);
        UserGroupDatabaseHelper  *userGroupHelper = new UserGroupDatabaseHelper(db);
        vEditor->setPermissions(userGroupHelper->getPermissionsByUserGroupAndVoucherType(
                                    LoginValues::model._id,
                                    GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder)));
        vEditor->exportVoucher(voucher, GMVoucherTypes::getVoucherString(GMVoucherTypes::PurchaseOrder));

    }

    else if(action == "printList"){

        QSqlQueryModel *model = db->getQueryModel(prepareQuery(query));

        if(model->rowCount() > 1){

            QPrinter *printer = new QPrinter;
            printer->setPrinterName("printerName");
            printerHelper = new CounterPrinterHelper;
            printerHelper->setPrinter(printer);



            QStringList extraTop;
            QStringList extraBottom;

            float total =0;
            for(int i=0; i< model->rowCount(); i++)
                total += model->record(i).value(4).toFloat();

            extraBottom<< "Total: " <<QString::number(total,'f',2);
            QList<int> avlCols; avlCols<<0<<1<<2<<3<<4<<5;
            QList<int> widthList; widthList <<20<<20<<40<<20<<20<<20;
            QList<int> AlignmentList; AlignmentList<< 0<< 1 <<0 << 0 <<1<<0;

            printerHelper-> printReportGeneralwithModel(model
                                                        ,widthList,reportName,fromDate,toDate,AlignmentList
                                                        ,extraTop,extraBottom,avlCols);
        }

        else {

            showMessageBox("No Data Found");
        }

    }

    else if(action == "PendingBills"){
        //        AddBillwiseRecord *billwiseWidget = new AddBillwiseRecord() ;
        //        billwiseWidget->setWindowFlags(Qt::Window);
        //        billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
        //        connect(billwiseWidget, &AddBillwiseRecord::closing, this, &ReportWidget::setTableView);
        //        billwiseWidget->setLedger(ledHelper->getLedgerPtrById
        //                                  (ledHelper->getLedgerIDByName(ledSearch->text())));
        //        billwiseWidget->showPendingBills();

        //        billwiseWidget->show();

        BillwiseMappingEditor *billwiseWidget = new BillwiseMappingEditor(ledHelper->getLedgerPtrById
                                                                          (ledHelper->getLedgerIDByName(ledSearch->text())));
        billwiseWidget->showClearDues();
        //        billwiseWidget->setWindowFlag(Qt::Popup);
        billwiseWidget->setAttribute(Qt::WA_DeleteOnClose);
        billwiseWidget->show();
    }

    else if(action == "printAllLedgers"){
        LedgerHelper *ledgerHelper = new LedgerHelper;

        qDebug()<<Q_FUNC_INFO<<__LINE__<<accGroupId;

        QStringList ledGroups; ledGroups<<accGroupId;

        for(AccountGroupDataModel acc: accHelper->getAccountsUnderParentGroupRecursive(accGroupId)){
            ledGroups.append(acc.groupID);
        }

        QSqlQueryModel *ledModel = ledHelper->getLedgerQueryString(ledGroups);
        qDebug()<<Q_FUNC_INFO<<__LINE__<<ledModel->rowCount();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,"","Printing " + QString::number(ledModel->rowCount()) + " ledgers. Continue? ",
                                      QMessageBox::Yes|QMessageBox::No);
        if(reply==QMessageBox::Yes){

            QPrinter *printer = new QPrinter(QPrinter::ScreenResolution) ;
            QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
            QString fileName = dir+"/" + "Report.pdf";
            printer->setOutputFormat(QPrinter::PdfFormat);
            fileName = QFileDialog::getSaveFileName(this,"File Name",fileName,"*.pdf");
            printer->setOutputFileName(fileName);
            this->setProperty("PrintType",4);
            printerHelper = new CounterPrinterHelper;
            printerHelper->setPrinter(printer);
            for(int i=0; i<ledModel->rowCount(); i++){
                qDebug()<<Q_FUNC_INFO<<__LINE__<<"printing ledger"<<ledModel->record(i).value(1).toString();
                QString ledId = ledModel->record(i).value(0).toString();
                QString ledgerName = ledModel->record(i).value(1).toString();


                QSqlQueryModel *model1 = ledgerHelper->getLedgerReportModel(ledId,
                                                                            fromDate,
                                                                            toDate);
                float openingBalance = ledgerHelper->getOpeningBalanceOfLedger(ledId, fromDate);
                float closingBalance = ledgerHelper->getOpeningBalanceOfLedger(ledId, toDate.addDays(1));

//                printerHelper->addLedgerHtml(ledgerName,
//                                             model1,
//                                             fromDate,
//                                             toDate,
//                                             openingBalance,
//                                             closingBalance
//                                             );

            }

//            printerHelper->printLedgerSimpleByModel();

        }
        else{

        }



        delete ledgerHelper;

    }


}

QString ReportWidget::escapeChar(QString inStr)
{
    return inStr;

    QString outStr = inStr;

    outStr.replace("-", "\\-");

    return outStr;


}

void ReportWidget::showMessageBox(QString text)
{
    QMessageBox box; box.setText(text); box.setTextInteractionFlags(Qt::TextSelectableByMouse); box.exec();
    box.setFocus();

}

QString ReportWidget::prepareQuery(QString qStr)
{
    qDebug()<<filterMap<<qStr;
    QMapIterator<QString,QPair<QString, QString>> i(filterMap);
    while(i.hasNext()){
        i.next();
        //        qDebug()<<"setting filter"<<i.key()<<i.value();
        qStr = qStr.replace("$" + i.key(),i.value().second);

    }

    qStr = qStr.replace("$trans_db", transDb);
    qStr = qStr.replace("$master_db", masterDb);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    return qStr;

}

QSqlQueryModel *ReportWidget::prepareQueryModel(QString qStr)
{
    QMapIterator<QString,QPair<QString, QString>> i(filterMap);
    while(i.hasNext()){
        i.next();
        //        qDebug()<<"setting filter"<<i.key()<<i.value();
        qStr = qStr.replace("$" + i.key(),i.value().second);

    }

    qStr = qStr.replace("$trans_db", transDb);
    qStr = qStr.replace("$master_db", masterDb);

    qDebug()<<Q_FUNC_INFO<<__LINE__<<qStr;

    return db->getQueryModel(qStr);

}

void ReportWidget::showMappingsOfVoucher(QString vNo, QString vType, QString vPrefix)
{
//    MappedBillsByVoucher *mappingsWid = new MappedBillsByVoucher(vNo, vType, vPrefix, this);
//    //    mappingsWid->show();
//    mappingsWid->setWidgets();

}

void ReportWidget::showEvent(QShowEvent *event)
{
    emit showWidget();
    QWidget::showEvent(event);

}

void ReportWidget::setMasterLayout()
{
    if(uiModel->ui_name == "Inventory Items"){
        // QtMaterialFlatButton *button = new QtMaterialFlatButton("Print QR",this);
        // button->setForegroundColor(QColor(iconColor));
        // connect(button, &QtMaterialFlatButton::clicked, this,[=](){
        //     ItemsQRPrintListWidget *widget = new ItemsQRPrintListWidget(this);
        //     widget->setWindowFlags(Qt::Window);
        //     widget->showMaximized();

        // });
        // registerLayout->addWidget(button);

        QtMaterialFlatButton *plubutton = new QtMaterialFlatButton("Upload PLU",this);
        plubutton->setForegroundColor(QColor(iconColor));
        connect(plubutton, &QtMaterialFlatButton::clicked, this,[=](){
            UpdatePLUListTextFileClass *plu = new UpdatePLUListTextFileClass();
            plu->UpdateFile();

        });

        registerLayout->addWidget(plubutton);


    }
    newButton = new QtMaterialIconButton(QtMaterialTheme::icon("content", "add"));
    newButton->setParent(this);
    newButton->setColor(QColor(iconColor));
    newButton->setIconSize(QSize(30,30));
    //    backButton->hide();
    QObject::connect(newButton,&QtMaterialIconButton::clicked, this, &ReportWidget::openMasterEditor);

    registerLayout->addWidget(newButton);
    //    if(!permissions->allowCreate)
    //        newButton->hide();


}
