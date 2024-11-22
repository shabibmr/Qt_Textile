#ifndef UISETTINGSDATAMODEL_H
#define UISETTINGSDATAMODEL_H
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>


class UiSettingsDataModel
{
public:
    UiSettingsDataModel();

    int ui_id;
    QString ui_name;
    QString ui_type;
    QString sub_type;
    QString query;
    QJsonObject redirect_to;
    QJsonObject filters;
    QJsonObject displayOptions;
    QJsonArray extras;

    QList<int> hideCols;
    QList<int> widthList;
    QList<int> AlignmentList;
    QList<int> totalCols;
    int stretchCol;
    QList<int> avlCols;


//    bool operator==(const UiSettingsDataModel& rhs) const
//    {
//        return (ui_id == rhs.ui_id) && (ui_name == rhs.ui_name)
//                && (ui_type == rhs.ui_type) && (sub_type == rhs.sub_type)
//                && (query == rhs.query) && (redirect_to == rhs.redirect_to)
//                && (filters == rhs.filters) && (displayOptions == rhs.displayOptions);

//    }

};

#endif // UISETTINGSDATAMODEL_H
