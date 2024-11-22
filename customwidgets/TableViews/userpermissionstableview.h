#ifndef USERPERMISSIONSTABLEVIEW_H
#define USERPERMISSIONSTABLEVIEW_H

#include <QTableView>
#include <QObject>
#include <QWidget>

class UserPermissionsTableView : public QTableView
{
    Q_OBJECT
public:

    explicit UserPermissionsTableView(QTableView *parent= nullptr);
};

#endif // USERPERMISSIONSTABLEVIEW_H
