#include "assignsalesman.h"
#include "ui_assignsalesman.h"
#include "datamodels/projects/Quotation/quotationstatus.h"
AssignSalesman::AssignSalesman(GeneralVoucherDataObject *v,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AssignSalesman)
{
    ui->setupUi(this);
    this->v=v;
    dbHelper = new UserProfileDatabaseHelper();
    sHelper = new SalesOrderDatabaseHelper();
    emps = dbHelper->getAllUsers(groupName);
    qDebug()<<"Assign Salesman Contruct";
    this->setObjectName("POSMain");

    this->setStyleSheet("QWidget#POSMain{background-color:#e0f7fa}");
}

AssignSalesman::~AssignSalesman()
{
    delete ui;
}

void AssignSalesman::setGroupName(const QString &value)
{
    groupName = value;
    //    qDebug()<<"Group name"<<groupName;
}

void AssignSalesman::setTable()
{
    if(ui->layout!=NULL){
        QLayoutItem* item;
        while((item=ui->layout->takeAt(0))!= NULL){
            delete item->widget();
            delete item;
        }
    }
    //    qDebug()<< "setting Emp Table";


    QSignalMapper* signalMapper = new QSignalMapper(this);

    QWidget *client= new QWidget(this);

    QGridLayout *gLayout = new QGridLayout(client);
    //QLayoutItem item = new QLayoutItem;
    QFont tfont;
    QFont sfont;

    tfont.setPointSize(12);
    sfont.setPointSize(8);

    if(v->status == QuotationStatus::SalesOrderDeliveryCreated){
        QSqlQueryModel *m = sHelper->getEmployeesBookedBetween(v->DeliveryDate,
                                                                  v->CustomerExpectingDate
                                                                  );
        for(int i =0;i<m->rowCount();i++){
            avlEmps<<m->record(i).value(0).toInt();
//            emps.remove(m->record(i).value(0).toInt());
        }

    }

    QObject::connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(empSelected(int)));
    int cnt =0;
    QMapIterator<int, QString> i(emps);
    int ids=0;

    while (i.hasNext()) {
        i.next();
        int id = i.key();
        if(v->status == QuotationStatus::SalesOrderDeliveryCreated){
            if(!avlEmps.contains(id)){
                continue;
            }
        }
        ids=id;
        QString name = i.value();
        //        qDebug()<<id<<name;
        QtButton* button = new QtButton(this);
        //        button->setText(name);
        button->setProperty("id",id);

        empsShort.insert(QString(cnt+65),id);
        QVBoxLayout *blay = new QVBoxLayout();

        QLabel *tbname = new QLabel(this);

        QLabel *shortCut = new QLabel(this);

        tbname->setText(name);
        shortCut->setText("("+QString(cnt+65)+")");

        tbname->setFont(tfont);
        shortCut->setFont(sfont);
        shortCut->setAlignment(Qt::AlignRight);



        blay->addWidget(tbname);
        blay->addWidget(shortCut);
        button->setLayout(blay);


        button->setFixedSize(QSize(120,80));
        signalMapper->setMapping(button, id);
        connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));

        gLayout->addWidget(button,cnt/6,cnt%6);
        cnt++;
    }

    client->setLayout(gLayout);

    client->setObjectName("POSMain");
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setObjectName("POSMain");
    scroll->verticalScrollBar()->
        setStyleSheet(QString::fromUtf8("QScrollBar:vertical {"
                                        "    border: 1px solid #999999;"
                                        "    background:white;"
                                        "    width:10px;    "
                                        "    margin: 0px 0px 0px 0px;"
                                        "}"
                                        "QScrollBar::handle:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    min-height: 0px;"
                                        ""
                                        "}"
                                        "QScrollBar::add-line:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    height: px;"
                                        "    subcontrol-position: bottom;"
                                        "    subcontrol-origin: margin;"
                                        "}"
                                        "QScrollBar::sub-line:vertical {"
                                        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
                                        "    stop: 0  rgb(0,255,0), stop: 0.5 rgb(0,255,0),  stop:1 rgb(0,255,0));"
                                        "    height: 0px;"
                                        "    subcontrol-position: top;"
                                        "    subcontrol-origin: margin;"
                                        "}"
                                        ""));

    scroll->setWidget(client);
    scroll->setAlignment(Qt::AlignCenter);
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(scroll);
    //vLayout->addWidget(new QLabel("ITEMS"));
    ui->layout->addLayout(vLayout);
    qDebug()<<"Set all Emps";

    if(cnt==1)
        empSelected(ids);


}

void AssignSalesman::empSelected(int empID)
{
    qDebug()<<"Closing waiter";
    qDebug()<<Q_FUNC_INFO<<__LINE__;
    this->close();
    emit selectEmployeeID(empID);

}

void AssignSalesman::on_toolButton_clicked()
{
    this->close();
}


void AssignSalesman::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape){
        this->close();
    }
    int selected = e->key();
    empSelected(empsShort.value(QString(selected)));


    //    default:
    //        AssignSalesman::keyPressEvent (e);

}
