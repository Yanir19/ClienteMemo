#ifndef SOCKETCLIENTE_H
#define SOCKETCLIENTE_H
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <QObject>
#include <arpa/inet.h>

using namespace std;

struct datas{
    void *dato;
    int tipo;
};

struct  estructura{
    int descriptor;
    int info;
};


class SocketCliente: public QObject
{
    Q_OBJECT
public:
    SocketCliente();
    bool connectarTCP(string IP, int Puerto);
    bool connectarUDP(string IP, int Puerto);
    void * getCliente(int id);
    int updateCliente(void *dat,size_t tam,int id);
    int deleteCliente(int id);
    int createCliente(void* dat,size_t t);
    void * combine(void *o1, size_t s1, void *o2, size_t s2);
private:
    int descriptor,port;
    sockaddr_in info;
    static void * controlador(void *obj);
    int tipo_conex;
    string ipp;


signals:
    void NewMensaje(QString msn);
};

#endif // SOCKETCLIENTE_H
