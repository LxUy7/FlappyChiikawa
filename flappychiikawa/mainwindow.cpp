#include "mainwindow.h"
#include <QDesktopWidget>
#include <QMessageBox>
#include <ctime>
#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(380,500);	///锁定窗口的大小为380x500
    this->setWindowIcon(QIcon(":/Images/chiikawa.jpeg"));
    this->move((QApplication::desktop()->width()-this->width())/2,(QApplication::desktop()->height()-this->height())/2);	///这样做可以让窗口居中于屏幕显示，需要引入 QDesktopWidget

    for(int i=0;i<pipeCount;i++)	///创建障碍物对象
        pipe[i]=new Pipe(this);

    createPipe();
    pipeTimer =new QTimer(this);	///创建Timer
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(pipeAction()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(collisDete()));
    connect(pipeTimer,SIGNAL(timeout()),this,SLOT(scoreDete()));
    pipeTValue=8;

    chiikawas=new Chiikawa(this);
    chiikawas->move(60,250);
    setFocusPolicy(Qt::StrongFocus);
    this->fcV[0]=-2;
    this->fcV[1]=-2;
    this->fcV[2]=-2;
    this->fcV[3]=-1;
    this->fcV[4]=-1;
    this->fcV[5]=-1;
    this->fcV[6]=-1;
    this->fcV[7]=0;
    this->fcV[8]=1;
    this->fcV[9]=1;
    this->fcV[10]=1;
    this->fcV[11]=1;
    this->fcV[12]=1;
    this->fcV[13]=2;
    this->fcV[14]=2;
    this->fcp=0;


    cTimer=new QTimer(this);
    connect(cTimer,SIGNAL(timeout()),this,SLOT(cAction()));
    cV=0;
    gamemod=redy;		///游戏状态出事化为redy

    score=0;
    top=0;
    loadTop();		///载入得分记录

    scoreLCD=new FBNumLCD(this);	///计分板的实现
    scoreLCD->setShowHead(false);	///不显示多余位数
    scoreLCD->setValue(0);
    scoreLCD->move(140,-100);

    scoreBoard=new ScoreBoard(this);
    scoreBoard->move(-350,100);
    scobTimer=new QTimer(this);
    connect(scobTimer,SIGNAL(timeout()),this,SLOT(scbAction()));


    thisGround=new Ground(this);
    thisGround->move(0,450);

    myredyBoard=new RedyBoard(this);
    myredyBoard->setFixedSize(380,500);
    myredyBoard->move(0,0);

    replay=new QLabel(this);
    QPixmap pix;
    pix.load(":/Images/replay.png");		///所有的素材写进资源文件中
    replay->setPixmap(pix);
    replay->setFixedSize(140,80);
    replay->move(120,-400);
    this->setWindowTitle("FlappyChiikawa");		///设置标题

    QSoundEffect *music = new QSoundEffect();
    music->setSource(QUrl::fromLocalFile(":/new/res_mus.wav"));
    music->setLoopCount(QSoundEffect::Infinite);  ///设置无限循环
    music->setVolume(0.5f);  ///设置音量，在0到1之间
    music->play();

}

void MainWindow::createPipe()		///初始化管道。使其以一定次序排在地图之外
{
    int startx=500+380;		///第一个管道的位置
    pipeXgap=200;		///间距
    int pipR;			///产生一个垂直位置的随机数
    qsrand(time(NULL));
    for(int i=0;i<pipeCount;i++)
    {
        pipR=qrand()%200;

        pipe[i]->move(startx+i*pipeXgap,-200+pipR);
        lastPipe=i;
    }
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *)		///重载的绘图事件
{
    QPainter painter(this);
    QPixmap pixmap;
    pixmap.load(":/Images/background.jpg");
    painter.drawPixmap(0,0,380,450,pixmap);
}

void MainWindow::mousePressEvent(QMouseEvent *event)	///鼠标事件
{

    if((event->button()==Qt::LeftButton||event->button()==Qt::RightButton)&&chiikawas->pos().y()>0)
    {
        if(gamemod==stop)
        {
            if(isScobOk)
                if((event->pos().x()>=120&&event->pos().x()<=260)&&(event->pos().y()>=400&&event->pos().y()<=480))
                {
                    gameRedy();
                }

        }
        else
        {
            mainAction();
        }

    }
}

void MainWindow::keyPressEvent(QKeyEvent *levent)
{
    //键盘处理 空格 上键的可操控
    if((levent->key()==Qt::Key_Space||levent->key()==Qt::Key_Up)&&chiikawas->pos().y()>0)
    {
        mainAction();
    }
}

void MainWindow::mainAction()
{
    //通用事件处理
    if(gamemod==redy)	//如果是redy：开始游戏，目标开始运动，管道开始运动
    {
        gameStart();
        timedata=8;
        cTimer->start(timedata);
        pipeTimer->start(pipeTValue);
        fcp=0.0;
        cV=fcV[int(fcp)];		//每次触发 目标都会向上飞 也就是速度是负数。
        emit chiikawas->fly();	//这个信号是CHiikawa类的 用于抬头
    }
    else if(gamemod==start)	//如果游戏是开始状态，只处理chiikawa的飞行姿态
    {
        fcp=0.0;
        cV=fcV[int(fcp)];
        timedata=8;
        cTimer->setInterval(timedata);
        emit chiikawas->fly();
    }
}

void MainWindow::cAction()
{
    ///chiikawa的运动时间
    chiikawas->move(chiikawas->pos().x(),chiikawas->pos().y()+cV);

    if(fcp<14.0)
        fcp+=0.2;
    else
        fcp=14.0;
    cV=fcV[int(fcp)];

    if(chiikawas->pos().y()+chiikawas->height()>=450)
    {
        chiikawas->move(chiikawas->pos().x(),450-chiikawas->height()+5);
        cTimer->stop();
        gameLose();
        chiikawas->setRale(90);
    }

}

void MainWindow::pipeAction()
{
    //障碍物的动画
    int pipR;
    for(int i=0;i<pipeCount;i++)
    {
        pipe[i]->move(pipe[i]->pos().x()-1,pipe[i]->pos().y());
        if(pipe[i]->pos().x()<-100)
        {
            pipR=qrand()%200;
            pipe[i]->move(pipe[lastPipe]->pos().x()+pipeXgap,-200+pipR);
            lastPipe=i;
        }
    }
}

void MainWindow::collisDete()
{
    //碰撞检测
    int cRx=chiikawas->pos().x()+30;
    int cDy=chiikawas->pos().y()+30;
    for(int i=0;i<pipeCount;i++)
    {
        if(cRx>=pipe[i]->x()&&chiikawas->pos().x()<=pipe[i]->pos().x()+pipe[i]->width()-10)
        {
            if(chiikawas->y() <= (pipe[i]->y()+pipe[i]->getH1()) ||
                    cDy >= (pipe[i]->y()+pipe[i]->getH1()+pipe[i]->getGap()))
                gameLose();
        }
    }
}

void MainWindow::scoreDete()
{
    ///分数检测
    for(int i=0;i<pipeCount;i++)
    {
        if(chiikawas->pos().x()+chiikawas->width()>=pipe[i]->pos().x()+35&&chiikawas->pos().x()+chiikawas->width()<=pipe[i]->pos().x()+40&&cx)
        {         
            this->score+=1;
            scoreLCD->setValue(score);
            if(score>=1000)
            {
                scoreLCD->move(90+14*3,50);
            }
            else if(score>=100)
            {
                scoreLCD->move(90+14*2,50);
            }
            else if(score>=10)
            {
                scoreLCD->move(90+14,50);
            }
            cx=0;
        }

        if(chiikawas->pos().x()>=pipe[i]->pos().x()+68&&chiikawas->pos().x()<=pipe[i]->pos().x()+73)
            cx=1;
    }
}


void MainWindow::scbAction()
{
    ///失败时的记分牌动画
    scoreBoard->move(scoreBoard->pos().x()+1,scoreBoard->pos().y());
    if(scoreBoard->pos().x()>=40)
    {
        scoreBoard->move(40,scoreBoard->pos().y());
        scobTimer->stop();
        replay->move(120,400);
        isScobOk=1;
    }
}

void MainWindow::gameRedy()
{
    myredyBoard->show();
    scoreBoard->move(-350,100);
    chiikawas->move(60,250);
    replay->move(120,-400);
    createPipe();
    chiikawas->setRale(-50);
    gamemod=redy;
    scoreLCD->setValue(0);
    thisGround->play();
    chiikawas->play();
}

void MainWindow::gameLose()
{
    ///游戏失败时的处理
    isScobOk=0;
    gamemod=stop;
    chiikawas->stop();
    pipeTimer->stop();
    thisGround->stop();
    if(score>top)	///如果创造了新纪录，保存记录
    {
        top=score;
        saveTop();
    }
    scoreBoard->setScore(score,top);
    scobTimer->start(3);
    scoreLCD->move(140,-100);

}

void MainWindow::gameStart()
{
    ///开始游戏的动作
    gamemod=start;
    myredyBoard->close();
    cx=1;
    score=0;
    scoreLCD->move(90,50);

}

void MainWindow::saveTop()
{
    ///保存记录，二进制保存，存储在top.d文件下。
    QFile file("top.d");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out<<this->top;
}

void MainWindow::loadTop()
{
    ///读取记录 在构造函数里触发
    QFile file("top.d");
    if(file.open(QIODevice::ReadOnly))
    {
        QDataStream in(&file);
        in>>this->top;
    }
    else
        top=0;
}
