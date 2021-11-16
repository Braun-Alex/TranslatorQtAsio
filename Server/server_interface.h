#pragma once
#ifndef SERVER_INTERFACE_H
#define SERVER_INTERFACE_H
#include <QMainWindow>
#include <QDateTime>
#include <fstream>
#include <iomanip>
#include <thread>
#include <boost/asio.hpp>
using namespace std;
using namespace boost;
QT_BEGIN_NAMESPACE
namespace Ui
{
class Server_Interface;
}
QT_END_NAMESPACE
class Server_Interface : public QMainWindow
{
Q_OBJECT
public:
Server_Interface(QWidget* the_widget,
                 const string &entered_address,
                 uint16_t entered_port);
~Server_Interface();
private slots:
void EnableServer();
void CheckLog();
private:
bool DoLog=false;
asio::io_context the_context;
asio::ip::tcp::endpoint the_endpoint;
asio::ip::tcp::socket the_socket;
asio::ip::tcp::acceptor the_acceptor;
system::error_code the_error_code;
Ui::Server_Interface* ui;
const string the_IP_of_the_server;
const uint16_t the_port_of_the_server;
string the_IP_of_the_client,the_port_of_the_client;
QMap<string,string> the_database={{"One","Cake"},
                                  {"Two","Banana"},
                                  {"Three","Chocolate"},
                                  {"Four","Coffee"},
                                  {"Five","Pizza"},
                                  {"Six","Burger"},
                                  {"Seven","Milk"},
                                  {"Eight","Bread"},
                                  {"Nine","Cacao"},
                                  {"Ten","Watermelon"},
                                  {"Eleven","Juice"},
                                  {"Twelve","Strawberry"},
                                  {"Thirteen","Tea"},
                                  {"Fourteen","Soup"},
                                  {"Fiveteen","Cream"}};
void Get(bool &the_connecting_status);
void DisableServer();
class Logger
{
public:
void OpenFile(const string &entered_path);
void LogToFile(const string &entered_message);
~Logger();
private:
ofstream the_file;
QDate the_date;
QTime the_time;
};
Logger the_logger;
};
#endif
