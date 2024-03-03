#include "widget.h"
#include "ui_widget.h"
#include "choosefile.h"
#include "choosefile2.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


}

Widget::~Widget()
{
    delete ui;
}







void Widget::on_pushButton_clicked()
{
    chooseFile *nc=new chooseFile();
    nc->setAttribute(Qt::WA_DeleteOnClose);
    nc->setWindowTitle("选择文件");
    nc->show();

}



void Widget::on_pushButton_2_clicked()
{
    choosefile2 *nb=new choosefile2();
     nb->setAttribute(Qt::WA_DeleteOnClose);
     nb->setWindowTitle("选择文件");
     nb->show();
}


void Widget::on_pushButton_3_clicked()
{

}

