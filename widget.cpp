#include "widget.h"
#include<QPaintEvent>
#include<QMouseEvent>
#include<QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setdata();
}
//数组初始化
void Widget::setdata(){
    resize(840, 840);
    memset(chessdata, 0, 20 * 20 * sizeof(int));


}
//绘制棋盘、棋子
void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);
    int i, j;
    for (i = 0; i < 21; i++)
    {
        p.drawLine(20, 20 + i * 40, 820, 20 + i * 40);
        p.drawLine(20 + i * 40, 20, 20 + i * 40, 820);
    }

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (chessdata[i][j] == black)
            {
                brush.setColor(Qt::black);
                p.setBrush(brush);
                p.drawEllipse(QPoint((i + 1) * 40, (j + 1) * 40), 20, 20);
                update();
            }
            else if (chessdata[i][j] == white)
            {
                brush.setColor(Qt::white);
                p.setBrush(brush);
                p.drawEllipse(QPoint((i + 1) * 40, (j + 1) * 40), 20, 20);
                update();
            }
        }
    }
}
//获取鼠标坐标
void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    int x , y;
    if(e->x() >= 20 && e->x() < 820 && e->y() >= 20 && e->y() < 820)
    {
        x = (e->x() - 20) / 40;
        y = (e->y() - 20) / 40;
       // qDebug()<<x<<y;
        SignalSendChessData(x,y);
    }
    update();


}

//传送棋盘数据
void Widget::setchessstatus(void *p){
    memcpy(chessdata,p,sizeof(int)*20*20);
    this->update();
}


Widget::~Widget()
{
}

