#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QDate>
#include "addproject.h"

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();
    AddProject *AddProjectWidget;

private slots:
    void on_createNewPushButton_clicked();
    void keyPressEvent(QKeyEvent *e);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

private:
    Ui::ProjectWidget *ui;
};

#endif // PROJECTWIDGET_H
