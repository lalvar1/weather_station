#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QWidget>
#include <QColor>
#include <QVector>
#include <QByteArray>
#include <QFrame>
#include <QTimer>
#include <qextserialport.h>
#include <unistd.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void CerrarPuerto();
    bool Conectado();
    void EnviarComando(QString comando);

    void EnumerarPuertos();
    void ActualizarEstadoConexion();

    ~MainWindow();

private slots:
    void on_pushButton_T_clicked();
    void on_button_refresh_ports_clicked();
    void on_pushButton_conectar_clicked();

    void on_pushButton_P_clicked();

    void on_pushButton_P_2_clicked();

    //void on_pushButton_systemtime_clicked();
    void onDatosRecibidos();
    //void onReadyRead();
    //void ProcesarDatosRecibidos(QByteArray & bytes);

    void on_pushButton_Leer_dht_clicked();

    void on_pushButton_T_2_clicked();

    void on_pushButton_Leer_bmp_clicked();

private:
    Ui::MainWindow *ui;
    QextSerialPort *m_port;
    QByteArray Buffer_Recepcion;
    //QLabel *m_status_bytes_recibidos;
    //int m_cant_bytes_recibidos;
    //int m_cant_bytes_enviados;
};

#endif // MAINWINDOW_H
