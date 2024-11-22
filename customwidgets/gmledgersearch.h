#ifndef GMLEDGERSEARCH_H
#define GMLEDGERSEARCH_H

#include <QObject>
#include <QLineEdit>
#include <QListWidget>
#include <QPointer>
#include <QPoint>
#include <QFocusEvent>
#include "gmledgersearchwidget.h"
#include "database/finance/ledgermaster/ledgermasterdatabasehelper.h"


class GMLedgerSearch : public QLineEdit
{
    Q_OBJECT
public:
    explicit GMLedgerSearch(QList<QSharedPointer<LedgerMasterDataModel>> itemsListPtr,QLineEdit *parent=nullptr);
    ~GMLedgerSearch();
    //QList<LedgerMasterDataModel> ledgers;
    QList<QSharedPointer<LedgerMasterDataModel>> ledgers;
    QStringList list;
    LedgerMasterDatabaseHelper *ledgersHelper;
    GMLedgerSearchWidget *listWidget;
    bool ftoggle;
    QWidget *mainParent;

protected:
    void textClicked();
    void focusInEvent(QFocusEvent* e);

public slots:
    void checkEdit();
    void editTextChanged(QString text);
    void setMainParent(QWidget *Parent);
    void ledgerSelected(QString,QString);
    void noneSelected();

signals:
    void SelectedLedger(LedgerMasterDataModel);
    void clearItem();
};


#endif // GMLEDGERSEARCH_H
