#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
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
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9070);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	if (listen(sock_listen, 2) < 0)
		printf("Error en el Listen");
	
	int i;
	// Atenderemos solo 5 peticione
	for(;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexi?n\n");
		//sock_conn es el socket que usaremos para este cliente

		int terminar=0;
		while (terminar==0)
		{
			// Ahora recibimos su nombre, que dejamos en buff
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
		
			// Tenemos que a?adirle la marca de fin de string 
			// para que no escriba lo que hay despues en el buffer
			peticion[ret]='\0';
		
			//Escribimos el nombre en la consola
		
			printf ("Se ha conectado: %s\n",peticion);
		
		
			char *p = strtok( peticion, "/");
			int codigo =  atoi (p);
			float grados;

			if(codigo!=0)
			{
				p = strtok( NULL, "/");
				grados=atof(p);
				printf ("Codigo: %d, Grados: %f\n", codigo, grados);
			}
			
			
			
			if (codigo ==0) 
				terminar=1;
		
			else if (codigo ==1) //Si hay un 1/ nos da los grados en Celsius para pasarlos a Fahrenheit
				float c=(grados*1.8)+32;
				sprintf (respuesta,"%f",c);

			else if (codigo ==2)
				float c=(grados-32)/1.8;
				sprintf (respuesta,"%f",c);
	
			if (codigo!=0)
			{
				printf ("%s\n", respuesta);
				// Y lo enviamos
				write (sock_conn,respuesta, strlen(respuesta));
			}
		}	
			// Se acabo el servicio para este cliente
			close(sock_conn); 
	}
}
