#include "user_interface.h"
#include <QApplication>
using namespace std;
int main(int argc, char *argv[])
{
    QApplication the_application(argc, argv);
    User_Interface the_interface;
    the_interface.show();
    return the_application.exec();
}
