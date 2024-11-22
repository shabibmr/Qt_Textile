#ifndef ADDPROJECT_H
#define ADDPROJECT_H

#include <QWidget>
#include "database/projects/projectsdatabasehelper.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"

#include "datamodels/projects/projectsdataobject.h"

#include "customwidgets/gmledgersearch.h"

namespace Ui {
class AddProject;
}

class AddProject : public QWidget
{
    Q_OBJECT

public:
    explicit AddProject(QWidget *parent = 0);
    ~AddProject();
    ProjectsDataObject* projectsDataObject;
    ProjectsDatabaseHelper *dbHelper;

    QString id="";

public slots:
    void editProject(ProjectsDataObject prj);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::AddProject *ui;
signals:
    void AddProjectClosed();

};

#endif // ADDPROJECT_H
