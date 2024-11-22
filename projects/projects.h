#ifndef PROJECTS_H
#define PROJECTS_H

#include <QWidget>
#include "addproject.h"
#include "database/projects/projectsdatabasehelper.h"
#include "datamodels/projects/projectsdataobject.h"

namespace Ui {
class Projects;
}

class Projects : public QWidget
{
    Q_OBJECT

public:
    explicit Projects(QWidget *parent = 0);
    ~Projects();

    ProjectsDatabaseHelper *dbHelper;
    QList<ProjectsDataObject> projList;


    AddProject* AddProjectWidget;

    void setTable();
private:
    Ui::Projects *ui;

signals:
    void selectedProject(QString projectID);
private slots:
    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void on_createNewPushButton_clicked();
};

#endif // PROJECTS_H
