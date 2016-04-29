#include "socketcliente.h"

SocketCliente::SocketCliente()
{
}

bool SocketCliente::connectarTCP(string IP, int Puerto)
{
    descriptor = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(descriptor < 0)
        return false;
    info.sin_family = AF_INET;
    info.sin_addr.s_addr = inet_addr(IP.c_str());
    info.sin_port = ntohs(Puerto);
    memset(&info.sin_zero,0,sizeof(info.sin_zero));

    if((::connect(descriptor,(sockaddr*)&info,(socklen_t)sizeof(info))) < 0)
     return false;

    tipo_conex = 0;
    return true;
}


bool SocketCliente::connectarUDP(string IP, int Puerto)
{
    ipp=IP;
    port=Puerto;

 cout << "conectar" << endl;
   /* Se abre el socket UDP */
    descriptor = socket (AF_INET, SOCK_DGRAM, 0);
    if (descriptor == -1)
    {
        cout << "Hola" << endl;
        return false;
    }

    /* Se rellena la estructura de datos necesaria para hacer el bind() */
    info.sin_family = AF_INET;                    /* Socket inet */
    info.sin_addr.s_addr =  INADDR_ANY;    /* Cualquier dirección IP */
    info.sin_port = htons(0);                    /* Dejamos que linux eliga el servicio */

    /* Se hace el bind() */
    if (bind ( descriptor,(struct sockaddr *)&info, sizeof (info)) == -1)
    {
         cout << "mal bind" << endl;
        close (descriptor);
        return false; //Si no se puede hacer la conexion retorna un false
    }
    tipo_conex = 1;
     cout << "retrun" << endl;
    return true; // Si se pudo establacer la conexion se retorna true
}


void * SocketCliente::getCliente(int id)
{

    char * buffer;
    buffer="2";
    void *idd;
    idd=(void *)&id;
    char respuesta[1024] = {0};
    int bytes;
    void * dato = combine((void *)buffer,strlen("1"),idd,sizeof(int));

    if(tipo_conex == 0){


        send(descriptor,dato,strlen("1")+sizeof(int),0);

        bytes = recv(descriptor,respuesta,1024,0);
    }
    if(tipo_conex == 1){

        struct sockaddr_in Dire;
         Dire.sin_family = AF_INET;

         Dire.sin_addr.s_addr = inet_addr(ipp.c_str());
         Dire.sin_port = ntohs(port);

        sendto (descriptor, dato, strlen("1")+sizeof(int), 0, (struct sockaddr *)&Dire, sizeof (Dire));

        bytes = recvfrom (descriptor, respuesta, 1024, 0,0,0);

     }



    void *recv;
    recv=malloc(bytes);// VER QUE RETORNAR SI NO LO ENCUENTRA
    memcpy(recv,respuesta,bytes);
  return recv;

}

int SocketCliente::updateCliente(void *dat,size_t tam,int id)
{

    char * buffer;
    buffer="4";
    void *idd;
    idd=(void *)&id;
    int bytes;
    char respuesta[1024] = {0};

    void * dato1 = combine((void *)buffer,strlen("1"),idd,sizeof(int)); //Aqui los combino

    void *dato =combine(dato1,sizeof(int)+strlen("1"),dat,tam);

    if(tipo_conex ==0){

         send(descriptor,dato,strlen("1")+tam+sizeof(int),0);//Mando el dato

         bytes =recv(descriptor,respuesta,1024,0); //Recibo la respuesta
    }

    if(tipo_conex == 1){
        struct sockaddr_in Dire;
         Dire.sin_family = AF_INET;

         Dire.sin_addr.s_addr = inet_addr(ipp.c_str());
         Dire.sin_port = ntohs(port);
        sendto (descriptor, dato, strlen("1")+tam+sizeof(int), 0, (struct sockaddr *)&Dire, sizeof (Dire));

        bytes = recvfrom (descriptor, respuesta, 1024, 0, 0, 0);
    }


    int * mensaje;//A Si se recibe cuando sabes que te estan retornando un entero
    mensaje=(int *) malloc(sizeof(int));
    memcpy(mensaje,respuesta,bytes);
    cout << *mensaje<< endl;
    if(*mensaje!=0){ //Si el entero que me retornan no es 0 entocnes retorno 1, si no 0.
        return 1;
    }else{
        return 0;
    }

}

int SocketCliente::createCliente(void *dat,size_t tam)
{
//Aqui lo que hago es crear un char * y junto con el puntero que me pasan los uno
    //los uno a travez de la funcion combine. esto lo qeu hace es agarrar unos bytes y pegarselos
    //Al lado a otros bytes y te da un puntero al primer elemento de la funcion
    //algo como esto:  dato=[bytes de variable buffer][bytes de lo que quieren mandar]
    char * buffer;
    buffer="1";
    int bytes;
    char respuesta[1024] = {0};

    void * dato = combine((void *)buffer,strlen("1"),dat,tam); //Aqui los combino

    if(tipo_conex ==0){
        send(descriptor,dato,strlen("1")+tam,0);//Mando el dato

        bytes=recv(descriptor,respuesta,1024,0); //Recibo la respuesta
    }

    if(tipo_conex == 1){



        struct sockaddr_in Dire;
         Dire.sin_family = AF_INET;

         Dire.sin_addr.s_addr = inet_addr(ipp.c_str());
         Dire.sin_port = ntohs(port);

         sendto (descriptor, dato, strlen("1")+tam, 0, (struct sockaddr *)&Dire, sizeof (Dire));

         bytes = recvfrom (descriptor, respuesta, 1024, 0, 0, 0);
    }

    int * mensaje;//ASi se recibe cuando sabes que te estan retornando un entero
    mensaje=(int *) malloc(sizeof(int));
    memcpy(mensaje,respuesta,bytes);
    cout << *mensaje<< endl;
    if(*mensaje!=0){ //Si el entero que me retornan no es 0 entocnes retorno 1, si no 0.
        return *mensaje;
    }else{
        return 0;
    }


}

int SocketCliente::deleteCliente(int id)
{
    char * buffer;
    buffer="3";
    void *idd;
    idd=(void *)&id;
    char respuesta[1024] = {0};
    int bytes;
    void * dato = combine((void *)buffer,strlen("1"),idd,sizeof(int));

    if(tipo_conex == 0){
        send(descriptor,dato,strlen("1")+sizeof(int),0);

        bytes = recv(descriptor,respuesta,1024,0);
    }

     if(tipo_conex == 1){
         struct sockaddr_in Dire;
          Dire.sin_family = AF_INET;

          Dire.sin_addr.s_addr = inet_addr(ipp.c_str());
          Dire.sin_port = ntohs(port);

          sendto (descriptor, dato, strlen("1")+sizeof(int), 0, (struct sockaddr *)&Dire, sizeof (Dire));

          bytes = recvfrom (descriptor, respuesta, 1024, 0, 0, 0);
     }


     int * mensaje;
     mensaje=(int *) malloc(sizeof(int));
     memcpy(mensaje,respuesta,bytes);
     cout << *mensaje<< endl;
     if(*mensaje==1){
         return 1;
     }else{
         return 0;
     }
}


void * SocketCliente::combine(void *o1, size_t s1, void *o2, size_t s2)
      {


          void *result = malloc(s1 + s2);
          if (result != NULL)
              mempcpy(mempcpy(result, o1, s1), o2, s2);
          return result;
      }
