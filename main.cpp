#include "chessform.h"

#include <QApplication>
#include<netgame.h>
#include<QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(NULL,"server or client ","作为服务器启动");
    bool bserver = 0;
    if(ret == QMessageBox::Yes){
        bserver = 1;
    }
    NetGame w(bserver);
    w.show();
    return a.exec();
}
