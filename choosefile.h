#ifndef CHOOSEFILE_H
#define CHOOSEFILE_H

#include <QWidget>

namespace Ui {
class chooseFile;
}

class chooseFile : public QWidget
{
    Q_OBJECT

public:
    explicit chooseFile(QWidget *parent = nullptr);
    ~chooseFile();

private slots:
    void on_toolButton_clicked();

    void on_toolButton_2_clicked();


    void on_compress_clicked();

private:
    Ui::chooseFile *ui;
};

#endif // CHOOSEFILE_H
