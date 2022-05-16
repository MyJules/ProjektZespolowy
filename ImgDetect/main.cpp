#include <mainwidget.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;

    QPalette pallete;
    pallete.setColor(QPalette::Window, Qt::gray);

    w.setAutoFillBackground(true);
    w.setPalette(pallete);
    w.show();
    return a.exec();
}

