#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QLCDNumber>
#include "fbnumlcd.h"

///计分板类 单纯的用于显示成绩
class ScoreBoard : public QWidget
{
    Q_OBJECT

public:
    ScoreBoard(QWidget *parent = 0);
    ~ScoreBoard();

    void setScore(int score,int top);	///设置显示的值：分数，最高分.

protected:
    void paintEvent(QPaintEvent *);

private:
    int Score;		///得分
    int top;		///排行

    FBNumLCD *scoreLcd;	///得分显示LCD
    FBNumLCD *topLcd;		///排行显示LCD
};

#endif // SCOREBOARD_H
