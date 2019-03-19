#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "watershed.h"
#include <QFileDialog>
#include <vector>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addRows(int count){
    for (int i =0; i < count; i++){
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
   }
}

void MainWindow::on_pushButton_clicked()
{
    ui->tableWidget->setRowCount(0);
    int amount;
    std::vector<double> length;
    std::vector<double> width;
    std::vector<double> area;
    wshed(ui->label->text(),ui->spinBox->value(),ui->spinBox_2->value(), amount, length, width, area);
    ui->label_2->setText("Amount: " +QString::number(amount));
    addRows(amount);

    for(int row = 0; row < amount; row++) {
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(length[row])));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(width[row])));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(area[row])));
    }
    QImage img("result.png");
    pixmap = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(pixmap);
}

void MainWindow::on_pushButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                    this,
                    tr("Open File"),
                    "",
                    tr("Image Files (*.png *.jpg *.bmp *.gif *.tiff)")
                    );
    ui->label->setText(path);
}
