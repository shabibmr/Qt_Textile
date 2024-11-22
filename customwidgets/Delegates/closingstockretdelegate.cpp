#include "closingstockretdelegate.h"



ClosingStockRetDelegate::ClosingStockRetDelegate(bool showStockVarValues)
{
    this->showStockVariance = showStockVarValues;

    qDebug()<<Q_FUNC_INFO<<__LINE__<<showStockVariance;
}
