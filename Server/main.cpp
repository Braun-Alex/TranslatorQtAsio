#include "server_interface.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication the_application(argc, argv);
    Server_Interface the_interface(nullptr,"127.0.0.1",1033);
    the_interface.show();
    return the_application.exec();
}
