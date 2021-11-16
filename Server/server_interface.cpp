#include "server_interface.h"
#include "./ui_server_interface.h"
const QString BLACK_COLOR="color: rgb(0, 0, 0);",
              GREEN_COLOR="color: rgb(0, 255, 127);",
              RED_COLOR="color: rgb(255, 0, 0);";
Server_Interface::Server_Interface(QWidget* the_widget,
const string &entered_address,
const uint16_t entered_port):the_IP_of_the_server(entered_address),
    the_port_of_the_server(entered_port),
    the_socket(the_context),
    the_acceptor(the_context),
    QMainWindow(the_widget),
    ui(new Ui::Server_Interface)
{
    ui->setupUi(this);
    ui->File->setText("C:/Users/Lamaba/Desktop/Server/Log.txt");
    ui->File->hide();
    ui->Text_to_file->hide();
    ui->Information->hide();
    ui->Activating->hide();
    ui->IP->hide();
    ui->Port->hide();
}
void Server_Interface::EnableServer()
{
    if (ui->Log->checkState()==Qt::CheckState::Checked)
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
        the_logger.OpenFile(the_text.toStdString());
        DoLog=true;
    }
    ui->Activation->hide();
    ui->File->hide();
    ui->Log->hide();
    ui->Text_to_file->hide();
    the_endpoint.address(asio::ip::make_address(the_IP_of_the_server,
                                                the_error_code));
    the_endpoint.port(the_port_of_the_server);
    the_acceptor=asio::ip::tcp::acceptor(the_context,the_endpoint);
    ui->Activating->show();
    ui->Activating->setStyleSheet(RED_COLOR);
    ui->Activating->setText("Waiting for a new client's connection...");
    ui->IP->setText("IP: "+QString::fromStdString(
                        the_IP_of_the_server));
    ui->IP->show();
    ui->Port->setText("Port: "+QString::fromStdString(
                        to_string(the_port_of_the_server)));
    ui->Port->show();
    std::thread the_thread([&]()
    {
        the_acceptor.accept(the_socket);
    if (the_error_code)
    {
        ui->Activating->setText("Connection failed!");
        if (DoLog) the_logger.LogToFile("The server enabled, but could not launch. Connection failed");
        return;
    }
    the_IP_of_the_client=
            the_socket.remote_endpoint().address().to_string();
    the_port_of_the_client=to_string(
                the_socket.remote_endpoint().port());
    const string the_successfull_connection=
    "The client with IP "+the_IP_of_the_client+ " and port "+
    the_port_of_the_client+ " has connected to the server";
    ui->Activating->setStyleSheet(GREEN_COLOR);
    ui->Activating->setText(
    QString::fromStdString(the_successfull_connection));
    if (DoLog) the_logger.LogToFile(
    "The server enabled successfully. "+the_successfull_connection);
    bool IsClientStillConnected=true;
    while (IsClientStillConnected) Get(IsClientStillConnected);
    });
    the_thread.detach();
}
void Server_Interface::CheckLog()
{
    if (ui->Log->checkState()==Qt::CheckState::Checked)
    {
        ui->File->show();
        ui->Text_to_file->show();
    }
    else
    {
        ui->File->setText("C:/Users/Lamaba/Desktop/Server/Log.txt");
        ui->File->hide();
        ui->Text_to_file->setStyleSheet(BLACK_COLOR);
        ui->Text_to_file->setText(
                    "Enter the directory file if necessary");
        ui->Text_to_file->hide();
    }
}
void Server_Interface::Get(bool &the_connecting_status)
{
the_socket.wait(the_socket.wait_read);
const size_t the_byte_size=the_socket.available();
string entered_request;
entered_request.resize(the_byte_size);
if (the_byte_size!=0)
{
the_socket.read_some(asio::buffer(entered_request.data(),
                                  the_byte_size),the_error_code);
if (entered_request=="Stop")
{
if (DoLog)
the_logger.LogToFile("The client has been disconnected. The server has been disabled");
DisableServer();
the_connecting_status=false;
return;
}
if (DoLog) the_logger.LogToFile("The client translated word \""+
                         entered_request+'"');
string the_answer;
auto the_pair=the_database.equal_range(entered_request);
if (the_pair.first==the_pair.second) the_answer="Not found!";
else the_answer=*the_pair.first;
the_socket.write_some(asio::buffer(the_answer.data(),
                                   the_answer.size()),the_error_code);
if (DoLog) the_logger.LogToFile(
"The server translated the client's sent word \""+
entered_request+"\" as \""+the_answer+'"');
}
}
void Server_Interface::DisableServer()
{
    ui->IP->hide();
    ui->Port->hide();
    ui->Activating->setStyleSheet(RED_COLOR);
    ui->Activating->setText("The client has been disconnected. The server has been disabled");
    the_acceptor.close(the_error_code);
}
Server_Interface::~Server_Interface()
{
    delete ui;
}
void Server_Interface::Logger
::OpenFile(const string &entered_path)
{
if (the_file.is_open()) the_file.close();
the_file.open(entered_path,ios_base::app);
the_file<<setw(111)<<setfill('*')<<'\n';
LogToFile("The server's work has been started");
}
void Server_Interface::Logger::LogToFile(const string &entered_message)
{
the_file<<entered_message<<". Date: "<<
the_date.currentDate().toString().toStdString()<<". Time: "<<
the_time.currentTime().toString().toStdString()<<'\n';
}
Server_Interface::Logger::~Logger()
{
if (the_file.is_open())
{
    LogToFile("The server's work has been stopped");
    the_file<<setw(111)<<setfill('*')<<'\n';
    the_file.close();
}
}
