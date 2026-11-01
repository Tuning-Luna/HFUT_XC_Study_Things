#include <QtGui/QApplication>
#include "uva.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    uva foo;
    foo.show();
    return app.exec();
}
