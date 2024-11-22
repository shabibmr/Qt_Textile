#ifndef LINEEDITTEST_H
#define LINEEDITTEST_H

#include <QWidget>

namespace Ui {
class LineEditTest;
}

class LineEditTest : public QWidget
{
    Q_OBJECT

public:
    explicit LineEditTest(QWidget *parent = 0);
    ~LineEditTest();

private:
    Ui::LineEditTest *ui;
};

#endif // LINEEDITTEST_H
