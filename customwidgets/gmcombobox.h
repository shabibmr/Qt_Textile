#ifndef GMCOMBOBOX_H
#define GMCOMBOBOX_H

#include <QComboBox>
#include "database/inventory/godown/godowndatabasehelper.h"

class GMComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit GMComboBox(QString *m, QComboBox *parent = Q_NULLPTR);
    ~GMComboBox();

    QString *value;
    QMap<QString, QString> godowns;
    GodownDatabaseHelper* gHelper;
    void getGodownList();

    bool isGodown=false;

signals:
    void selected();
public slots:
    void setValue(QString *temp);
    void ItemSelected(QString temp);
};

#endif // GMCOMBOBOX_H
