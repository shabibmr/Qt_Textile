#ifndef EMPLOYEEADDRESSDATAMODEL_H
#define EMPLOYEEADDRESSDATAMODEL_H

#include <QObject>

class EmployeeAddressDataModel : public QObject
{
    Q_OBJECT
public:
    explicit EmployeeAddressDataModel(QObject *parent = nullptr);
    QString empName;

signals:

public slots:
};

#endif // EMPLOYEEADDRESSDATAMODEL_H
