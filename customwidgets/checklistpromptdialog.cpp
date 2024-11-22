#include "checklistpromptdialog.h"
#include "ui_checklistpromptdialog.h"
#include <QScreen>

ChecklistPromptDialog::ChecklistPromptDialog(GeneralVoucherDataObject *obj, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChecklistPromptDialog)
{
    ui->setupUi(this);
    voucher = obj;


    this->setObjectName("checklist");
    this->setStyleSheet("QDialog#checklist{background-color:#ffffff}");
    this->setWindowTitle("Confirm Save");
    this->setFixedSize(200,150);
    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
                                          qApp->primaryScreen()->geometry()));

    QPalette pal = palette();

    pal.setColor(QPalette::Background,QColor("#ffffff"));
    setAutoFillBackground(true);
    setPalette(pal);

    QtMaterialFlatButton *checkList = new QtMaterialFlatButton("Show Checklist");
    checkList->setIcon(QtMaterialTheme::icon("action", "list"));
    checkList->setForegroundColor(QColor("#00bcd4"));
    checkList->setFontSize(10);
    checkList->setIconSize(QSize(20,20));
    checkList->setTextAlignment(Qt::AlignLeft);


    QtMaterialFlatButton *cancel = new QtMaterialFlatButton("Cancel");

    cancel->setIcon(QtMaterialTheme::icon("navigation", "cancel"));
    cancel->setForegroundColor(QColor("#00bcd4"));
    cancel->setFontSize(10);
    cancel->setTextAlignment(Qt::AlignLeft);
    cancel->setIconSize(QSize(20,20));

    QtMaterialFlatButton *save = new QtMaterialFlatButton("Ignore & Save");
    save->setIcon(QtMaterialTheme::icon("content", "save"));
    save->setForegroundColor(QColor("#00bcd4"));
    save->setFontSize(10);
    save->setTextAlignment(Qt::AlignLeft);
    save->setIconSize(QSize(20,20));

    ui->verticalLayout->addWidget(checkList);
    ui->verticalLayout->addWidget(save);
    ui->verticalLayout->addWidget(cancel);


    connect(checkList, &QPushButton::clicked, this, [=](bool val){
        openItemCheckList();
        qDebug()<<Q_FUNC_INFO<<__LINE__;
    }
    );

    connect(cancel, &QPushButton::clicked, this, [=](bool val){
        emit saveConfirmed(false);
        this->close();

    });

    connect(save, &QPushButton::clicked, this, [=](bool val){

        emit saveConfirmed(true);
        this->close();

    });



}

ChecklistPromptDialog::~ChecklistPromptDialog()
{
    delete ui;
}

void ChecklistPromptDialog::openItemCheckList()
{
    ItemCheckListWidget *checkList = new ItemCheckListWidget(voucher);
    qDebug()<<Q_FUNC_INFO<<__LINE__;
//    checkList->setWindowFlags(Qt::Popup);
    checkList->show();

    qDebug()<<Q_FUNC_INFO<<__LINE__;
    connect(checkList, &ItemCheckListWidget::checkCompleted, this, [=](bool val){
        emit checkedItems(val);
        this->close();
    });

    qDebug()<<Q_FUNC_INFO<<__LINE__;
}
