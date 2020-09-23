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
        connect(_server,SIGNAL(newConnection()),this,SLOT(slotNewConnection()));
    }
    else {
        _socket = new QTcpSocket(this);
        _socket->connectToHost(QHostAddress("127.0.0.1"),9999);
        connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));

}
}
//重载函数以传输坐标
void NetGame::click(int x,int y){

    chessForm::click(x,y);
     char buf[3];
    buf[0]=1;
    buf[1]=x;
    buf[2]=y;
    _socket->write(buf,3);
    update();
    turn = 0;
    emit(turn_change(turn));



}
//接受坐标
int NetGame::slotRecv(){
  QByteArray  ba =  _socket->readAll();
  char cmd = ba[0];
  if(cmd == 1)
  {int Ba[2]={0};
  Ba[0] = (int)ba[1];
  Ba[1] = (int)ba[2];

  formchessdata[Ba[0]][Ba[1]] = -1*currentrole;

  //失败
  if((s1(Ba[0],Ba[1])||s2(Ba[0],Ba[1])||s3(Ba[0],Ba[1])||s4(Ba[0],Ba[1])
      ||s5(Ba[0],Ba[1])||s6(Ba[0],Ba[1])||s7(Ba[0],Ba[1])||s8(Ba[0],Ba[1]))){
      QMessageBox msg;
      msg.setText("you lose!");
      msg.exec();
  }
  chess->setchessstatus(formchessdata);
  turn = 1 ;
  emit turn_change(turn);
  update();
  }
  if(cmd == 2){
      QMessageBox msg;
      msg.setText("对方选择黑子，请选择白子");
      msg.exec();
      currentrole = Widget::white;
  }
  if(cmd == 3){
      QMessageBox msg;
      msg.setText("对方选择白子，请选择黑子");
      msg.exec();
      currentrole = Widget::black;
  }
return 1;
}
NetGame::~NetGame(){

}
//c/s连接
void NetGame::slotNewConnection(){

    if(_socket) return;
    _socket = _server->nextPendingConnection();
    connect(_socket,SIGNAL(readyRead()),this,SLOT(slotRecv()));
    qDebug()<<"connect";
    //发送先后手数据
    char buf[3];
    buf[0] = 2;
    buf[1] = 1;
    buf[2] = 0;
    char Buf[3];
    Buf[0] = 3;
    Buf[1] = -1;
    Buf[2] = 0;
    if(currentrole == Widget::black){
        _socket->write(buf,3);
    }
    else
        _socket->write(Buf,3);

}

