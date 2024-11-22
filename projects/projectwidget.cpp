#include "projectwidget.h"
#include "ui_projectwidget.h"

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
    ui->fromDateDateEdit->setDate(QDate::currentDate());
    ui->toDateDateEdit->setDate(QDate::currentDate());
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}

void ProjectWidget::keyPressEvent(QKeyEvent *e)
{
   if ((e->key() == Qt::Key_N)  && (e->modifiers().testFlag(Qt::ControlModifier))){
        on_createNewPushButton_clicked();
    }

}

void ProjectWidget::on_createNewPushButton_clicked()
{
    AddProjectWidget = new AddProject(this);
    AddProjectWidget->setWindowFlags(Qt::Window);
    AddProjectWidget->show();
}

void ProjectWidget::on_tableWidget_doubleClicked(const QModelIndex &index)
{

}
