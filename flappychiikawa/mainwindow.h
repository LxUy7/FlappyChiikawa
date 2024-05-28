#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QTimer>
#include <QDataStream>
#include <QLabel>
#include <QPainter>
#include <QPixmap>

#include <chiikawa.h>
#include <ground.h>
#include "pipe.h"
#include "scoreboard.h"
#include "fbnumlcd.h"
#include "redyboard.h"

#include <QtGui>
#include <QMouseEvent>
#include <math.h>
#include <QDataStream>
#include <QFile>
#include <QtMultimedia>
#include<QSoundEffect>
#include <QUrl>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *);    ///绘制背景图
    void mousePressEvent(QMouseEvent *event);	///鼠标点击时间的处理
    void keyPressEvent(QKeyEvent *event);	///键盘事件处理

private slots:				
    void cAction();			///chiikawa的动作处理
    void pipeAction();			///管道的动作处理
    void collisDete();			///碰撞检测
    void scoreDete();			///计分检测函数
    void scbAction();			///计分板动画
private:
    void mainAction();			///主触发 作为鼠标和键盘事件处理的执行函数.

    void createPipe();			///初始化创建管道
    void gameLose();			///游戏失败时的处理
    void gameStart();			///游戏开始时的处理
    void gameRedy();

    void saveTop();			///存储最高分
    void loadTop();			///读取最高分

private:	
    Chiikawa *chiikawas;			///创建目标
    double cV;			///速度
    double timedata;			///timer时间戳
    QTimer *cTimer;			///控制运动的Timer
    double fcV[15];
    double fcp;
    enum{stop=0,start=1,redy=2};		///3个枚举值 代表了当前的游戏状态
    int gamemod;		///游戏状态 参照上面的枚举值

    RedyBoard *myredyBoard;	///开始时的准备面板

    enum{pipeCount=5};		///管道数量的常数
    Pipe *pipe[pipeCount];		///创建pipeCount个管道
    QTimer *pipeTimer;		///管道的远动处理Timer 并且该timer承担了窗口多个主要的处理任务
    int pipeTValue;
    int pipeXgap;		///管道之间的间距
    int lastPipe;		///最右端的管道标示
    QMediaPlayer *startSound;
    Ground *thisGround;		///底部的滚动动画

    ScoreBoard *scoreBoard;	///计分板
    bool isScobOk;		///计分板的显示状态 
    int score;			///分数
    int top;			///最高纪录
    bool cx;			///防止出现重复积分的状态标示
    QTimer *scobTimer;		///计分板的处理 当游戏结束时 触发
    FBNumLCD *scoreLCD;		///小计分牌
    QLabel *replay;		///最后计分板下面的开始按钮

};

#endif // MAINWINDOW_H
