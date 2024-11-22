#include "projects.h"
#include "ui_projects.h"

Projects::Projects(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Projects)
{
    ui->setupUi(this);

    dbHelper = new ProjectsDatabaseHelper();
    ui->tableWidget->setColumnCount(1);

}

Projects::~Projects()
{
    delete ui;
}

void Projects::setTable()
{
    projList = dbHelper->getAllProjects();

}

void Projects::on_tableWidget_doubleClicked(const QModelIndex &index)
{
    AddProjectWidget = new AddProject(this);
    AddProjectWidget ->setWindowFlags(Qt::Window);
    AddProjectWidget ->show();
    QObject::connect(AddProjectWidget,SIGNAL(closing()),this,SLOT(setTable()));
}

void Projects::on_createNewPushButton_clicked()
{
    AddProjectWidget = new AddProject(this);
    AddProjectWidget ->setWindowFlags(Qt::Window);
    AddProjectWidget ->show();
    QObject::connect(AddProjectWidget,SIGNAL(closing()),this,SLOT(setTable()));
}
