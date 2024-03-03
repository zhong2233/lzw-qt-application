#include "choosefile2.h"
#include "ui_choosefile2.h"
#include <QFileDialog>
#include "lzw.cpp"
#include <string>
#include "bitio.h"
#include <QMessageBox>
choosefile2::choosefile2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::choosefile2)
{
    ui->setupUi(this);
}

choosefile2::~choosefile2()
{
    delete ui;
}

QString aFile2;
QString bFile2;
QString cFile2;
QString suffix;
void choosefile2::on_toolButton_clicked()
{
    QString curDir=QCoreApplication::applicationDirPath(); // 使用应用程序所在的目录
    aFile2=QFileDialog::getOpenFileName(this,"选择压缩文件",curDir); // 省略文件过滤器参数
    ui->lineEdit->setText(aFile2);
    QFileInfo fileInfo(aFile2);
    cFile2 = fileInfo.fileName(); // get the file name without path and suffix
    suffix = fileInfo.suffix(); // get the file suffix

}


void choosefile2::on_toolButton_2_clicked()
{
    QString curDir=QCoreApplication::applicationDirPath(); // 使用应用程序所在的目录
    bFile2=QFileDialog::getExistingDirectory(this,"选择压缩文件夹",curDir); // 省略文件过滤器参数
    ui->lineEdit_2->setText(bFile2);;
    // remove the "lzw" character from the file name
    cFile2 = cFile2.replace("lzw.", ".");
    // append the file name and suffix to the output folder path
    bFile2 = bFile2 + "/" + cFile2 ;

}


void choosefile2::on_pushButton_clicked()
{
    // check if the input and output files are valid
    if (aFile2.isEmpty() || bFile2.isEmpty())
    {
        // show a message box to warn the user
        QMessageBox::warning(this, "警告", "请选择有效的文件");
        return;
    }
    // create a bit file input object and a bit file output object
    BitFileIn bit_in(aFile2.toStdString().c_str()); // use the c_str() method to convert QString to const char*
    BitFileOut bit_out(bFile2.toStdString().c_str());
    // check if the objects are open
    if (!bit_in.is_open() || !bit_out.is_open())
    {
        // show a message box to warn the user
        QMessageBox::critical(this, "错误", "无法打开文件");
        return;
    }
    // call your own LZWdecode function to decompress the file
    // you need to implement this function using the methods in bitio.h and lzw.h
    bool success = LZWdecode(bit_out, bit_in);
    // close the objects
    bit_in.close();
    bit_out.close();
    // check if the decompression was successful
    if (success)
    {
        // show a message box to inform the user
        QMessageBox::information(this, "成功", "文件解压缩完成");
        aFile2="";
        bFile2="";
        ui->lineEdit->setText(aFile2);
        ui->lineEdit_2->setText(bFile2);

    }
    else
    {
        // show a message box to warn the user
        QMessageBox::warning(this, "失败", "文件解压缩失败");
    }
}



