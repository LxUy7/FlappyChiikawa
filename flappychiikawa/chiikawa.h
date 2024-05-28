#ifndef CHIIKAWA_H
#define CHIIKAWA_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <math.h>
#include<QEvent>

///这是Chiikawa的类
class Chiikawa : public QWidget
{
    Q_OBJECT
public:
    Chiikawa(QWidget *parent = 0);
    ~Chiikawa();
    void setRale(int rale);	///设置旋转角度。
    void play();
    void stop();
    void keyPressEvent(QKeyEvent *event);
signals:
    void fly();		///触发抬头动作的信号

private slots:
    void updateRale();		///更新旋转角度。

protected:
    void paintEvent(QPaintEvent *);		///绘图事件
private:
    double rale;		///角度参数
    double x;			///旋转角度的单位偏移
    int zt;		///一个30度夹角的标识
    double ztr;
    int co;		///chiikawa的动作由三张图片组成
    QTimer *timer;
    QString src[3];	///位图的绝对路径
};

#endif // CHIIKAWA_H
