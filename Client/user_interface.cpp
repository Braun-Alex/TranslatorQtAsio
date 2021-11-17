#include "user_interface.h"
#include "./ui_user_interface.h"
using namespace std;
const QString BLACK_COLOR="color: rgb(0, 0, 0);",
              GREEN_COLOR="color: rgb(0, 255, 127);",
              RED_COLOR="color: rgb(255, 0, 0);";
User_Interface::User_Interface(QWidget* the_widget):the_socket(the_context),
QMainWindow(the_widget),ui(new Ui::User_Interface)
{
    ui->setupUi(this);
    ui->Information->hide();
    ui->Entered_word->hide();
    ui->Received_word->hide();
    ui->Receiving->hide();
    ui->Sending->hide();
    ui->Translate->hide();
    ui->Disconnect->hide();
    ui->Connecting->hide();
    ui->File->setText("C:/Users/Lamaba/Desktop/Client/Log.txt");
    ui->File->hide();
    ui->Text_to_file->hide();
    this->setWindowTitle("Client");
    ui->Received_word->setReadOnly(true);
}
void User_Interface::InitiateConnection()
{
    if (ui->Log->checkState()==Qt::CheckState::Checked&&!DoLog)
    {
        const QString &the_text=ui->File->text();
        if (the_text.isEmpty())
        {
            ui->Text_to_file->setStyleSheet(RED_COLOR);
            ui->Text_to_file->setText("The field is empty!");
            return;
        }
        if (the_text.indexOf(".txt")==-1)
        {
            ui->Text_to_file->setStyleSheet(RED_COLOR);
            ui->Text_to_file->setText("The file must be .txt file!");
            return;
        }
        ui->Log->hide();
        ui->File->hide();
        ui->Text_to_file->hide();
        the_logger.OpenFile(the_text.toStdString());
        DoLog=true;
    }
    else if (!ui->Log->isHidden()) ui->Log->hide();
    ui->Text_for_IP->show();
    ui->Text_for_port->show();
    bool DoConnect=true;
    string the_IP=ui->IP->text().toStdString();
    QString the_copy_of_port=ui->Port->text();
    if (the_IP.empty())
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("The field is empty!");
        if (DoLog) the_logger.LogToFile(
        "The client entered nothing to server's IP field and released \"Connect\" button");
    }
    if (the_copy_of_port.isEmpty())
    {
        ui->Text_for_port->setStyleSheet(RED_COLOR);
        ui->Text_for_port->setText("The field is empty!");
        if (DoLog) the_logger.LogToFile(
        "The client entered nothing to server's port field and released \"Connect\" button");
    }
    if (the_IP.empty()&&the_copy_of_port.isEmpty()) return;
    bool IsCorrectIP=false;
    if (!the_IP.empty())
    {
    DoConnect=true;
    IsCorrectIP=true;
    istringstream entered_IP(the_IP);
    int value;
    while (DoConnect)
    {
    if (ui->IP->text().count('.')!=3||*ui->IP->text().rbegin()=='.')
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("Entered incorrect IP!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
        IsCorrectIP=false;
        break;
    }
    if (!(entered_IP>>value)||value<0||value>255)
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("Entered incorrect IP!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
        IsCorrectIP=false;
        break;
    }
    char the_point;
    the_point=entered_IP.peek();
    entered_IP.ignore(1);
    if (the_point!='.'||!(entered_IP>>value)||
             value<0||value>255)
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("Entered incorrect IP!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
        IsCorrectIP=false;
        break;
    }
    the_point=entered_IP.peek();
    entered_IP.ignore(1);
    if (the_point!='.'||!(entered_IP>>value)||
             value<0||value>255)
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("Entered incorrect IP!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
        IsCorrectIP=false;
        break;
    }
    the_point=entered_IP.peek();
    entered_IP.ignore(1);
    if (the_point!='.'||!(entered_IP>>value)||
            value<0||value>255)
    {
        ui->Text_for_IP->setStyleSheet(RED_COLOR);
        ui->Text_for_IP->setText("Entered incorrect IP!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
    }
    ui->Text_for_IP->setStyleSheet(GREEN_COLOR);
    ui->Text_for_IP->setText("All is right!");
    if (DoLog) the_logger.LogToFile(
    "The client entered correct IP \""+the_IP+"\" to server's IP field and released \"Connect\" button");
    DoConnect=false;
    }
    }
    uint16_t the_port=the_copy_of_port.toUShort();
    if (!the_copy_of_port.isEmpty())
    {
    DoConnect=true;
    if (the_port==0)
    {
        ui->Text_for_port->setStyleSheet(RED_COLOR);
        ui->Text_for_port->setText("Entered incorrect port!");
        if (DoLog) the_logger.LogToFile(
        "The client entered incorrect port \""+
        the_copy_of_port.toStdString()+"\" to server's port field and released \"Connect\" button");
        DoConnect=false;
    }
    if (DoConnect)
    {
        ui->Text_for_port->setStyleSheet(GREEN_COLOR);
        ui->Text_for_port->setText("All is right!");
        if (DoLog) the_logger.LogToFile(
        "The client entered correct port \""+
        the_copy_of_port.toStdString()+"\" to server's port field and released \"Connect\" button");
    }
    }
    if (DoConnect&&IsCorrectIP)
    {
        ui->Text_for_IP->hide();
        ui->Text_for_port->hide();
        ui->Connecting->show();
        DoConnect=connect(the_IP,the_port);
        if (!DoConnect)
        {
            ui->Connecting->setStyleSheet(RED_COLOR);
            ui->Connecting->setText("Connection failed. Try again");
            if (DoLog) the_logger.LogToFile(
            "Connection failed. The client could not connected to the server");
        }
        else
        {
            ui->Connecting->setStyleSheet(GREEN_COLOR);
            ui->Connecting->setText("Successfully connected!");
            ui->Status->setStyleSheet(GREEN_COLOR);
            ui->Status->setText("Status: connected to server with IP "+
                ui->IP->text()+" and port "+ui->Port->text()+"...");
            the_logger.LogToFile("The client successfully connected to the server");
            ui->Entered_word->show();
            ui->Disconnect->show();
            ui->Received_word->show();
            ui->Receiving->show();
            ui->Sending->show();
            ui->Translate->show();
            ui->IP->clear();
            ui->IP->setReadOnly(true);
            ui->Port->clear();
            ui->Port->setReadOnly(true);
            ui->Connection->setDisabled(true);
        }
    }
}
void User_Interface::InitiateTranslating()
{
string the_word=ui->Entered_word->text().toStdString();
if (the_word.empty())
{
    ui->Sending->setStyleSheet(RED_COLOR);
    ui->Sending->setText("The field is empty!");
    the_logger.LogToFile(
    "The client entered nothing to field with translating word and released \"Translate\" button");
    return;
}
ui->Sending->setStyleSheet(BLACK_COLOR);
ui->Sending->setText("Enter the word to translating");
the_logger.LogToFile(
"The client entered word \""+the_word+"\" to field with translating word and released \"Translate\" button");
if (!send(the_word))
{
    ui->Status->setStyleSheet(RED_COLOR);
    ui->Status->setText("Status: problems with connections...");
    ui->Sending->setStyleSheet(RED_COLOR);
    ui->Sending->setText("Problem with connection!");
    ui->Connecting->setStyleSheet(RED_COLOR);
    ui->Connecting->setText("Server does not answer!");
    the_logger.LogToFile(
    "Problem with connection: server does not answer");
    return;
}
const pair<bool,string> the_pair=receive();
if (!the_pair.first)
{
    ui->Receiving->setStyleSheet(RED_COLOR);
    ui->Receiving->setText("Problems with receiving of socket!");
    the_logger.LogToFile(
    "The client could not receive socket");
    return;
}
ui->Received_word->setText(QString::fromStdString(the_pair.second));
the_logger.LogToFile(
"The client received translating word \""+
the_word+"\" as \""+the_pair.second+"\" from the server");
}
void User_Interface::InitiateDisconnection()
{
    ui->Status->setStyleSheet(RED_COLOR);
    ui->Status->setText("Status: disconnected...");
    ui->Entered_word->clear();
    ui->Received_word->clear();
    ui->Sending->setStyleSheet(BLACK_COLOR);
    ui->Sending->setText("Enter the word to translating");
    ui->Sending->hide();
    ui->Disconnect->hide();
    ui->Entered_word->hide();
    ui->Receiving->setStyleSheet(BLACK_COLOR);
    ui->Receiving->setText("The translated word by server");
    ui->Receiving->hide();
    ui->Received_word->hide();
    ui->Connecting->hide();
    ui->Translate->hide();
    ui->Text_for_IP->setStyleSheet(BLACK_COLOR);
    ui->Text_for_IP->setText("Enter IP of the server");
    ui->Text_for_IP->show();
    ui->Text_for_port->setStyleSheet(BLACK_COLOR);
    ui->Text_for_port->setText("Enter port of the server");
    ui->Text_for_port->show();
    ui->IP->clear();
    ui->Port->clear();
    ui->IP->setReadOnly(false);
    ui->Port->setReadOnly(false);
    ui->Connection->setDisabled(false);
    disconnect();
    return;
}
void User_Interface::CheckLog()
{
    if (ui->Log->checkState()==Qt::CheckState::Checked)
    {
        ui->File->show();
        ui->Text_to_file->show();
    }
    else
    {
        ui->File->setText("C:/Users/Lamaba/Desktop/Client/Log.txt");
        ui->File->hide();
        ui->Text_to_file->setStyleSheet(BLACK_COLOR);
        ui->Text_to_file->setText(
                    "Enter the directory file if necessary");
        ui->Text_to_file->hide();
    }
}
bool User_Interface::connect(const string &entered_address,
                             uint16_t entered_port)
{
if (the_socket.is_open()) the_socket.close(the_error_code);
the_socket.connect(boost::asio::ip::tcp::
endpoint(boost::asio::ip::make_address(entered_address,the_error_code),
         entered_port),
the_error_code);
return !static_cast<bool>(the_error_code);
}
bool User_Interface::send(const string &entered_request)
{
if (!the_error_code&&the_socket.is_open())
the_socket.write_some(boost::asio::buffer(entered_request),
                      the_error_code);
return !static_cast<bool>(the_error_code);
}
pair<bool,string> User_Interface::receive()
{
the_socket.wait(the_socket.wait_read,the_error_code);
size_t the_bytes=the_socket.available(the_error_code);
if (!the_error_code)
{
string the_answer;
the_answer.resize(the_bytes);
the_socket.read_some(boost::asio::buffer(the_answer),the_error_code);
return make_pair(true,the_answer);
}
return make_pair(false,"");
}
void User_Interface::disconnect()
{
send("Stop");
if (the_socket.is_open())
{
the_socket.close(the_error_code);
the_logger.LogToFile(
"The client has been disconnected from the server");
}
}
User_Interface::~User_Interface()
{
    delete ui;
    disconnect();
}
void User_Interface::Logger
::OpenFile(const string &entered_path)
{
if (the_file.is_open()) the_file.close();
the_file.open(entered_path,ios_base::app);
the_file<<setw(111)<<setfill('*')<<'\n';
LogToFile(
"The client has been launched");
}
void User_Interface::Logger::LogToFile(const string &entered_message)
{
the_file<<entered_message<<". Date: "<<
the_date.currentDate().toString().toStdString()<<". Time: "<<
the_time.currentTime().toString().toStdString()<<'\n';
}
User_Interface::Logger::~Logger()
{
if (the_file.is_open())
{
    LogToFile("The client exited");
    the_file<<setw(111)<<setfill('*')<<'\n';
    the_file.close();
}
}
