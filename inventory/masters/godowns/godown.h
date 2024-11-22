#ifndef GODOWN_H
#define GODOWN_H

#include <QWidget>
#include <QKeyEvent>
#include "inventory/masters/godowns/addgodown.h"

namespace Ui {
class godown;
}

class godown : public QWidget
{
    Q_OBJECT

public:
    explicit godown(QWidget *parent = 0);
    ~godown();
    addGodown *addGodownObject;
    GodownDatabaseHelper *dbHelper;
    QList<GodownDataObject*> godownList;
public slots:
    void settable();
private slots:
    void on_pushButton_clicked();

    void on_tableWidget_doubleClicked(const QModelIndex &index);
    void keyPressEvent(QKeyEvent *e);

private:
    Ui::godown *ui;
};

#endif // GODOWN_H
