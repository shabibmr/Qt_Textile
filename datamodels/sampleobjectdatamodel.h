#ifndef SAMPLEOBJECTDATAMODEL_H
#define SAMPLEOBJECTDATAMODEL_H

#include <QObject>

class SampleObjectDataModel : public QObject
{
    Q_OBJECT
public:
    explicit SampleObjectDataModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SAMPLEOBJECTDATAMODEL_H
