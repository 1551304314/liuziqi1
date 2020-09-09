#include "chessform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chessForm w;
    w.show();
    return a.exec();
}
