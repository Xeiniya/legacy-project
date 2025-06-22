#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "module.h"
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setwColor();

private slots:
    void on_pbColor_clicked();
    void changeColor1();
    void changeColor2();
    void changeColor3();
private:
    void setColors();
    Ui::MainWindow *ui;
    Module* m1 = new Module;
    Module* m2 = new Module;
    Module* m3 = new Module;
    QWidget* wColor = new QWidget;
    QPushButton* pbColor = new QPushButton;
    QColor color;
};
#endif // MAINWINDOW_H
