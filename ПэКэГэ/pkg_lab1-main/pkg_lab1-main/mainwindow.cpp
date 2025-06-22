#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "module.h"
#include <QGridLayout>
#include <QPalette>
#include <QColorDialog>
#include <QObject>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QObject::connect(pbColor, SIGNAL(clicked()), this, SLOT(on_pbColor_clicked()));
    QObject::connect(m1, SIGNAL(changed()), this, SLOT(changeColor1()));
    QObject::connect(m2, SIGNAL(changed()), this, SLOT(changeColor2()));
    QObject::connect(m3, SIGNAL(changed()), this, SLOT(changeColor3()));
    QObject::connect(m1, SIGNAL(sliderMoved()), this, SLOT(changeColor1()));
    QObject::connect(m2, SIGNAL(sliderMoved()), this, SLOT(changeColor2()));
    QObject::connect(m3, SIGNAL(sliderMoved()), this, SLOT(changeColor3()));

    QPalette f = this->palette();
    f.setColor(QPalette::Window, QColor(227, 241, 241));
    this->setAutoFillBackground(true);
    this->setPalette(f);

    QPalette p = QPalette();
    p.setBrush(QPalette::Window, Qt::white);
    wColor->setAutoFillBackground(true);
    wColor->setPalette(p);

    ui->setupUi(this);
    QGridLayout* l = new QGridLayout;

    pbColor->setText("Choose color");
    m1->setType(type::RGB);
    m2->setType(type::CMYK);
    m3->setType(type::XYZ);

    l->addWidget(wColor, 1, 0, 1, 1);
    l->addWidget(pbColor, 2, 0, 1, 1);
    l->addWidget(m1, 0, 1, 1, 1);
    l->addWidget(m2, 1, 1, 1, 1);
    l->addWidget(m3, 2, 1, 1, 1);

    ui->centralwidget->setLayout(l);
}

void MainWindow::on_pbColor_clicked(){
    color = QColorDialog::getColor();
    setColors();
    setwColor();
}

void MainWindow::changeColor1(){
    m2->setRecalc();
    m3->setRecalc();
    m2->setSliderRecalc();
    m3->setSliderRecalc();
    color = m1->getColor();
    m2->setColor(color);
    m3->setColor(color);
    setwColor();
}

void MainWindow::changeColor2(){
    m1->setRecalc();
    m3->setRecalc();
    m1->setSliderRecalc();
    m3->setSliderRecalc();
    color = m2->getColor();
    m1->setColor(color);
    m3->setColor(color);
    setwColor();
}

void MainWindow::changeColor3(){
    m2->setRecalc();
    m1->setRecalc();
    m2->setSliderRecalc();
    m1->setSliderRecalc();
    color = m3->getColor();
    m2->setColor(color);
    m1->setColor(color);
    setwColor();
}

void MainWindow::setColors(){
    m1->setColor(color);
    m2->setColor(color);
    m3->setColor(color);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setwColor(){
    QPalette p = QPalette();
    p.setBrush(QPalette::Window, color);
    wColor->setAutoFillBackground(true);
    wColor->setPalette(p);
}


