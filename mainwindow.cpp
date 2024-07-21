#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QIcon>
#include <QDebug>
#include <QListView>
#include <QFileDialog>
#include <QDir>
#include <QPainter>
#include <QMessageBox>
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->ui->scrollAreaWidgetContents->setIconSize(100, 100);
    this->ui->scrollAreaWidgetContents->setItemVSpace(20);
    this->ui->scrollAreaWidgetContents->setItemHSpace(20);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QFileDialog dlg(this, "请选择图片文件");
    dlg.setFileMode(QFileDialog::ExistingFiles);
    dlg.setFilter(QDir::Files);
    QStringList str_list = dlg.getOpenFileNames(this, "", "", "Image Files (*.png *.jpg *.bmp)");
    for (const QString &str : str_list) {
        this->ui->scrollAreaWidgetContents->addItem(str);
    }
}


void MainWindow::on_pushButton_3_clicked()
{
    int max_width = 0, height = 0;
    QList<QImage> img_list = this->ui->scrollAreaWidgetContents->imageList();
    for (const QImage &img : img_list) {
        max_width = max_width > img.width() ? max_width : img.width();
        height += img.height();
    }

    QImage resultImg = QImage(max_width, height, QImage::Format_RGB32);
    QPainter *painter = new QPainter(&resultImg);
    int current_y = 0;
    for (const QImage &img : img_list) {
        int x = (max_width - img.width()) / 2;
        painter->drawImage(x, current_y, img);
        current_y += img.height();
    }
    QFileDialog dlg(this, "请选择保存文件");
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setFilter(QDir::Files);
    QString str = dlg.getSaveFileName(this, "", "", "JPG File (*.jpg)");
    if (resultImg.save(str)) {
        QMessageBox::information(this, "系统提示", "合并成功");
    }
    delete painter;
}


void MainWindow::on_pushButton_2_clicked()
{
    this->ui->scrollAreaWidgetContents->removeBatch(
                this->ui->scrollAreaWidgetContents->currentIndexList());
}

