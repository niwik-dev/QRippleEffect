#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "ripple.hpp"

QPushButton* RippleButton1(QWidget*parent,QColor color){
    QPushButton*btn = new QPushButton();
    btn->setParent(parent);
    btn->setFixedSize(200,200);
    btn->setStyleSheet("background-color:transparent;");
    RippleEffect *effect = new RippleEffect(btn);
    RippleOption *option = new RippleOption();
    option->rippleColor = color;
    effect->setRippleOption(option);
    btn->setGraphicsEffect(effect);
    return btn;
}

QPushButton* RippleButton2(QWidget*parent,QColor color){
    QPushButton*btn = new QPushButton();
    btn->setParent(parent);
    btn->setFixedSize(200,200);
    btn->setStyleSheet("background-color:transparent;");
    RippleEffect *effect = new RippleEffect(btn);
    RippleOption *option = new RippleOption();
    option->rippleColor = color;
    option->startPointOption = StartPointOption::Centered;
    effect->setRippleOption(option);
    btn->setGraphicsEffect(effect);
    return btn;
}

class RippleTestWidget: public QWidget{
private:
    QLayout *cols,*row1,*row2;
public:
    RippleTestWidget(){
        cols = new QVBoxLayout(this);
        row1 = new QHBoxLayout(this);
        cols->addItem(row1);
        this->row1->addWidget(RippleButton1(this,QColor(255,0,0,100)));
        this->row1->addWidget(RippleButton1(this,QColor(0,255,0,100)));
        this->row1->addWidget(RippleButton1(this,QColor(0,0,255,100)));

        row2 = new QHBoxLayout(this);
        cols->addItem(row2);
        this->row2->addWidget(RippleButton2(this,QColor(255,255,0,100)));
        this->row2->addWidget(RippleButton2(this,QColor(0,255,255,100)));
        this->row2->addWidget(RippleButton2(this,QColor(255,0,255,100)));
    }
};


int main(int argc,char**argv){
    QApplication app(argc,argv);
    QWidget *widget = new RippleTestWidget();
    widget->show();
    return app.exec();
}