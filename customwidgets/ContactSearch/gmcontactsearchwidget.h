#ifndef GMCONTACTSEARCHWIDGET_H
#define GMCONTACTSEARCHWIDGET_H

#include <QWidget>
#include "datamodels/addressbook/addressbooksmall.h"
#include <QListWidgetItem>
#include "contactlistwidgetitem.h"
#include <QEvent>
#include <QKeyEvent>
#include <QPointer>
#include <QFocusEvent>
#include <QTableWidgetItem>


namespace Ui {
class GMContactSearchWidget;
}

class GMContactSearchWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GMContactSearchWidget(QWidget *parent = 0);
    ~GMContactSearchWidget();
    QList<QSharedPointer<AddressBookSmall>> itemsList;
    QStringList itemNames;
    int lLoc =0;
public slots:
    void setData(QList<QSharedPointer<AddressBookSmall>> items);
    void filterData(QString text);
    void checkData();
    void setFocusExpl(QString text);

private:
    Ui::GMContactSearchWidget *ui;

protected:
    bool eventFilter(QObject* obj, QEvent *event);


signals:
    void selectedID(int,QString);
    void nonSelected();
private slots:
    void on_tableWidget_clicked(const QModelIndex &index);
    void onTablewidgetEnterpressed(int item);
};

#endif // GMCONTACTSEARCHWIDGET_H
