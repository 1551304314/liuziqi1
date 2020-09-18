#include "netgame.h"
#include<QDebug>
#include<qwidget.h>
NetGame::NetGame(bool server)
{

    _server = 0;
    _socket = 0;
    if(server){
        _server = new QTcpServer(this);
        _server->listen(QHostAddress::Any,9999);
       // turn = 1;
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    else {
        _socket = new QTcpSocket(this);
       // turn = 0;
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);
        connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));


}
}
//重载函数以传输坐标
void NetGame::click(int x,int y){

    chessForm::click(x,y);
     char buf[2];
    buf[0]=x;
    buf[1]=y;
    //qDebug()<<x<<y;
    _socket->write(buf,2);
    update();
    turn = 0;
    emit(turn_change(turn));



}
//接受坐标
int NetGame::slotRecv(){
  QByteArray  ba =  _socket->readAll();
  int Ba[2]={0};
  Ba[0] = (int)ba[0];
  Ba[1] = (int)ba[1];

  formchessdata[Ba[0]][Ba[1]] = -1*currentrole;
  if((s1(Ba[0],Ba[1])||s2(Ba[0],Ba[1])||s3(Ba[0],Ba[1])||s4(Ba[0],Ba[1])
      ||s5(Ba[0],Ba[1])||s6(Ba[0],Ba[1])||s7(Ba[0],Ba[1])||s8(Ba[0],Ba[1]))){
      QMessageBox msg;
      msg.setText("you lose!");
      msg.exec();
  }
  chess->setchessstatus(formchessdata);
  turn = 1 ;
  emit turn_change(turn);
  return 1;
   update();

}
NetGame::~NetGame(){

}
//cs连接
void NetGame::slotNewConnection(){

    if(_socket) return;

    _socket = _server->nextPendingConnection();
    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));

    qDebug()<<"connect";

    //发送数据

}

