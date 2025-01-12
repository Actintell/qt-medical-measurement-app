#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QPen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scaleFactor(1.0) {
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    imageItem = nullptr;
    lineItem = nullptr;

    // Подключаем сигналы и слоты
    connect(ui->btnLoadImage, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(ui->spinScaleFactor, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::setScaleFactor);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png)");
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            scene->clear();
            imageItem = scene->addPixmap(pixmap);
            scene->setSceneRect(pixmap.rect());
            lineItem = nullptr;
        }
    }
}

void MainWindow::setScaleFactor(double factor) {
    scaleFactor = factor;
}

void MainWindow::updateLength() {
    if (lineItem) {
        QLineF line = lineItem->line();
        double lengthMM = line.length() * scaleFactor;
        ui->lblLength->setText(QString("Length: %1 mm").arg(lengthMM, 0, 'f', 2));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());
    if (!lineItem) {
        startPoint = scenePos;
        lineItem = scene->addLine(QLineF(scenePos, scenePos), QPen(Qt::red, 2));
    } else {
        lineItem->setLine(QLineF(startPoint, scenePos));
        updateLength();
    }
}
