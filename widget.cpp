#include "widget.h"
#include<QPaintEvent>
#include<QMouseEvent>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setdata();
}
void Widget::setdata(){
    bgFilename.clear();
    bgFilename="4.jpg";
    for (int i=0;i<20;i++) {
        for (int j=0;j<20;j++) {
            chessdata[i][j]=0;
        }

    }

}
void Widget::paintEvent(QPaintEvent *)
{
    //绘制背景图
    QPainter painter(this);
    QRect rec(QPoint(0,0),QPoint(this->width(),this->height()));
    QPixmap pix(bgFilename);
    painter.drawPixmap(rec,pix);
    //设置画线
    QPen pen;
    pen.setColor(Qt::yellow);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(4);
    painter.setPen(pen);
    //绘制棋盘
    for(int i=0;i<=20;i++)
        painter.drawLine(x0,y0+i*gridheight,21*gridwidth,y0+i*gridheight);
    for(int j=0;j<=20;j++)
        painter.drawLine(x0+j*gridwidth,y0,x0+j*gridwidth,21*gridheight);
    //绘制棋子
    QString chessFilename;
    for(int i=0;i<20;i++){
        for(int j=0;j<20;j++){
            if(chessdata[i][j]==white){
                chessFilename="1.png";
            }
            else if(chessdata[i][j]==black){
                chessFilename="2.png";
            }
            else {
                chessFilename.clear();
                continue;
            }
    painter.drawPixmap(x0+i*gridwidth,y0+j*gridheight,gridwidth,gridheight,chessFilename);
}
    }
}
void Widget::resizeEvent(QResizeEvent *){
    gridwidth=size().width()/22;
    gridheight=size().height()/22;
    x0=gridwidth;
    y0=gridheight;
}
void Widget::mousePressEvent(QMouseEvent *e){
    int x=e->x();
    int y=e->y();
    if(((x>=x0&&(x<=x0+20*gridwidth))&&(y>=y0&&(y<=y0+20*gridheight)))&&e->button()==Qt::LeftButton){
        int i=0,j=0;
        i=(x-x0)/gridwidth;
        j=(y-y0)/gridheight;
        SignalSendChessData(i,j);

    }

}
void Widget::setchessstatus(void *p){
    memcpy(chessdata,p,sizeof(int)*20*20);
    this->update();
}

void Widget::changebg(QString a){
    bgFilename=a;
}
Widget::~Widget()
{
}

