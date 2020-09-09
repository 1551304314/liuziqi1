#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPainter>
#include<QMouseEvent>
class Widget : public QWidget
{
    Q_OBJECT
private:
    int chessdata[20][20];
    QString bgFilename;
    int gridwidth,gridheight,x0,y0;
protected:
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void setdata();
    void mousePressEvent(QMouseEvent *e);
signals:
    void SignalSendChessData(int i,int j);
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    enum chesstype{empty=0,white,black};
    void setchessstatus(void * p);
    void changebg(QString);

};
#endif // WIDGET_H
