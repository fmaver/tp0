#include "client.h"

t_log* logger;

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"

	log_info(logger, "Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	
	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config

	log_info(logger, valor);
	log_info(logger, ip);
	log_info(logger, puerto);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log *iniciar_logger(void)
{
    t_log *nuevo_logger;

    if ((nuevo_logger = log_create("../log/client.log", "TP0", 1, LOG_LEVEL_INFO)) == NULL)
    {
        printf("No se pudo crear el logger");
        exit(1);
    }
    return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;
	
	nuevo_config = config_create("../config/cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "\0") != 0)
	{
		log_info(logger, leido);
		leido = readline("> ");
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char *leido = readline("> ");
	t_paquete *paquete = crear_super_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	while (strcmp(leido, "\0") != 0)
	{
		agregar_a_paquete(paquete, leido, sizeof(leido));
		free(leido);
		leido = readline("> ");
	}
	enviar_paquete(paquete, conexion);


	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	eliminar_paquete(paquete);
	free(leido);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	config_destroy(config);

}
