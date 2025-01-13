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
    connect(ui->btnReset, &QPushButton::clicked, this, &MainWindow::resetImage);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png)");
    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            scene->clear();  // Очищаем сцену перед загрузкой нового изображения
            imageItem = scene->addPixmap(pixmap);
            scene->setSceneRect(pixmap.rect());
            lineItem = nullptr;
        }
    }
}

void MainWindow::setScaleFactor(double factor) {
    scaleFactor = factor;  // Устанавливаем новый коэффициент масштаба
}

void MainWindow::updateLength() {
    if (lineItem) {
        QLineF line = lineItem->line();
        double lengthMM = line.length() * scaleFactor;  // Переводим длину в миллиметры
        ui->lblLength->setText(QString("Length: %1 mm").arg(lengthMM, 0, 'f', 2));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    QPointF scenePos = ui->graphicsView->mapToScene(event->pos());  // Получаем координаты точки на сцене
    if (!lineItem) {
        startPoint = scenePos;
        lineItem = scene->addLine(QLineF(scenePos, scenePos), QPen(Qt::red, 2));  // Создаем новый отрезок
    } else {
        lineItem->setLine(QLineF(startPoint, scenePos));  // Обновляем линию
        updateLength();  // Обновляем длину
    }
}

void MainWindow::resetImage() {
    scene->clear();  // Очищаем сцену
    currentImage = QImage();  // Сбрасываем изображение
    lineItem = nullptr;  // Удаляем текущую линию
    ui->lblLength->clear();  // Очищаем метку с длиной
    ui->spinScaleFactor->setValue(1.00);  // Сбрасываем коэффициент масштаба на 1
}

