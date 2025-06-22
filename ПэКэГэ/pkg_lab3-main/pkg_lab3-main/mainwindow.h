#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <opencv2/opencv.hpp>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void applyMorphologicalProcessing();
    void applySmoothingFilter();

private:
    Ui::MainWindow *ui;
    QImage currentImage;
    QImage processedImage;

    cv::Mat QImageToMat(const QImage &image);
    QImage MatToQImage(const cv::Mat &mat);
};

#endif // MAINWINDOW_H
