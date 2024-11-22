#ifndef QUOTEVIEW_H
#define QUOTEVIEW_H

#include <QWidget>
#include "itemdetailview.h"
#include "datamodels/transaction/generalvoucherdataobject.h"
#include <QToolButton>
#include "sharetools/print/printerhelper.h"


namespace Ui {
class QuoteView;
}

class QuoteView : public QWidget
{
    Q_OBJECT

public:
    explicit QuoteView(QWidget *parent = 0);
    ~QuoteView();
    GeneralVoucherDataObject Voucher;
    ItemDetailView* ItemDetailViewWidget;
    QList<DocumentsDataModel> docObj;

    QuotationDatabaseHelper* dbHelper;

    QToolButton *CompletedButton;
    QToolButton *printButton;

    QToolButton *DraftsApprovedButton;


private slots:
    void on_tableWidget_clicked(const QModelIndex &index);

    void on_tableWidget_doubleClicked(const QModelIndex &index);

    void on_browseButton_clicked();

    void on_UploadButton_clicked();

    void on_save_clicked();

    void on_cancelButton_clicked();

    void delButtonClicked(bool);

    void downloadClicked(bool);

    void printClicked(bool);

    void on_fileName_textChanged(const QString &arg1);

public slots:
    void settable();
    void setFilesTable();
    void showVoucher(GeneralVoucherDataObject quote);
    void completedClicked(bool t);
    void DraftsApprovedButtonClicked(bool x);
signals:
    void closing();
private:
    Ui::QuoteView *ui;
};

#endif // QUOTEVIEW_H
