#include "redyboard.h"

RedyBoard::RedyBoard(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(0,0,380,500);
    label=new QLabel(tr("<font color=red>控制：鼠标左右键 or 空格 </font>"),this);
    label->move(80,480);
    convertLabel=new QLabel(tr(" PRESS\n C吉伊\nU乌萨奇\n H小八"),this);
    convertLabel->move(145,320);
}

RedyBoard::~RedyBoard()
{

}

void RedyBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    painter.setWindow(0,0,380,500);
    pix.load(":/Images/ready.png");
    painter.drawPixmap(0,0,380,500,pix);
}
