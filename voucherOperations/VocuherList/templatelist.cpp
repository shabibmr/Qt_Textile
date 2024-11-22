#include "templatelist.h"
#include "ui_templatelist.h"
#include "customwidgets/Delegates/reportcommondelegate.h"

TemplateList::TemplateList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemplateList)
{
    ui->setupUi(this);

    this->setWindowTitle(windowTitle);
    this->setObjectName("editor");
    this->setStyleSheet("QWidget#editor{background-color:" + backgroundColor + "}");
    QPalette pal = palette();

    //    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(),
    //                                          qApp->primaryScreen()->geometry()));

    // set black background
    pal.setColor(QPalette::Background,QColor(backgroundColor));
    setAutoFillBackground(true);
    setPalette(pal);
    ui->tableView->setItemDelegate(new ReportCommonDelegate());

}

TemplateList::~TemplateList()
{
    delete ui;
}

void TemplateList::on_createNew_clicked()
{
    VoucherEditor *editor = new VoucherEditor();
    editor->setWindowFlags(Qt::Window);
    editor->showMaximized();

}
