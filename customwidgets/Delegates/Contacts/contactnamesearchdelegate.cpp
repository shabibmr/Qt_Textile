
#include "contactnamesearchdelegate.h"

ContactNameSearchDelegate::ContactNameSearchDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{
    fontBold.setBold(true);
    fontItalic.setItalic(true);
}

