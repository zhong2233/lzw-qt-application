#ifndef CHOOSEFILE2_H
#define CHOOSEFILE2_H

#include <QWidget>

namespace Ui {
class choosefile2;
}

class choosefile2 : public QWidget
{
    Q_OBJECT

public:
    explicit choosefile2(QWidget *parent = nullptr);
    ~choosefile2();

private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::choosefile2 *ui;
};

#endif // CHOOSEFILE2_H
