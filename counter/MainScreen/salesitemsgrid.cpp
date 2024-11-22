#include "salesitemsgrid.h"
#include <QHeaderView>
#include <customwidgets/Material/components/qtmaterialscrollbar.h>
#include "customwidgets/Delegates/Inventory/possalesitemsdelegate.h"
SalesItemsGrid::SalesItemsGrid(SalesInventoryItemDatabaseHelper *dbHelper,
                               QRect rect,
                               int colCount,
                               int buttonHeight,
                               int buttonWidth,
                               QWidget *parent):
    QWidget(parent)
{

    this->rect = rect;
    this->dbHelper = dbHelper;
    this->colCount = colCount;
    this->buttonWidth = buttonWidth;
    this->buttonHeight = buttonHeight;

    setWidgets();
}

SalesItemsGrid::~SalesItemsGrid()
{

}

void SalesItemsGrid::setGrpID(const QString &value)
{
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    grpID = value;
    if(grpID == "x"){
        model = dbHelper->getInventoryItemsAllFavos();
    }
    else{
        model = dbHelper->getInventoryItemsByGroupModel(grpID);
    }

    tModel = new POSSalesItemsModel(model,colCount);

    table->setModel(tModel);

}

void SalesItemsGrid::setWidgets()
{
    lay = new QVBoxLayout(this);
    setLayout(lay);

    table = new QTableView();

    PosSalesItemsDelegate *delegate = new PosSalesItemsDelegate(buttonWidth,buttonHeight);

    connect(delegate,&PosSalesItemsDelegate::clicked,this,[=](QString val)
    {
        qDebug()<<"X : VAL";
        emit selectSalesItemID(val);
    });

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    //    table->setModel(model);
    table->setItemDelegate(delegate);
    table->verticalHeader()->hide();
    table->horizontalHeader()->hide();
    table->setShowGrid(false);
    table->setObjectName("sss");
    table->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //    table->verticalHeader()->setDefaultSectionSize(buttonHeight);
    //    table->horizontalHeader()->setDefaultSectionSize(buttonWidth);
    QtMaterialScrollBar *bar = new QtMaterialScrollBar;
    bar->setBackgroundColor(Qt::white);
    bar->setHideOnMouseOut(false);

    QtMaterialScrollBar *bar2 = new QtMaterialScrollBar;
    bar2->setBackgroundColor(Qt::white);
    bar2->setHideOnMouseOut(false);
    table->setVerticalScrollBar(bar);
    table->setHorizontalScrollBar(bar2);

    table->setStyleSheet("background-color:transparent");

    //    table->setr(buttonHeight);

    table->setContentsMargins(15,15,15,15);
    table->setStyleSheet("QTableView#sss::item {border: 0px; padding: 10px;"
                         "border-style: outset;border-bottom-color: rgb(0, 150, 136);"
                         "border-top-color: rgb(120, 150, 136);border-left-color:  rgb(120, 150, 136);"
                         "background-color: rgb(255, 255, 255);border-right-color: rgb(0, 150, 136);"
                         "border-radius: 10px;border-width: 1px;}"
                         "QTableView#sss{background-color:transparent}"
                         "");
    lay->addWidget(table);
    table->setFrameStyle(QFrame::NoFrame);

}
