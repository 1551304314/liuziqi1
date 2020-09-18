#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPainter>
#include<QMouseEvent>
class Widget : public QWidget
{
    Q_OBJECT
private:


protected:
    void paintEvent(QPaintEvent *);//绘制棋盘、棋子
    void setdata();//数据初始化
signals:
    void SignalSendChessData(int i,int j);//发送信号
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    enum chesstype{empty=0,white,black=-1};//棋子类型
    void setchessstatus(void * p);//同步数组
    void mouseReleaseEvent(QMouseEvent *);//鼠标事件
    int chessdata[20][20];//核心数组，保存棋盘坐标
};
#endif // WIDGET_H
