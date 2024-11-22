#ifndef GMPAYSLIPDELEGATE_H
#define GMPAYSLIPDELEGATE_H

#include <QStyledItemDelegate>
#include <QDateTimeEdit>
#include <QCheckBox>
#include "customwidgets/SearchWidgets/Ledger/gmledgerlinewidget.h"
#include "customwidgets/SearchWidgets/Employee/gmemployeesearchwidget.h"
#include "QVBoxLayout"
#include <QMessageBox>

#include "customwidgets/Material/components/qtmaterialiconbutton.h"
#include "customwidgets/Material/components/lib/qtmaterialtheme.h"


class GMPayslipDelegate : public QStyledItemDelegate
{
public:
    GMPayslipDelegate();
    virtual ~GMPayslipDelegate() override;
    UserProfileDatabaseHelper *itemHelper;
    explicit GMPayslipDelegate(QSqlQueryModel *model);

public:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const override;

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QSqlQueryModel *model;

public slots :
    void commitAndCloseItemEditor(QModelIndex index);

};

#endif // GMPAYSLIPDELEGATE_H
