#include "chessform.h"
#include "ui_chessform.h"
#include<QDebug>
chessForm::chessForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::chessForm)
{
    ui->setupUi(this);
    Init();
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
    //connect(chess,SIGNAL(SignalSendChessData(int,int)),this,SLOT(judge(int,int,int *,widget::chesstype)));
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
    painter.drawPixmap(this->rect(),QPixmap("3.jpg"));


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
//数据处理
void chessForm::doProcesschessdata(int i,int j){
    qDebug()<<"i:"<<i<<"j:"<<j;
    int rec = judge(i,j,currentrole);
    if(rec){
        chess->setchessstatus(formchessdata);
        rolechange();
    }
}
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
//合法性判断
int  chessForm::judge(int i,int j,Widget::chesstype a){
    chessForm::formchessdata[i][j]=a;
    return 1;
}
//胜利总判断
int chessForm::isWin(int x,int y)
{
    return s1(x,y)||s2(x,y)||s3(x,y)||s4(x,y);
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
                   formchessdata[x + i][y - i] == formchessdata[x - 5 + i][y + 4 - i])
                   return 1;
            }
            return 0;

    }
//和棋
int  chessForm::isDead()
{
    // 所有空格全部填满
    for (int i = 1; i < 20; i++)
        for (int j = 1; j < 20; j++)
        {
            if (!(formchessdata[i][j] == Widget::white || formchessdata[i][j] == Widget::black))
                return 0;
        }
    return 1;
}


//bool chessForm::judge(int x,int y){

//}
