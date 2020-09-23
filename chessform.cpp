#include "chessform.h"
#include "ui_chessform.h"
#include<QTimer>
#include<QMessageBox>
#include<QDebug>
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
    connect(chess,SIGNAL(SignalSendChessData(int,int)),this,SLOT(click(int,int)));
    //把棋盘显示到当前容器中
    ui->gridLayout->addWidget(chess);
    //初始化选择器
   QStringList array;
   array.append("黑子先");
   array.append("白子先");
   ui->cbox_item->addItems(array);
   roleinit("1.png","2.png");
   if(currentplayer==pvc||currentplayer == cvc){

       scoreMapVec.clear();
               for (int i = 0; i < 20; i++)
               {
                   std::vector<int> lineScores;
                   for (int j = 0; j < 20; j++)
                       lineScores.push_back(0);
                   scoreMapVec.push_back(lineScores);
               }
         playerFlag=1;

   }

}
//黑白子显示初始化
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
    currentplayer = pvp ;
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
//人机对战
void chessForm::on_btn_pvc_clicked()
{
    currentplayer = pvc ;
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
    playerFlag=1;
}
void chessForm::on_btn_cvc_clicked()
{

    currentplayer = cvc ;
    if(ui->cbox_item->currentText().contains("白")){
        setrole(Widget::white);
        setfirstrole(Widget::white);
        playerFlag=1;
    }
    else
    {
        setrole(Widget::black);
        setfirstrole(Widget::black);
        playerFlag=0;
    }
    setchessinit();

}

void chessForm::on_btn_nvn_clicked()
{

    currentplayer = nvn ;
    //turn = 1;
    if(ui->cbox_item->currentText().contains("白")){
        setrole(Widget::white);
        setfirstrole(Widget::white);
        turn = 0;
    }
    else
    {
        setrole(Widget::black);
        setfirstrole(Widget::black);
        turn = 1;
    }
    setchessinit();

}



//手动刷新棋盘
void chessForm::on_restartbtn_clicked()
{

    setchessinit();
    setrole(Widget::black);
}
//提供虚函数给netgame以传输坐标
void chessForm::click(int , int ){
     //qDebug()<<x<<y;
     update();
 }

//数据处理***************核心区************
void chessForm::doProcesschessdata(int i,int j){
    //qDebug()<<"i:"<<i<<"j:"<<j;
        if(currentplayer==pvp){
            int a = judge(i,j,currentrole);
            if(a){
            rolechange();
            chess->setchessstatus(formchessdata);
        isDead();
        if(firstrole==Widget::black)
           { int a = ban1(i,j);
             ban3(i,j);
             int c = ban5(i,j);
            if(a){
                isWin2(i,j);
                 }
            else if(c){
                isWin3(i,j);
            }
             else{
                isWin1(i,j);
            }
        }
        else
        {
           int b = ban2(i,j);
            ban4(i,j);
           int d = ban6(i,j);
            if(b){
                isWin2(i,j);
            }
            else if(d){

                isWin3(i,j);
            }
            else{
                isWin1(i,j);
                }
        }}}
        else if (currentplayer == pvc){
            int a = judge(i,j,currentrole);
            if(a){
            actionByAI(i,j);
            chess->setchessstatus(formchessdata);
            isDead();
            isWin1(i,j);

        }

}
     else if(currentplayer == cvc ){

        actionByAI(i,j);
        chess->setchessstatus(formchessdata);
        isDead();
        isWin1(i,j);
    }
        if(currentplayer==nvn){
            int a = judge(i,j,currentrole);
            if(a){
             click(i,j);
            chess->setchessstatus(formchessdata);
            isDead();
            isWin4(i,j);
}

        }
}


//*******************************
//人机刷新
void chessForm::updatechessdata(int x, int y)
{
    if (playerFlag){
        formchessdata[x][y] = 1;
        update();
}
}
//机机刷新
void chessForm::updatechessdata1(int x, int y){

        if(playerFlag){
            formchessdata[x][y] = 1;
        }
        else formchessdata[x][y] = -1;
        playerFlag = !playerFlag;
    }
//AI下棋
void chessForm::actionByAI(int &clickRow, int &clickCol)
{
    // 计算评分
    calculateScore();

    // 从评分中找出最大分数的位置
    int maxScore = 0;
    std::vector<std::pair<int, int>> maxPoints;

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
        {
            // 前提是这个坐标是空的
            if (formchessdata[i][j] == 0)
            {
                if (scoreMapVec[i][j] > maxScore)          // 找最大的数和坐标
                {
                    maxPoints.clear();
                    maxScore = scoreMapVec[i][j];
                    maxPoints.push_back(std::make_pair(i, j));
                }
                else if (scoreMapVec[i][j] == maxScore)     // 如果有多个最大的数，都存起来
                    maxPoints.push_back(std::make_pair(i, j));
            }
        }

    // 随机落子，如果有多个点的话
    srand((unsigned)time(0));
    int index = rand() % maxPoints.size();

    std::pair<int, int> pointPair = maxPoints.at(index);
    clickRow = pointPair.first; // 记录落子点
    clickCol = pointPair.second;
    if(currentplayer == pvc)updatechessdata(clickRow,clickCol);
    else updatechessdata1(clickRow,clickCol);
}
// 最关键的计算评分函数
void chessForm::calculateScore()
{
    // 统计玩家或者电脑连成的子
    int personNum = 0; // 玩家连成子的个数
    int botNum = 0; // AI连成子的个数
    int emptyNum = 0; // 各方向空白位的个数

    // 清空评分数组
    scoreMapVec.clear();
    for (int i = 0; i < 20; i++)
    {
        std::vector<int> lineScores;
        for (int j = 0; j < 20; j++)
            lineScores.push_back(0);
        scoreMapVec.push_back(lineScores);
    }

    // 计分
    for (int row = 0; row < 20; row++)
           for (int col = 0; col < 20; col++)
           {
               // 空白点就算
               if (row >= 0 && col >= 0 &&
                   formchessdata[row][col] == 0)
               {
                   // 遍历周围八个方向
                   for (int y = -1; y <= 1; y++)
                       for (int x = -1; x <= 1; x++)
                       {
                           // 重置
                           personNum = 0;
                           botNum = 0;
                           emptyNum = 0;

                           // 原坐标不算
                           if (!(y == 0 && x == 0))
                           {
                               // 每个方向延伸5个子

                               // 对玩家黑子评分（正反两个方向）
                               for (int i = 1; i <= 5; i++)
                               {
                                   if (row + i * y >= 0 && row + i * y < 20 &&
                                       col + i * x >= 0 && col + i * x < 20 &&
                                       formchessdata[row + i * y][col + i * x] == -1) // 玩家的子
                                   {
                                       personNum++;
                                   }
                                   else if (row + i * y >= 0 && row + i * y < 20 &&
                                            col + i * x >= 0 && col + i * x < 20 &&
                                            formchessdata[row + i * y][col + i * x] == 0) // 空白位
                                   {
                                       emptyNum++;
                                       break;
                                   }
                                   else            // 出边界
                                       break;
                               }

                               for (int i = 1; i <= 5; i++)
                               {
                                   if (row - i * y >= 0 && row - i * y < 20 &&
                                       col - i * x >= 0 && col - i * x < 20 &&
                                       formchessdata[row - i * y][col - i * x] == -1) // 玩家的子
                                   {
                                       personNum++;
                                   }
                                   else if (row - i * y >= 0 && row - i * y < 20 &&
                                            col - i * x >= 0 && col - i * x < 20 &&
                                            formchessdata[row - i * y][col - i * x] == 0) // 空白位
                                   {
                                       emptyNum++;
                                       break;
                                   }
                                   else            // 出边界
                                       break;
                               }

                               if (personNum == 1)                      // 杀二
                                   scoreMapVec[row][col] += 10;
                               else if (personNum == 2)                 // 杀三
                               {
                                   if (emptyNum == 1)
                                       scoreMapVec[row][col] += 30;
                                   else if (emptyNum == 2)
                                       scoreMapVec[row][col] += 40;
                               }
                               else if (personNum == 3)                 // 杀四
                               {
                                   // 量变空位不一样，优先级不一样
                                   if (emptyNum == 1)
                                       scoreMapVec[row][col] += 60;
                                   else if (emptyNum == 2)
                                       scoreMapVec[row][col] += 110;
                               }
                               else if (personNum == 4)                 // 杀五
                                   scoreMapVec[row][col] += 10100;
                               else if(personNum == 5)
                                   scoreMapVec[row][col] += 20100;      //杀六
                               // 进行一次清空
                               emptyNum = 0;

                               // 对AI白子评分
                               for (int i = 1; i <= 5; i++)
                               {
                                   if (row + i * y >= 0 && row + i * y < 20 &&
                                       col + i * x >= 0 && col + i * x < 20 &&
                                       formchessdata[row + i * y][col + i * x] == 1) // AI的子
                                   {
                                       botNum++;
                                   }
                                   else if (row + i * y >= 0 && row + i * y < 20 &&
                                            col + i * x >= 0 && col + i * x < 20 &&
                                            formchessdata[row +i * y][col + i * x] == 0) // 空白位
                                   {
                                       emptyNum++;
                                       break;
                                   }
                                   else            // 出边界
                                       break;
                               }

                               for (int i = 1; i <= 5; i++)
                               {
                                   if (row - i * y >= 0 && row - i * y < 20 &&
                                       col - i * x >= 0 && col - i * x < 20 &&
                                       formchessdata[row - i * y][col - i * x] == 1) // AI的子
                                   {
                                       botNum++;
                                   }
                                   else if (row - i * y >= 0 && row - i * y < 20 &&
                                            col - i * x >= 0 && col - i * x < 20 &&
                                            formchessdata[row - i * y][col - i * x] == 0) // 空白位
                                   {
                                       emptyNum++;
                                       break;
                                   }
                                   else            // 出边界
                                       break;
                               }

                               if (botNum == 0)                      // 普通下子
                                   scoreMapVec[row][col] += 5;
                               else if (botNum == 1)                 // 活二
                                   scoreMapVec[row][col] += 10;
                               else if (botNum == 2)
                               {
                                   if (emptyNum == 1)                // 死三
                                       scoreMapVec[row][col] += 25;
                                   else if (emptyNum == 2)
                                       scoreMapVec[row][col] += 50;  // 活三
                               }
                               else if (botNum == 3)
                               {
                                   if (emptyNum == 1)                // 死四
                                       scoreMapVec[row][col] += 55;
                                   else if (emptyNum == 2)
                                       scoreMapVec[row][col] += 100; // 活四
                               }
                               else if (botNum == 4)
                                   scoreMapVec[row][col] += 10000;   // 活五
                               else if (botNum >= 5)
                                   scoreMapVec[row][col] += 20000;  //活六
                           }
                       }

               }
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
    if(currentrole==Widget::white)
    {
            ui->label1->setVisible(true);
            ui->label2->setVisible(false);
    }
    else if(currentrole==Widget::black)
    {
            ui->label1->setVisible(false);
            ui->label2->setVisible(true);
}
}

//------------------逻辑区--------------------//
//落子判断
int  chessForm::judge(int i,int j,Widget::chesstype a){
    if(currentplayer == nvn){
        if(turn&&chessForm::formchessdata[i][j]==Widget::empty){
            chessForm::formchessdata[i][j]=a;

       return 1; }
        return 0;
    }
    else if(!chessForm::formchessdata[i][j]){
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

//和棋
int  chessForm::isDead()
{   int m=1;
    QMessageBox msgbox;
    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 20; j++)
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
//网络对战胜利
int chessForm::isWin4(int x, int y)
{
    QMessageBox msg;
    QString str;
    if(formchessdata[x][y] == currentrole)
        str="you win!";
    if((s1(x,y)||s2(x,y)||s3(x,y)||s4(x,y)||s5(x,y)||s6(x,y)||s7(x,y)||s8(x,y)))
    {
            msg.setText(str);
            msg.exec();
    }
    return 1;
}

//y方向活四
int chessForm::lian41(int x, int y){

    for(int i=0;i<4;i++)
    {
        if(y - i - 1 >= 0 &&
                   y + 4 - i < 20 &&
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
                   x + 4 - i < 20 &&
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
                   x + 4 - i < 20 &&
                   y + 4 - i < 20 &&
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
        if(x + i + 1< 20 &&
                   y - i -1>= 0 &&
                   x - 4 + i >= 0 &&
                   y + 4 - i < 20 &&
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
                   y + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
                   y + 5 - i < 20 &&
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
        if(x + i + 1< 20 &&
                   y - i - 1>= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i < 20 &&
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
                   y + 6 - i < 20 &&
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
                   x + 6 - i < 20 &&
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
                   x + 6 - i < 20 &&
                   y + 6 - i < 20 &&
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
        if(x + i < 20 &&
                   y - i >= 0 &&
                   x - 6 + i >= 0 &&
                   y + 6 - i < 20 &&
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
                   y + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
                   y + 5 - i < 20 &&
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
        if(x + i < 20 &&
                   y - i >= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i < 20 &&
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
                   y + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
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
                   x + 5 - i < 20 &&
                   y + 5 - i < 20 &&
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
        if(x + i + 1< 20 &&
                   y - i - 1>= 0 &&
                   x - 5 + i >= 0 &&
                   y + 5 - i < 20 &&
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

