#pragma once
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H
#include <QMainWindow>
#include <QDateTime>
#include <fstream>
#include <iomanip>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
using namespace std;
using namespace boost;
QT_BEGIN_NAMESPACE
namespace Ui { class User_Interface; }
QT_END_NAMESPACE
class User_Interface : public QMainWindow
{
Q_OBJECT
public:
User_Interface(QWidget* the_widget=nullptr);
~User_Interface();
private slots:
void InitiateConnection();
void InitiateTranslating();
void InitiateDisconnection();
void CheckLog();
private:
bool DoLog=false;
asio::io_context the_context;
asio::ip::tcp::socket the_socket;
system::error_code the_error_code;
Ui::User_Interface* ui;
QCursor the_normal_cursor,the_blocked_cursor;
bool connect(const string &entered_address,uint16_t entered_port);
bool send(const string &entered_request);
pair<bool,string> receive();
void disconnect();
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
