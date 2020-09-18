#ifndef NETGAME_H
#define NETGAME_H

#include<widget.h>
#include<chessform.h>
#include<QTcpServer>
#include<QTcpSocket>
class NetGame :public chessForm
{
    Q_OBJECT


public:
    NetGame(bool server);
    QTcpServer * _server;
    QTcpSocket * _socket;
    void click(int, int) override;//落子重载
    ~NetGame();
public slots:
    void slotNewConnection();
    int slotRecv ();
signals:
    void turn_change(bool turn);//落子轮次转换
};

#endif // NETGAME_H
