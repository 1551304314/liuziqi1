#ifndef CHESSFORM_H
#define CHESSFORM_H

#include <QWidget>
#include "widget.h"
#include<QPainter>
namespace Ui {
class chessForm;
}

class chessForm : public QWidget
{
    Q_OBJECT

public:
    explicit chessForm(QWidget *parent = nullptr);
    ~chessForm();
    int isWin(int ,int );
    int s1(int ,int);
    int s2(int ,int);
    int s3(int ,int);
    int s4(int ,int);
    int isDead();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_btn_pvp_clicked();
    void doProcesschessdata(int i,int j);
    int  judge(int,int,Widget::chesstype);

private:
    Ui::chessForm *ui;
    Widget * chess;
    int formchessdata[20][20];
    void Init();
    void roleinit(const QString whitefilename,const QString blackfilename);
    void setrole(Widget::chesstype currentrole);
    Widget::chesstype currentrole;
    Widget::chesstype firstrole;
    void setfirstrole(Widget::chesstype firstrole);
    void setchessinit();//棋盘初始化
    void rolechange();



};
#endif // CHESSFORM_H
