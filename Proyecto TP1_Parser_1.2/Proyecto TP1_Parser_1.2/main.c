#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>     /* atof */
#include <ctype.h>		/* isdigit */
#include "pcmld.h"




#define EXIT_LOOP 0

#define MAX_NUM_VALUE	5200
#define	MIN_NUM_VALUE	-200


enum type_of_operand { KEY, VALUE, PARAMETER };

typedef struct {

	int cant_valid_keys;
	int cant_valid_values;
	int cant_valid_params;
	
	char *(*p2_valid_key)[];
	char *(*p2_valid_param)[];
	char *(*p2_valid_value)[];

} userdata_t;

int parseCallback(char *key, char *value, void *userData);
/*Funciones que utiliza el callback*/
bool is_valid_str(char *str, void *valid_strs, int type_of_operand);
// validate_letterstr devuelve 1 si el string pertenece a la lista de opciones/parametros validos sino 0

bool str_is_number(const char* str); //funcion para fijarse si el string es un numero float (positivo o negativo)

int main(int argc, char*argv[]) {

	char *valid_key[] = { "key","font","queue","color" }; // lista de opciones válidas
	char *valid_values[] = { "-value","value","times","grow" };
	char *valid_param[] = { "hello","bye" };	// lista de parametros válidos

	userdata_t user_info;

	user_info.cant_valid_keys = sizeof(valid_key) / sizeof(valid_key[0]);
	user_info.cant_valid_params = sizeof(valid_param) / sizeof(valid_param[0]);
	user_info.cant_valid_values = sizeof(valid_values) / sizeof(valid_values[0]);


	user_info.p2_valid_key = &valid_key;
	user_info.p2_valid_value = &valid_values;
	user_info.p2_valid_param = &valid_param;

	pCallback p2_function = parseCallback;


	/////////////////////////////

	printf("Prueba 1 ---> Error de Forma 1: Opción sin valor {'path','-key'} \n ");

	char *test1_argv[] = { "path", "-" };
	int test1_argc = sizeof(test1_argv) / sizeof(test1_argv[0]) - 1;

	if ( parseCmdline(test1_argc, test1_argv, p2_function, &user_info) == PARSER_ERROR)
		printf("Prueba Exitosa \n\n");
	else
		printf("Prueba Fallada \n\n");

	/////////////////////////////

	printf("Prueba 2 ---> Error de Forma 2: Opción sin clave {'path','-'} \n ");

	char * test2_argv[] = { "path", "-" };
	int test2_argc = sizeof(test2_argv) / sizeof(test2_argv[0]) - 1;

	if ( parseCmdline(test2_argc, test2_argv, p2_function, &user_info) == PARSER_ERROR)
		printf("Prueba Exitosa \n\n");
	else
		printf("Prueba Fallada \n\n");


	/////////////////////////////

	printf("Prueba 3 ---> Opción válida: Opción con clave y valor {'path','-key','value'} \n ");

	char * test3_argv[] = { "path", "-key","value" };
	int test3_argc = sizeof(test3_argv) / sizeof(test3_argv[0]) - 1;

	if ( parseCmdline(test3_argc, test3_argv, p2_function, &user_info) != PARSER_ERROR)
		printf("Prueba Exitosa \n\n");
	else
		printf("Prueba Fallada \n\n");

	/////////////////////////////

	printf("Prueba 4 ---> Opción válida 2: Opción con clave (con ademas el prefijo de la clave) y valor {'path','--key','value'} \n ");

	char * test4_argv[] = { "path", "--key","value" };
	int test4_argc = sizeof(test4_argv) / sizeof(test4_argv[0]) - 1;

	if ( parseCmdline(test4_argc, test4_argv, p2_function, &user_info) != PARSER_ERROR)
		printf("Prueba Exitosa \n\n");
	else
		printf("Prueba Fallada \n\n");

	/////////////////////////////

	printf("Prueba 5 ---> Opción válida 3: Opción con clave y valor (con ademas el prefijo de la clave) {'path','--key','-value'} \n ");

	char * test5_argv[] = { "path", "--key","-value" };
	int test5_argc = sizeof(test5_argv) / sizeof(test5_argv[0]) - 1;

	if ( parseCmdline(test5_argc, test5_argv, p2_function, &user_info) != PARSER_ERROR)
		printf("Prueba Exitosa \n\n");
	else
		printf("Prueba Fallada \n\n");

	/////////////////////////////

	getchar();

	return 0;

}


int parseCallback(char *key, char *value, void *userData) {

	bool no_error = true;
	userdata_t * p2userinfo = (userdata_t*)userData;
	if (key) { // errores correspondientes a opciones	
		if (!value) { // si es opcion y es el ultimo argumento -> error

			no_error = false;
		}
		if (no_error) {

			no_error = is_valid_str(key + 1, userData, KEY);
			// evadimos el OPTION_IDENTIFIER
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


bool is_valid_str(char *str, void *valid_strs, int type_of_operand) {

	bool valid_str;
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

	valid_str = false;


	if (type_of_operand == KEY) {

		while (i--) {
			if (!strcmp(str, *(*(p2_user_data->p2_valid_key) + i))) {
				i = EXIT_LOOP;
				valid_str = true;
			}
		}
	}
	else if (type_of_operand == VALUE) {

		while (i--) {
			if (!strcmp(str, *(*(p2_user_data->p2_valid_value) + i))) {
				i = EXIT_LOOP;
				valid_str = true;
			}
			else if (str_is_number(*(*(p2_user_data->p2_valid_value) + i)) == false) //value can be a number
			{
				i = EXIT_LOOP;
				valid_str = true;
			}
			else 
			{
				double number = atof(*(*(p2_user_data->p2_valid_value) + i)); //i know that is a number, but i want to if it's in the rank
				
				if (!((number<MAX_NUM_VALUE) && (number > MIN_NUM_VALUE))) //but value can be a number between a rank
				{
					i = EXIT_LOOP;
					valid_str = true;

				}
			}
		}

	}
	else if (type_of_operand == PARAMETER) {

		while (i--) {

			if (!strcmp(str, *(*(p2_user_data->p2_valid_param) + i))) {
				i = EXIT_LOOP;
				valid_str = true;
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
