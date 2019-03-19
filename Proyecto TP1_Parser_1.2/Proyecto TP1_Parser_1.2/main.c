/*

TP1 EDA 2019 GRUPO 1 - ParseCmdLine
INTEGRANTES:
-
  DIMAS MIGUEL BOSCH
  PEDRO DALLA LASTA
  IGNACIO DOMINGUEZ
 
 -

*/



//--------------------------------------------LIBRERIAS-----------------------------------------//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>    
#include <ctype.h>		
#include "pcmld.h"



#if defined(_WIN32) || defined(_WIN64) //si el sistema operativo es WINDOWS

#include <Windows.h>  // API del Sistema Operativo de Windows (Permite trabajar sobre la Consola).
					  //sacado de 
					  //https://foro.elhacker.net/programacion_cc/cambio_colores_en_un_solo_printf_en_c-t484932.0.html

#endif // _WIN32 or _WIN64



//-------------------------------------DEFINES and CONSTANTES---------------------------------------//


#define EXIT_LOOP 0

#define MAX_NUM_VALUE	5200		//valor maximo que permite como value, sin poner dentro de la lista de value valido
#define	MIN_NUM_VALUE	-200		//valor minimo que permite como value, sin poner dentro de la lista de value valido

#define STR_EQUAL	0

#define	TRUE	1
#define FALSE	0



enum type_of_operand { KEY, VALUE, PARAMETER }; //enum que me define los tipos de operando




#if defined(_WIN32) || defined(_WIN64)
//enum sacado de https://foro.elhacker.net/programacion_cc/cambio_colores_en_un_solo_printf_en_c-t484932.0.html

enum colors { // Listado de colores (La letra "L" al inicio, indica que es un color más claro que su antecesor).
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LGREY = 7,
	DGREY = 8,
	LBLUE = 9,
	LGREEN = 10,
	LCYAN = 11,
	LRED = 12,
	LMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};
//si el sistema operativo es WINDOWS //si el sistema operativo es WINDOWS
#endif // _WIN32 or _WIN64





//--------------------------------------------ESTRUCTURAS-----------------------------------------//

typedef struct {

	int cant_valid_keys;	//me dice cuantos keys validos hay en la lista de keys validos
	int cant_valid_values;	//me dice cuantos values validos hay en la lista de values validos
	int cant_valid_params;	//me dice cuantos parameters validos hay en la lista de parameters validos
	
	char *(*p2_valid_key)[];	//lista de keys validos
	char *(*p2_valid_param)[];	//lista de vaLues validos
	char *(*p2_valid_value)[];	//lista de parameters validos

} userdata_t;






//------------------------------------------FUNCIONES-------------------------------------------------//

int parseCallback(char *key, char *value, void *userData);

/*
Esta función es llamada cada vez que parseCmdLine() encuentra una opción o un parámetro.

La función de callback deberá devolver 1
si la interpretación de las opciones o parámetros fuera correcta, y 0 para indicar
que el procesamiento debe detenerse porque se encontró una opción o
parámetro inválido. */


/*Funciones que utiliza el callback*/
int is_valid_str(char *str, void *valid_strs, int type_of_operand);
// validate_letterstr devuelve 1 si el string pertenece a la lista de opciones/parametros validos sino 0

bool str_is_number(const char* str); //funcion para fijarse si el string es un numero float (positivo o negativo)


//si el sistema operativo es WINDOWS
#if defined(_WIN32) || defined(_WIN64)

void Color(int Background, int Text); // Prototipo de función  
									  //sacado de 
									  //https://foro.elhacker.net/programacion_cc/cambio_colores_en_un_solo_printf_en_c-t484932.0.html


#endif // _WIN32 or _WIN64




//--------------------------------------------MAIN--------------------------------------------------//


int main(int argc, char*argv[]) {

	
	//si el sistema operativo es WINDOWS

	#if defined(_WIN32) || defined(_WIN64)
	Color(BLACK, YELLOW);		//Texto en color amarillo y fondo Negro
	#endif // _WIN32 or _WIN64

	
	printf("El programa ademas de realizar lo pedido por consigna, acepta como value valido a un numero float que\n ");
	printf("se encuentre dentro del rango %f y %f , definidas por los defines MIN_NUM_VALUE y MAX_NUM_VALUE \n\n   " , (double) MIN_NUM_VALUE , (double) MAX_NUM_VALUE);


	//si el sistema operativo es WINDOWS

	#if defined(_WIN32) || defined(_WIN64)
	Color(BLACK, WHITE);		// Devolvemos el color original de la consola.
	#endif // _WIN32 or _WIN64

	





	char *valid_key[] = { "-key","key","font","queue","color" }; // lista de opciones válidas
	char *valid_values[] = { "-value" ,"value","times","grow" };
	char *valid_param[] = { "hello","bye" };	// lista de parametros válidos

	userdata_t user_info;

	user_info.cant_valid_keys = sizeof(valid_key) / sizeof(valid_key[0]);
	user_info.cant_valid_params = sizeof(valid_param) / sizeof(valid_param[0]);
	user_info.cant_valid_values = sizeof(valid_values) / sizeof(valid_values[0]);


	user_info.p2_valid_key = &valid_key;
	user_info.p2_valid_value = &valid_values;
	user_info.p2_valid_param = &valid_param;

	pCallback p2_function = parseCallback;


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 1 ---> Error de Forma 1: Opción sin valor {'path','-key'} \n ");

	char *test1_argv[] = { "path", "-key" };
	int test1_argc = sizeof(test1_argv) / sizeof(test1_argv[0]) ;

	if (parseCmdline(test1_argc, test1_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64
				
		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64

	}
		
	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64

		
	}
		

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 2 ---> Error de Forma 2: Opcion sin clave {'path','-'} \n ");

	char * test2_argv[] = { "path", "-" };
	int test2_argc = sizeof(test2_argv) / sizeof(test2_argv[0]) ;

	if ( parseCmdline(test2_argc, test2_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64

		
		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64

		
	}
		
	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64

		
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 3 ---> Opcion valida 1: Opcion con clave y valor {'path','-key','value'} \n ");

	char * test3_argv[] = { "path", "-key","value" };
	int test3_argc = sizeof(test3_argv) / sizeof(test3_argv[0]) ;

	if ( parseCmdline(test3_argc, test3_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 4 ---> Opcion valida 2: Opcion con clave (con ademas el prefijo de la clave) y valor {'path','--key','value'} \n ");

	char * test4_argv[] = { "path", "--key","value" };
	int test4_argc = sizeof(test4_argv) / sizeof(test4_argv[0]) ;

	if ( parseCmdline(test4_argc, test4_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 5 ---> Opcion valida 3: Opcion con clave y valor (con ademas el prefijo de la clave) {'path','--key','-value'} \n ");

	char * test5_argv[] = { "path", "-key","-value" };
	int test5_argc = sizeof(test5_argv) / sizeof(test5_argv[0]) ;

	if ( parseCmdline(test5_argc, test5_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	printf("Prueba 6 ---> Opcion valida 4: Opcion con clave y valor numero en rango valido {'path','--key', number_str_in_the_rank } \n ");

	char * test6_argv[] = { "path", "-key","-190.0005" };
	int test6_argc = sizeof(test6_argv) / sizeof(test6_argv[0]) ;

	if (parseCmdline(test6_argc, test6_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	printf("Prueba 7 ---> Opcion invalida 1: Opcion con clave y valor numero en rango invalido {'path','--key', number_str_out_the_rank } \n ");

	char * test7_argv[] = { "path", "-key","100050" };
	int test7_argc = sizeof(test7_argv) / sizeof(test7_argv[0]) ;

	if (parseCmdline(test7_argc, test7_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 8 ---> Opcion valida 5: Opcion parametro valido {'path','valid_parameter' } \n ");

	char * test8_argv[] = { "path", "hello" };
	int test8_argc = sizeof(test8_argv) / sizeof(test8_argv[0]) ;

	if (parseCmdline(test8_argc, test8_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 9 ---> Opcion invalida 2: Opcion parametro invalido {'path','invalid_parameter' } \n ");

	char * test9_argv[] = { "path", "hola" };
	int test9_argc = sizeof(test9_argv) / sizeof(test9_argv[0]) ;

	if (parseCmdline(test9_argc, test9_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	printf("Prueba 10 ---> Opcion invalida 3: Opcion key invalido y value valido {'path','invalid_key', 'valid_value' } \n ");

	char * test10_argv[] = { "path", "-hola", "value" };
	int test10_argc = sizeof(test10_argv) / sizeof(test10_argv[0]) ;

	if (parseCmdline(test10_argc, test10_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	printf("Prueba 11 ---> Opcion valida 4: Opcion key valido y value invalido {'path','valid_key', 'invalid_value' } \n ");

	char * test11_argv[] = { "path", "-key", "hola" };
	int test11_argc = sizeof(test11_argv) / sizeof(test11_argv[0]) ;

	if (parseCmdline(test11_argc, test11_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	printf("Prueba 12 ---> Opcion invalida 5: Opcion key invalido y value invalido {'path','invalid_key', 'invalid_value' } \n ");

	char * test12_argv[] = { "path", "-hola", "hola" };
	int test12_argc = sizeof(test12_argv) / sizeof(test12_argv[0]) ;

	if (parseCmdline(test12_argc, test12_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	printf("Prueba 13 ---> Opcion valida 6: Opcion key valido con value valido y parameter valido {'path','valid_key', 'valid_value', 'valid_parameter'} \n ");

	char * test13_argv[] = { "path", "-color", "times", "bye" };
	int test13_argc = sizeof(test13_argv) / sizeof(test13_argv[0]) ;

	if (parseCmdline(test13_argc, test13_argv, p2_function, &user_info) != PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	printf("Prueba 14 ---> Error de forma tipo I y II: Solo la ruta de acceso {'path'} \n ");

	char * test14_argv[] = { "path" };
	int test14_argc = sizeof(test14_argv) / sizeof(test14_argv[0]);

	if (parseCmdline(test14_argc, test14_argv, p2_function, &user_info) == PARSER_ERROR)
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, LGREEN); //Texto en color Verde Claro y fondo Negro
		#endif // _WIN32 or _WIN64


		printf("Prueba Exitosa \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
		#endif // _WIN32 or _WIN64


	}

	else
	{
		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, RED);	//Texto en color rojo y fondo Negro
		#endif // _WIN32 or _WIN64

		printf("Prueba Fallada \n\n");

		//si el sistema operativo es WINDOWS

		#if defined(_WIN32) || defined(_WIN64)
		Color(BLACK, WHITE);	// Devolvemos el color original de la consola.	
		#endif // _WIN32 or _WIN64


	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//si el sistema operativo es WINDOWS

	#if defined(_WIN32) || defined(_WIN64)
	Color(MAGENTA, BROWN);		//Texto en color azul y fondo blanco
	#endif // _WIN32 or _WIN64

	
	printf("\n\n\n\n PRESIONAR ENTER PARA FINALIZAR PROGRAMA <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <-- <--\n\n");
	
	//si el sistema operativo es WINDOWS

	#if defined(_WIN32) || defined(_WIN64)
	Color(BLACK, WHITE);	// Devolvemos el color original de la consola.
	#endif // _WIN32 or _WIN64

	
	getchar();				



	return 0;

}


int parseCallback(char *key, char *value, void *userData) {

	int no_error = TRUE;
	userdata_t * p2userinfo = (userdata_t*)userData;
	
	
	if (key) { // errores correspondientes a opciones	
		if (!value) { // si es opcion y es el ultimo argumento -> error

			no_error = FALSE;
		}
		if (no_error) {

			no_error = is_valid_str(key , userData, KEY);
		
		}

		if (no_error) {

			no_error = is_valid_str(value, userData, VALUE);

		}

	}
	else { // errores correspondientes a parametros

		no_error = is_valid_str(value, userData, PARAMETER);

	}
	return no_error;
}


int is_valid_str(char *str, void *valid_strs, int type_of_operand) {

	int valid_str;
	userdata_t *p2_user_data = (userdata_t*)valid_strs;
	int i;

	if (type_of_operand == KEY) { // si no es una opcion se da por sentado que es un parametro
		i = p2_user_data->cant_valid_keys;
	}
	else if (type_of_operand == VALUE) {
		i = p2_user_data->cant_valid_values;
	}
	else if (type_of_operand == PARAMETER) {
		i = p2_user_data->cant_valid_params;
	}

	valid_str = FALSE;


	if (type_of_operand == KEY) {

		while (i--) {
			if (strcmp(str, *(*(p2_user_data->p2_valid_key) + i)) == STR_EQUAL) {
				i = EXIT_LOOP;
				valid_str = TRUE;
			}
		}
	}
	else if (type_of_operand == VALUE) {

		while (i--) {
			if (strcmp(str, *(*(p2_user_data->p2_valid_value) + i)) == STR_EQUAL) {
				i = EXIT_LOOP;
				valid_str = TRUE;
			}
			else if (str_is_number(str) == true) //value can be a number
			{
				double number = atof(str); //i know that is a number, but i want to if it's in the rank

				if (((number < MAX_NUM_VALUE) && (number > MIN_NUM_VALUE))) //but value can be a number between a rank
				{
					i = EXIT_LOOP;
					valid_str = TRUE;

				}
				
				
			}
			
		}

	}
	else if (type_of_operand == PARAMETER) {

		while (i--) {

			if (strcmp(str, *(*(p2_user_data->p2_valid_param) + i)) == STR_EQUAL) {
				i = EXIT_LOOP;
				valid_str = TRUE;
			}
		}

	}

	return valid_str;

}


bool str_is_number(const char* str)
{
	bool posibbility_point = true;

	if (*str) // posibilidad de numero negativo 
	{
		if (!((isdigit(*str)) || (*str == '-')))
		{
			return false;
		}
		else
		{
			str++;
		}

	}

	for (; *str; ++str)
	{
		if (!isdigit(*str))
		{
			if (*str == '.')
			{
				if (posibbility_point == false)
				{
					return false;
				}
				else
				{
					posibbility_point = false;
				}
			}
			else
			{
				return false;
			}

		}

	}

	return true;
}





//si el sistema operativo es WINDOWS

#if defined(_WIN32) || defined(_WIN64)


// Función para cambiar el color del fondo y/o pantalla 
//	sacada de https://foro.elhacker.net/programacion_cc/cambio_colores_en_un_solo_printf_en_c-t484932.0.html

void Color(int Background, int Text) {

	HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE); // Tomamos la consola.

	// Para cambiar el color, se utilizan números desde el 0 hasta el 255.
	// Pero, para convertir los colores a un valor adecuado, se realiza el siguiente cálculo.
	int    New_Color = Text + (Background * 16);

	SetConsoleTextAttribute(Console, New_Color); // Guardamos los cambios en la Consola.

}


#endif // _WIN32 or _WIN64
