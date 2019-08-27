
#include <QApplication>

#include "speed.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    speed s;
    s.show();
    return app.exec();
}
