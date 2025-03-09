#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql/mysql.h>


int main(int argc, char *argv[])
{
	MYSQL *conn;
    int err;
    MYSQL_RES *resultado;
    MYSQL_ROW row;
	// Crear conexión con MySQL
    conn = mysql_init(NULL);
	char consulta[256];
    if (conn == NULL) {
        printf("Error al crear la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }

    conn = mysql_real_connect(conn, "localhost", "root", "mysql", "poker", 0, NULL, 0);
    if (conn == NULL) {
        printf("Error al inicializar la conexión: %u %s\n", mysql_errno(conn), mysql_error(conn));
        exit(1);
    }
	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	int respuesta;
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto de escucha
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	// Bucle infinito
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		int terminar =0;
		// Entramos en un bucle para atender todas las peticiones de este cliente
		//hasta que se desconecte
		while (terminar ==0)
		{
			// Ahora recibimos la petici?n
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
			
			
			printf ("Peticion: %s\n",peticion);
			
			// vamos a ver que quieren
			char *p = strtok( peticion, "/");
			int NumPeticion =  atoi (p);
			// Ya tenemos el numero de la peticion
			char IdUsuario[20];
			char Contraseña[20];
			if(NumPeticion != 0)
			{
				p = strtok( NULL, "/");
				strcpy (IdUsuario, p);
				// Ya tenemos el usuario
				p = strtok( NULL, "/");
				strcpy (Contraseña, p);
				//Ya tenemos la contraseña
			}
			switch(NumPeticion)
			{
				case 0://terminar peticion
				terminar = 1; 
				break;
					
				case 1://registrarse
				char DNI[20];
				char nombre[20];
				int edad;
				p = strtok( NULL, "/");
				strcpy (DNI, p);//Ya tenemos la DNI
				p = strtok( NULL, "/");
				strcpy (nombre, p);//Ya tenemos la contraseña
				p = strtok( NULL, "/");
				edad = atoi(p);//Ya tenemos la edad
				
				mysql_free_result(resultado); // Liberar resultado antes de una nueva consulta
				consulta[0] = '\0';//liberar cosulta
				sprintf(consulta, "INSERT INTO jugadores VALUES('%s','%s','%s','s%','%d') " ,IdUsuario ,Contraseña ,DNI ,nombre ,edad);
				err = mysql_query(conn, consulta);
				if (err != 0) 
				{
					printf("Error al registrarse");
					respuesta = 0;//Operacion no realizada.
				    // Enviamos respuesta
					write (sock_conn,respuesta, strlen(respuesta));
				}
				else{
					printf("Te has registrado con exito.");
					respuesta = 1;//Operacion realizada con exito.
				    // Enviamos respuesta
					write (sock_conn,respuesta, strlen(respuesta));
				}
				mysql_close(conn);
				break;
					
				case 2://login
				mysql_free_result(resultado);// Liberar resultado antes de una nueva consulta
				consulta[0] = '\0';//liberar cosulta
				sprintf(consulta, "SELECT contraseña FROM jugadores WHERE jugadores.id_jugador = '%s'", IdUsuario);
				err = mysql_query(conn, consulta);
				if (err != 0) {
					printf("El usuario introducido es incorrecto");
					respuesta = 0;//Operacion no realizada.
				    // Enviamos respuesta
					write (sock_conn,respuesta, strlen(respuesta));
				}
				else
				{
					resultado = mysql_store_result(conn);
					if(resultado == NULL) {
						printf("Error al obtener el resultado de la consulta.\n");
						respuesta = 0;//Operacion no realizada.
						// Enviamos respuesta
						write (sock_conn,respuesta, strlen(respuesta));
					}
					row = mysql_fetch_row(resultado);
					if(Contraseña == row[0]){
						printf("Has iniciado sesion con exito.");
						respuesta = 1;//Operacion realizada con exito.
						// Enviamos respuesta
						write (sock_conn,respuesta, strlen(respuesta));
					}			
				}
				mysql_close(conn);
				break;
					
				default:
				printf("Peticion incorrecta.");
				break;
			}
			terminar = 1;
			// Se acabo el servicio para este cliente
			close(sock_conn);
		}	 	
}