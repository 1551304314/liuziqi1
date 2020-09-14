#include "chessform.h"
#include "ui_chessform.h"
#include<QDebug>
#include<QMessageBox>
chessForm::chessForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chessForm)
{
    ui->setupUi(this);
    Init();
    setchessinit();
    setrole(Widget::black);
}

chessForm::~chessForm()
{
    delete ui;
}
//------------------初始化区-----------------//
//ui棋盘初始化
void chessForm::Init(){
    //申请棋盘空间
    chess = new Widget();
    //信号连接
    connect(chess,SIGNAL(SignalSendChessData(int,int)),this,SLOT(doProcesschessdata(int,int)));
    //把棋盘显示到当前容器中
    ui->gridLayout->addWidget(chess);
    //初始化选择器
   QStringList array;
   array.append("黑子先");
   array.append("白子先");
   ui->cbox_item->addItems(array);
   roleinit("1.png","2.png");
}
//黑白子初始化
void chessForm::roleinit(const QString whitefilename, const QString blackfilename)
{
    ui->label1->setPixmap(QPixmap(whitefilename));
    ui->label1->setScaledContents(1);
    ui->label2->setPixmap(QPixmap(blackfilename));
    ui->label2->setScaledContents(1);

}
//棋盘数组初始化
void chessForm::setchessinit(){
    memset(formchessdata,Widget::empty,sizeof(int)*400);
    chess->setchessstatus(formchessdata);
}
//---------------功能区-----------------//

//绘制背景
void chessForm::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.drawPixmap(this->rect(),QPixmap("bg1.jpg"));


}
//人人对战按钮
void chessForm::on_btn_pvp_clicked()
{
    if(ui->cbox_item->currentText().contains("白")){
        setrole(Widget::white);
        setfirstrole(Widget::white);
    }
    else
    {
        setrole(Widget::black);
        setfirstrole(Widget::black);
    }
    setchessinit();
}
//手动刷新棋盘
void chessForm::on_restartbtn_clicked()
{

    setchessinit();
    setrole(Widget::black);
}

//数据处理***************核心区************
void chessForm::doProcesschessdata(int i,int j){
    //qDebug()<<"i:"<<i<<"j:"<<j;
    int rec = judge(i,j,currentrole);
    if(rec){
        chess->setchessstatus(formchessdata);
        rolechange();
        isDead();
        if(firstrole==Widget::black)
           { int a = ban1(i,j);
             ban3(i,j);
             int c = ban5(i,j);
            if(a)
                isWin2(i,j);
            else if(c)
                isWin3(i,j);
            else
                isWin1(i,j);
        }
        else
        {
           int b = ban2(i,j);
            ban4(i,j);
           int d = ban6(i,j);
            if(b)
                isWin2(i,j);
            else if(d)
                isWin3(i,j);
            else
                isWin1(i,j);
        }
    }
}
//*******************************
//角色处理
void chessForm::setrole(Widget::chesstype role)
{
    currentrole=role;
    if(role==Widget::white)
    {
            ui->label1->setVisible(true);
            ui->label2->setVisible(false);
    }
    else
    {
            ui->label1->setVisible(false);
            ui->label2->setVisible(true);
}
}
//保存先手
void chessForm::setfirstrole(Widget::chesstype role)
{
    firstrole=role;
}
//棋子轮换
void chessForm::rolechange(){
    if(currentrole==Widget::white)
    {
        currentrole=Widget::black;
    }
    else
        currentrole=Widget::white;
}

//------------------逻辑区--------------------//
//落子判断
int  chessForm::judge(int i,int j,Widget::chesstype a){
    if(!chessForm::formchessdata[i][j]){
        chessForm::formchessdata[i][j]=a;
    return 1;}
    return 0;
}
//黑55禁手
int chessForm::ban1(int x, int y){
    QMessageBox msgbox;


    if((lian51(x,y)+lian52(x,y)+lian53(x,y)+lian54(x,y)>=2)&&formchessdata[x][y]==Widget::black){
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
        return 1;
    }
    else
        return 0;
    ;
}
//白55禁手
int chessForm::ban2(int x, int y){
    QMessageBox msgbox;
    if((lian51(x,y)+lian52(x,y)+lian53(x,y)+lian54(x,y)>=2)&&formchessdata[x][y]==Widget::white){
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
        return 1;
    }
    else
        return 0;
    ;
}
//黑44禁手
int chessForm::ban3(int x,int y){

    QMessageBox msgbox;
    if((lian41(x,y)+lian42(x,y)+lian43(x,y)+lian44(x,y)>=2)&&formchessdata[x][y]==Widget::black){
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
    }
    return 1;
}
//白44禁手
int chessForm::ban4(int x, int y){

    QMessageBox msgbox;
    if((lian41(x,y)+lian42(x,y)+lian43(x,y)+lian44(x,y)>=2)&&formchessdata[x][y]==Widget::white){
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
    }
    return 1;
}
//白长连禁手
int chessForm::ban6(int x, int y){

    QMessageBox msgbox;
    if((lian61(x,y)||lian62(x,y)||lian63(x,y)||lian64(x,y))&&formchessdata[x][y]==Widget::white)
    {
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
        return 1;

    }
    else return 0;
}
//黑长连禁手
int chessForm::ban5(int x, int y){

    QMessageBox msgbox;
    if((lian61(x,y)||lian62(x,y)||lian63(x,y)||lian64(x,y))&&formchessdata[x][y]==Widget::black)
    {
        msgbox.setText("this point is banned,you lost the game");
        msgbox.exec();
        return 1;
    }
    else return 0;
}
//和棋
int  chessForm::isDead()
{   int m=1;
    QMessageBox msgbox;
    for (int i = 1; i < 20; i++)
        for (int j = 1; j < 20; j++)
        {
            if (formchessdata[i][j]==Widget::empty)
                 m=0;



        }
    if(m){
    msgbox.setText("this game is dead!");
    msgbox.setInformativeText("please click the restart button!");
    msgbox.exec();
    }
    return 1;
}

//无禁手冲突时胜利
int chessForm::isWin1(int x,int y)
{   QMessageBox msgbox;
    QString str;
    if(formchessdata[x][y]==Widget::black)
            str="blackchess";
    else
        str="whitechess";
    if(s1(x,y)||s2(x,y)||s3(x,y)||s4(x,y)||s5(x,y)||s6(x,y)||s7(x,y)||s8(x,y))
    {   msgbox.setText(str+" is win!");
        msgbox.exec();
    }
     return 1;
}
//五五禁手冲突时胜利
int chessForm::isWin2(int x, int y)
{
    QMessageBox msgbox;
        QString str;
        if(formchessdata[x][y]==Widget::black)
                str="blackchess";
        else
            str="whitechess";
        if(s1(x,y)||s2(x,y)||s3(x,y)||s4(x,y))
        {   msgbox.setText(str+" is win!");
            msgbox.exec();
        }
         return 1;
}
//长连禁手冲突时胜利
int chessForm::isWin3(int x, int y)
{
    QMessageBox msgbox;
        QString str;
        if(formchessdata[x][y]==Widget::black)
                str="blackchess";
        else
            str="whitechess";
        if(s5(x,y)||s6(x,y)||s7(x,y)||s8(x,y))
        {   msgbox.setText(str+" is win!");
            msgbox.exec();
        }
         return 1;
}

//y方向活四
int chessForm::lian41(int x, int y){

    for(int i=0;i<4;i++)
    {
        if(y - i - 1 >= 0 &&
                   y + 4 - i <= 20 &&
                   formchessdata[x][y - 1 - i] == Widget::empty &&
                   formchessdata[x][y - i] == formchessdata[x][y + 1 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 2 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 3 - i] &&
                   formchessdata[x][y + 4 - i] == Widget::empty )
                   return 1;
            }
            return 0;
}
//x方向活四
int chessForm::lian42(int x, int y){

    for(int i=0;i<4;i++)
    {
        if(x - i -1 >= 0 &&
                   x + 4 - i <= 20 &&
                   formchessdata[x - 1 - i][y] == Widget::empty &&
                   formchessdata    [x - i][y] == formchessdata[x + 1 - i][y] &&
                   formchessdata    [x - i][y] == formchessdata[x + 2 - i][y] &&
                   formchessdata    [x - i][y] == formchessdata[x + 3 - i][y] &&
                   formchessdata[x + 4 - i][y] == Widget::empty )
                   return 1;
            }
            return 0;
}
//xy方向活四
int chessForm::lian43(int x, int y){

    for(int i=0;i<4;i++)
    {
        if(x - i - 1>= 0 &&
                   y - i -1>= 0 &&
                   x + 4 - i <= 20 &&
                   y + 4 - i <= 20 &&
                   formchessdata[x - 1 -i][y - 1 -i] == Widget::empty &&
                   formchessdata[x - i][y - i] == formchessdata[x + 1 - i][y + 1 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 2 - i][y + 2 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 3 - i][y + 3 - i] &&
                   formchessdata[x + 4 - i][y + 4 - i] == Widget::empty )
                   return 1;
            }
            return 0;
}
//yx方向活四
int chessForm::lian44(int x, int y){

    for(int i=0;i<4;i++)
    {
        if(x + i + 1<= 20 &&
                   y - i -1>= 0 &&
                   x - 4 + i >= 0 &&
                   y + 4 - i <= 20 &&
                   formchessdata[x + i + 1][y - i -1] == Widget::empty &&
                   formchessdata[x + i][y - i] == formchessdata[x - 1 + i][y + 1 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 2 + i][y + 2 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 3 + i][y + 3 - i] &&
                   formchessdata[x - 4 + i][y + 4 - i] == Widget::empty )
                   return 1;
            }
            return 0;
}
//y方向五连
int chessForm::lian51(int x,int y){

    for(int i=0;i<5;i++)
    {
        if(y - i - 1>= 0 &&
                   y + 5 - i <= 20 &&
                   ((formchessdata[x][y - i - 1] == Widget::empty&&formchessdata[x][y - i + 5] == Widget::empty)||
                    (formchessdata[x][y - i - 1] == Widget::empty||formchessdata[x][y - i + 5] == Widget::empty))&&
                   formchessdata[x][y - i] == formchessdata[x][y + 1 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 2 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 3 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 4 - i])
                   return 1;
            }
            return 0;
}
//x方向五连
int chessForm::lian52(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(x - i - 1>= 0 &&
                   x + 5 - i <= 20 &&
                   ((formchessdata[x - i - 1][y] == Widget::empty&&formchessdata[x - i + 5][y] == Widget::empty)||
                   (formchessdata[x - i - 1][y] == Widget::empty||formchessdata[x - i + 5][y] == Widget::empty))&&
                   formchessdata[x - i][y] == formchessdata[x + 1 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 2 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 3 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 4 - i][y] )
                   return 1;
            }
            return 0;
}
//xy方向五连
int chessForm::lian53(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(x - i - 1>= 0 &&
                   y - i - 1 >= 0 &&
                   x + 5 - i <= 20 &&
                   y + 5 - i <= 20 &&
                   ((formchessdata[x - i - 1][y - i - 1] == Widget::empty&&formchessdata[x - i + 5][y - i + 5] == Widget::empty)||
                   (formchessdata[x - i - 1][y - i - 1] == Widget::empty||formchessdata[x - i + 5][y - i + 5] == Widget::empty))&&
                   formchessdata[x - i][y - i] == formchessdata[x + 1 - i][y + 1 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 2 - i][y + 2 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 3 - i][y + 3 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 4 - i][y + 4 - i] )
                   return 1;
            }
            return 0;
}
//yx方向五连
int chessForm::lian54(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(x + i + 1<= 20 &&
                   y - i - 1>= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i <= 20 &&
                   ((formchessdata[x + i + 1][y - i - 1] == Widget::empty&&formchessdata[x + i - 5][y - i + 5] == Widget::empty)||
                   (formchessdata[x + i + 1][y - i - 1] == Widget::empty||formchessdata[x + i - 5][y - i + 5] == Widget::empty))&&
                   formchessdata[x + i][y - i] == formchessdata[x - 1 + i][y + 1 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 2 + i][y + 2 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 3 + i][y + 3 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 4 + i][y + 4 - i] )
                   return 1;
            }
            return 0;
}
//y方向七连
int chessForm::lian61(int x, int y){

    for(int i=0;i<7;i++)
    {
        if(y - i >= 0 &&
                   y + 6 - i <= 20 &&
                   formchessdata[x][y - i] == formchessdata[x][y + 1 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 2 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 3 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 4 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 5 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 6 - i] )
                   return 1;
            }
            return 0;
}
//x方向七连
int chessForm::lian62(int x,int y){

    for(int i=0;i<7;i++)
    {
        if(x - i >= 0 &&
                   x + 6 - i <= 20 &&
                   formchessdata[x - i][y] == formchessdata[x + 1 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 2 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 3 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 4 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 5 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 6 - i][y] )
                   return 1;
            }
            return 0;
}
//xy方向七连
int chessForm::lian63(int x, int y){

    for(int i=0;i<7;i++)
    {
        if(x - i >= 0 &&
                   y - i >= 0 &&
                   x + 6 - i <= 20 &&
                   y + 6 - i <= 20 &&
                   formchessdata[x - i][y - i] == formchessdata[x + 1 - i][y + 1 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 2 - i][y + 2 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 3 - i][y + 3 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 4 - i][y + 4 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 5 - i][y + 5 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 6 - i][y + 6 - i] )
                   return 1;
            }
            return 0;
}
//yx方向七连
int chessForm::lian64(int x, int y){

    for(int i=0;i<7;i++)
    {
        if(x + i <= 20 &&
                   y - i >= 0 &&
                   x - 6 + i >= 0 &&
                   y + 6 - i <= 20 &&
                   formchessdata[x + i][y - i] == formchessdata[x - 1 + i][y + 1 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 2 + i][y + 2 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 3 + i][y + 3 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 4 + i][y + 4 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 5 + i][y + 5 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 6 + i][y + 6 - i] )
                   return 1;
            }
            return 0;
}
//胜利1
int chessForm::s1(int x, int y){
    for(int i=0;i<6;i++)
    {
        if(y - i >= 0 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x][y - i] == formchessdata[x][y + 1 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 2 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 3 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 4 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 5 - i])
                   return 1;
            }
            return 0;
}
//胜利2
int chessForm::s2(int x, int y){
    for(int i=0;i<6;i++)
    {
        if(x - i >= 0 &&
                   x + 5 - i <= 20 &&
                   formchessdata[x - i][y] == formchessdata[x + 1 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 2 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 3 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 4 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 5 - i][y])
                   return 1;
            }
            return 0;
    }
//胜利3
int chessForm::s3(int x, int y){
    for(int i=0;i<6;i++)
    {
        if(x - i >= 0 &&
                   y - i >= 0 &&
                   x + 5 - i <= 20 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x - i][y - i] == formchessdata[x + 1 - i][y + 1 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 2 - i][y + 2 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 3 - i][y + 3 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 4 - i][y + 4 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 5 - i][y + 5 - i])
                   return 1;
            }
            return 0;
    }
//胜利4
int chessForm::s4(int x, int y){
    for(int i=0;i<6;i++)
    {
        if(x + i <= 20 &&
                   y - i >= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x + i][y - i] == formchessdata[x - 1 + i][y + 1 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 2 + i][y + 2 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 3 + i][y + 3 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 4 + i][y + 4 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 5 + i][y + 5 - i])
                   return 1;
            }
            return 0;

    }
//胜利5
int chessForm::s5(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(y - i -1 >= 0 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x][y - i -1] == Widget::empty&&
                   formchessdata[x][y - i] == formchessdata[x][y + 1 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 2 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 3 - i] &&
                   formchessdata[x][y - i] == formchessdata[x][y + 4 - i] &&
                   formchessdata[x][y + 5 - i] == Widget::empty)
                   return 1;
            }
            return 0;
}
//胜利6
int chessForm::s6(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(x - i - 1>= 0 &&
                   x + 5 - i <= 20 &&
                   formchessdata[x - i - 1][y] == Widget::empty&&
                   formchessdata[x - i][y] == formchessdata[x + 1 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 2 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 3 - i][y] &&
                   formchessdata[x - i][y] == formchessdata[x + 4 - i][y] &&
                   formchessdata[x + 5 - i][y] == Widget::empty )
                   return 1;
            }
            return 0;
}
//胜利7
int chessForm::s7(int x,int y){

    for(int i=0;i<5;i++)
    {
        if(x - i - 1>= 0 &&
                   y - i - 1 >= 0 &&
                   x + 5 - i <= 20 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x - i - 1][y - i - 1] == Widget::empty&&
                   formchessdata[x - i][y - i] == formchessdata[x + 1 - i][y + 1 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 2 - i][y + 2 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 3 - i][y + 3 - i] &&
                   formchessdata[x - i][y - i] == formchessdata[x + 4 - i][y + 4 - i] &&
                   formchessdata[x - i + 5][y - i + 5] ==Widget::empty )
                   return 1;
            }
            return 0;
}
//胜利8
int chessForm::s8(int x, int y){

    for(int i=0;i<5;i++)
    {
        if(x + i <= 20 &&
                   y - i - 1>= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i <= 20 &&
                   formchessdata[x + i + 1][y - i - 1] == Widget::empty &&
                   formchessdata[x + i][y - i] == formchessdata[x - 1 + i][y + 1 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 2 + i][y + 2 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 3 + i][y + 3 - i] &&
                   formchessdata[x + i][y - i] == formchessdata[x - 4 + i][y + 4 - i] &&
                   formchessdata[x + i - 5][y - i +5] == Widget::empty )
                   return 1;
            }
            return 0;
}


