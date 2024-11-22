#include "hsnreportdelegate.h"
#include "QFile"
#include <QJsonDocument>

HsnReportDelegate::HsnReportDelegate()
{
    QFile file(":/json/HSN.json");  // :/json/json/dbchanges.json doesnt work
    file.open(QIODevice::ReadOnly);

    QString data = file.readAll();

    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8());
    hsnList = doc.object();

    file.close();
}


