#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette f = this->palette();
    f.setColor(QPalette::Window, QColor(249, 232, 250));
    this->setAutoFillBackground(true);
    this->setPalette(f);

    connect(ui->loadImageButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(ui->morphologyButton, &QPushButton::clicked, this, &MainWindow::applyMorphologicalProcessing);
    connect(ui->smoothingButton, &QPushButton::clicked, this, &MainWindow::applySmoothingFilter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выбрать изображение", "", "Images (*.png *.xpm *.jpg *.bmp)");
    if (!fileName.isEmpty())
    {
        currentImage.load(fileName);
        processedImage = currentImage;
        ui->imageLabel->setPixmap(QPixmap::fromImage(currentImage).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
    }
}
//морфологическая обработка
void MainWindow::applyMorphologicalProcessing()
{
    if (processedImage.isNull())
    {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите изображение!");
        return;
    }

    cv::Mat inputImage = QImageToMat(processedImage);

    cv::Mat morphedImage;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(inputImage, morphedImage, cv::MORPH_DILATE, kernel);

    processedImage = MatToQImage(morphedImage);
    ui->imageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
}
//низкочастот. сглаж. фильтр
void MainWindow::applySmoothingFilter()
{
    if (processedImage.isNull())
    {
        QMessageBox::warning(this, "Ошибка", "Сначала загрузите изображение!");
        return;
    }

    cv::Mat inputImage = QImageToMat(processedImage);

    cv::Mat blurredImage;
    cv::GaussianBlur(inputImage, blurredImage, cv::Size(5, 5), 0);

    processedImage = MatToQImage(blurredImage);
    ui->imageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(ui->imageLabel->size(), Qt::KeepAspectRatio));
}

cv::Mat MainWindow::QImageToMat(const QImage &image)
{
    cv::Mat mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine());
    return mat.clone();
}

QImage MainWindow::MatToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC4)
    {
        return QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32).copy();
    }
    else
    {
        return QImage();
    }
}
