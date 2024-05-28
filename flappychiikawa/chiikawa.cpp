#include "chiikawa.h"
#include <QWidget>
#include <QKeyEvent>

Chiikawa::Chiikawa(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(50,50);
    co=0;
    timer=new QTimer(this);
    src[0]=":/Images/chiikawa1.png";
    src[1]=":/Images/chiikawa2.png";
    src[2]=":/Images/chiikawa3.png";
    this->rale=-31;
    setFocusPolicy(Qt::StrongFocus); // 允许控件接收键盘焦点
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    connect(this, SIGNAL(fly()), this, SLOT(updateRale()));
    timer->start(200);
}
void Chiikawa::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_U) {
        src[0] = ":/Images/usagi1.png";
        src[1] = ":/Images/usagi3.png";
        src[2] = ":/Images/usagi2.png";
        update();  // 刷新控件，如果需要的话
    }
    if(event->key()==Qt::Key_C){
        src[0]=":/Images/chiikawa1.png";
        src[1]=":/Images/chiikawa2.png";
        src[2]=":/Images/chiikawa3.png";
        update();
    }
    if(event->key()==Qt::Key_H){
        src[0]=":/Images/hachiware1.png";
        src[1]=":/Images/hachiware2.png";
        src[2]=":/Images/hachiware3.png";
        update();
    }
}

Chiikawa::~Chiikawa()
{

}

void Chiikawa::play()
{
    timer->start(200);
}

void Chiikawa::stop()
{
    timer->stop();
}

void Chiikawa::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;

    painter.setWindow(-20,-20,40,40);	//设置画布等于偏移中心坐标，使绝对坐标中心轴为20,0
    pix.load(src[co]);
    if(zt==1)
    {
        painter.rotate(ztr);
        ztr-=10;
        if(ztr<=30)
        {
            zt=0;
        }
    }
    else
    {
        if(rale>=-30&&rale<=60)
        {
            painter.rotate(rale);
            if(rale<10)
            {
                rale=0.01*x*x-30;
                x+=1;
            }
            else
            {
                rale+=2;
            }
        }
        else if(rale>=60)
        {
            rale=60;
            painter.rotate(rale);
        }
        else if(rale<-30)
        {
            painter.rotate(0);
        }
    }
    co++;
    if(co>=3)
        co=0;
    painter.rotate(0);
    painter.drawPixmap(-20,-20,40,40,pix);
}

void Chiikawa::updateRale()
{
    ztr=rale;
    this->rale=-30;
    x=0;
    zt=1;

}

void Chiikawa::setRale(int rale)
{
    this->rale=rale;
}
