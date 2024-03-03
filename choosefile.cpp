#include "choosefile.h"
#include "ui_choosefile.h"
#include <QFileDialog>
#include <lzw.h>
#include <string>
#include "bitio.h"
#include <QMessageBox>

using namespace std;


chooseFile::chooseFile(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::chooseFile)
{
    ui->setupUi(this);
}

chooseFile::~chooseFile()
{
    delete ui;
}
QString aFile;
QString bFile;
QString cFile;

void chooseFile::on_toolButton_clicked()
{


    QString curDir=QCoreApplication::applicationDirPath(); // 使用应用程序所在的目录
    aFile=QFileDialog::getOpenFileName(this,"选择压缩文件",curDir); // 省略文件过滤器参数
    ui->editFile->setText(aFile);

    // get the file name from aFile and append the original suffix and ".lzw" to it
    QFileInfo fileInfo(aFile);
    cFile = fileInfo.fileName(); // get the file name without path and suffix
    QString suffix = fileInfo.suffix(); // get the file suffix
    cFile = cFile.left(cFile.lastIndexOf("."));
    cFile += "lzw." + suffix;





}

void chooseFile::on_toolButton_2_clicked()
{

    QString curDir=QCoreApplication::applicationDirPath(); // 使用应用程序所在的目录
    bFile=QFileDialog::getExistingDirectory(this,"选择压缩文件夹",curDir); // 省略文件过滤器参数
    ui->saveFile->setText(bFile);
}





void chooseFile::on_compress_clicked()
{

    if (aFile.isEmpty() || bFile.isEmpty())
    {
        // show a message box to warn the user
        QMessageBox::warning(this, "警告", "请选择有效的文件");
        return;
    }
    // create a bit file input object and a bit file output object
    BitFileIn bit_in(aFile.toStdString().c_str()); // use the c_str() method to convert QString to const char*

    // use QDir to join bFile and cFile to get the full path of the output file
    QDir dir(bFile);
    QString fullPath = dir.filePath(cFile); // get the full path of the output file
    BitFileOut bit_out(fullPath.toStdString().c_str()); // use the c_str() method to convert QString to const char*

    // check if the objects are open
    if (!bit_in.is_open() /*|| !bit_out.is_open()*/)
    {
        // show a message box to warn the user
        QMessageBox::critical(this, "错误", "无法打开文件");
        return;
    }
    // call your own LZWencode function to compress the file
    // you need to implement this function using the methods in bitio.h
    bool success = LZWencode(bit_in, bit_out);
    // close the objects
    bit_in.close();
    bit_out.close();
    // check if the compression was successful
    if (success)
    {
        // show a message box to inform the user
        QMessageBox::information(this, "成功", "文件压缩完成");
        aFile="";
        bFile="";
        ui->saveFile->setText(bFile);
        ui->editFile->setText(aFile);

    }
    else
    {
        // show a message box to warn the user
        QMessageBox::warning(this, "失败", "文件压缩失败");
    }



}

