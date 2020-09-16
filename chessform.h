#ifndef CHESSFORM_H
#define CHESSFORM_H

#include <QWidget>
#include "widget.h"
#include<QPainter>
#include<QMessageBox>
namespace Ui {
class chessForm;
}

class chessForm : public QWidget
{
    Q_OBJECT

public:
    explicit chessForm(QWidget *parent = nullptr);
    ~chessForm();
    int isWin1(int ,int );//胜利判断1
    int isWin2(int ,int );//胜利判断2
    int isWin3(int ,int );
    int s1(int ,int);//胜利情况1-8
    int s2(int ,int);
    int s3(int ,int);
    int s4(int ,int);
    int s5(int ,int);
    int s6(int ,int);
    int s7(int ,int);
    int s8(int ,int);

    int isDead();//和棋判断
    int lian61(int ,int);
    int lian62(int ,int);
    int lian63(int ,int);
    int lian64(int ,int);
    int lian51(int ,int);//五连
    int lian52(int ,int);
    int lian53(int ,int);
    int lian54(int ,int);
    int lian41(int ,int);//活四
    int lian42(int ,int);
    int lian43(int ,int);
    int lian44(int ,int);
    int ban1(int,int);//55禁手
    int ban2(int,int);
    int ban3(int,int);//44禁手
    int ban4(int,int);
    int ban5(int,int);//长连禁手
    int ban6(int,int);

protected:
    void paintEvent(QPaintEvent *);//绘制ui背景

private slots:
    void on_btn_pvp_clicked();//人人对战按钮
    void doProcesschessdata(int i,int j);//数据处理核心
    void on_restartbtn_clicked();//重新开始按钮
    void on_btn_pvc_clicked();

    void on_btn_cvc_clicked();

private:
    Ui::chessForm *ui;
    Widget * chess;
    int formchessdata[20][20];
    void Init();//数据初始化
    void roleinit(const QString whitefilename,const QString blackfilename);//下棋方初始化
    void setrole(Widget::chesstype currentrole);//设置下棋方
    Widget::chesstype currentrole;//当前棋色
    Widget::chesstype firstrole = Widget::black;//先手
    void setfirstrole(Widget::chesstype firstrole);//保存先手
    void setchessinit();//棋盘初始化
    void rolechange();//交替下棋
    int  judge(int,int,Widget::chesstype);//落子判断
    void actionByAI(int &clickRow, int &clickCol); // 机器执行下棋
    std::vector<std::vector<int>> scoreMapVec; // 存储各个点位的评分情况，作为AI下棋依据
    enum playertype{pvp,pvc,nvn,cvc};
    playertype currentplayer;//所选游戏模式
    bool playerFlag;//AI棋色
    void calculateScore();//评分算法
    void updatechessdata(int x, int y); // 每次落子后更新游戏棋盘
    void updatechessdata1(int x,int y);


};
#endif // CHESSFORM_H
