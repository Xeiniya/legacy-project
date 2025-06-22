#ifndef MODULE_H
#define MODULE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>

enum type{
    RGB,
    CMYK,
    XYZ
};

class Module:public QWidget
{
    Q_OBJECT

public:
    Module();
    void setColor(QColor);
    void setType(type t);
    void outOfRangeError();
    void leChanged(QLineEdit*, int min, int max, bool);
    QColor getColor(){
        return color;
    }
    void setRecalc(){
        recalc = false;
    }
    void setSliderRecalc(){
        sliderRecalc = false;
    }
signals:
    void changed();
    void sliderMoved();
private slots:
    void le1Changed(bool = false);
    void le2Changed(bool = false);
    void le3Changed(bool = false);
    void le4Changed(bool = false);
    void s1Moved();
    void s2Moved();
    void s3Moved();
    void s4Moved();

private:
    double func1(double);
    double func2(double);
    QColor colorFromXYZ(double x, double y, double z);
    void setRGB();
    void setCMYK();
    void setXYZ();
    void setSliders();
    bool recalc = true;
    bool sliderRecalc = false;
    QColor xyzToColor();
    QColor color;
    type type;
    QLabel* l1 = new QLabel;
    QLabel* l2 = new QLabel;
    QLabel* l3 = new QLabel;
    QLabel* l4 = new QLabel;
    QLabel* lsystem = new QLabel;
    QLineEdit* le1 = new QLineEdit;
    QLineEdit* le2 = new QLineEdit;
    QLineEdit* le3 = new QLineEdit;
    QLineEdit* le4 = new QLineEdit;
    QSlider* s1 = new QSlider(Qt::Horizontal);
    QSlider* s2 = new QSlider(Qt::Horizontal);
    QSlider* s3 = new QSlider(Qt::Horizontal);
    QSlider* s4 = new QSlider(Qt::Horizontal);
};

#endif // MODULE_H
