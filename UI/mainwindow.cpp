#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qextserialenumerator.h>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QTimer>
#include <unistd.h>
int flag_h=0,flag_p=0,flag_t=0,flag_t2=0,flag_bmp=0,flag_dht=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_port(NULL), ui(new Ui::MainWindow)
{
     ui->setupUi(this);
    //UpdateTabs();
    EnumerarPuertos();
    ActualizarEstadoConexion();

}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_T_clicked()
{
    m_port->putChar('B');
    flag_t=1;
    while(m_port->bytesAvailable() <= 3)
    {}

}

void MainWindow::on_pushButton_P_clicked()
{

    m_port->putChar('P');
    flag_p=1;
    while(m_port->bytesAvailable() <= 6)
    {}

}

void MainWindow::on_pushButton_P_2_clicked(){

    m_port->putChar('H');
    flag_h=1;
    while(m_port->bytesAvailable() <= 2)
    {}
   }


void MainWindow::on_pushButton_Leer_dht_clicked()
{
    m_port->putChar('F');
    flag_dht=1;
    while(m_port->bytesAvailable() <= 10)
    {}


}


void MainWindow::on_pushButton_Leer_bmp_clicked()
{
    m_port->putChar('I');
    flag_bmp=1;
    while(m_port->bytesAvailable() <= 14)
    {}

}



void MainWindow::on_pushButton_T_2_clicked()
{
     m_port->putChar('D');
     flag_t2=1;
     while(m_port->bytesAvailable() <= 3)
     {}
}


void MainWindow::onDatosRecibidos(){

    int cant=0,i=0;
    QByteArray aux_dht,aux_bmp,aux_linea;

    aux_linea.append("   ");

    Buffer_Recepcion.clear();
    cant = m_port->bytesAvailable();
    Buffer_Recepcion.resize(cant);

    m_port->read(Buffer_Recepcion.data(), Buffer_Recepcion.size());
    if(flag_h)
    {

    ui->lineEdit_hum->setText(Buffer_Recepcion);
    flag_h=0;
    }
    if(flag_t)
    {
    ui->lineEdit_temp->setText(Buffer_Recepcion);
    flag_t=0;
    }
    if(flag_t2)
    {
    ui->lineEdit_temp_2->setText(Buffer_Recepcion);
    flag_t2=0;
    }
    if(flag_p)
    {
    ui->lineEdit_pres->setText(Buffer_Recepcion);
    flag_p=0;
    }
    if(flag_dht)
    {
        while(Buffer_Recepcion[0]!='X')//condicion de fin del micro
        {
        aux_dht.append(Buffer_Recepcion);
        aux_dht.append(aux_linea);
        m_port->putChar('Z');
        //cambio 11 por 10
        while(m_port->bytesAvailable() <= 10)
        {}
        Buffer_Recepcion.clear();
        cant = m_port->bytesAvailable();
        Buffer_Recepcion.resize(cant);
        m_port->read(Buffer_Recepcion.data(), Buffer_Recepcion.size());

        }
        ui->lineEdit_leerdht->setText(aux_dht);
        flag_dht=0;
    }

    if(flag_bmp)
    {
        while(Buffer_Recepcion[0]!='X' && Buffer_Recepcion[1]!='X' && Buffer_Recepcion[2]!='X'&& Buffer_Recepcion[3]!='X'&& Buffer_Recepcion[4]!='X'&& Buffer_Recepcion[5]!='X'&& Buffer_Recepcion[6]!='X'&& Buffer_Recepcion[7]!='X'&& Buffer_Recepcion[8]!='X'&& Buffer_Recepcion[9]!='X'&& Buffer_Recepcion[10]!='X'&& Buffer_Recepcion[11]!='X'&& Buffer_Recepcion[12]!='X'&& Buffer_Recepcion[13]!='X' )//condicion de fin del micro
        {

        aux_bmp.append(Buffer_Recepcion);
        //aux_bmp.append(aux_linea);
        m_port->putChar('Z');

        while(m_port->bytesAvailable() <= 15)
        {
        }
        Buffer_Recepcion.clear();
        cant = m_port->bytesAvailable();
        Buffer_Recepcion.resize(cant);
        m_port->read(Buffer_Recepcion.data(), Buffer_Recepcion.size());

        }
        flag_bmp=0;
        ui->lineEdit_leerbmp->setText(aux_bmp);
    }

      Buffer_Recepcion.clear();
}


void MainWindow::on_pushButton_conectar_clicked()
{
    if (!m_port)	//Si no hay conexión activa
    {
        //Obtenemos el nombre del puerto
        QString sPort;

        int i = ui->comboBox_puertos->currentIndex();
        if (i != -1 && ui->comboBox_puertos->itemText(i) == ui->comboBox_puertos->currentText())
            sPort = ui->comboBox_puertos->itemData(i).toString();
        else
            sPort = ui->comboBox_puertos->currentText();

        if(sPort.isEmpty())
        {
            QMessageBox::critical(this, QString::fromUtf8("Error de conexión"), QString::fromUtf8("Seleccione un puerto válido"));
            return;
        }

        //qDebug()<<sPort;
        //Abrimos el puerto en modo "EventDriven". También podría ser en modo "Polling".
        m_port = new QextSerialPort(sPort, QextSerialPort::EventDriven);

        m_port->setBaudRate(BAUD9600);		//9600 baud
        m_port->setFlowControl(FLOW_OFF);	//Sin control de flujo
        m_port->setParity(PAR_NONE);		//Sin paridad
        m_port->setDataBits(DATA_8);		//8 bits de datos
        m_port->setStopBits(STOP_1);		//1 bit de stop

        if (m_port->open(QIODevice::ReadWrite) == true)	//Abrimos el puerto en modo lectura-escritura
        {
            //Conectamos las señales que nos interesen
            connect(m_port, SIGNAL(readyRead()), this, SLOT(onDatosRecibidos()));
            m_port->readAll();


            //ui->tabWidget->setCurrentIndex(1);
        }
        else
            CerrarPuerto();
    }
    else	//Si no, cerramos la conexión
        CerrarPuerto();

    ActualizarEstadoConexion();
}

void MainWindow::CerrarPuerto()
{
    m_port->close();
    delete m_port;
    m_port = NULL;
}

void MainWindow::ActualizarEstadoConexion()
{
    if (Conectado())
    {
        ui->lineEdit_estado->setStyleSheet("font-weight: bold; color: black; background-color: lightgreen;");
        ui->lineEdit_estado->setText("CONECTADO");
        ui->pushButton_conectar->setText("Desconectar");
    }
    else
    {
        ui->lineEdit_estado->setStyleSheet("font-weight: normal; color: white; background-color: red;");
        ui->lineEdit_estado->setText("Desconectado");
        ui->pushButton_conectar->setText("Conectar");
    }
}

void MainWindow::EnviarComando(QString comando)
{
    //Controlar si en modo socket o rs232
    qDebug() << comando;

    QByteArray send = (comando + "\r\n").toLatin1();
qDebug() << send;
    m_port->write(send);

    //m_cant_bytes_enviados += send.size();
}

void MainWindow::on_button_refresh_ports_clicked()
{
    EnumerarPuertos();
}

void MainWindow::EnumerarPuertos()
{
    ui->comboBox_puertos->clear();

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

    for (int i = 0; i < ports.size(); i++)
    {

    #ifdef Q_OS_WIN
        ui->comboBox_puertos->addItem(ports.at(i).friendName, ports.at(i).portName);
    #elif (defined Q_OS_UNIX)
        ui->comboBox_puertos->addItem(ports.at(i).friendName, ports.at(i).physName);
    #endif

    }
}





//void MainWindow::on_pushButton_systemtime_clicked()
//{
//   // QDateTime::currentDateTime();

//    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());


//}

bool MainWindow::Conectado()
{
    return m_port && m_port->isOpen();
}




