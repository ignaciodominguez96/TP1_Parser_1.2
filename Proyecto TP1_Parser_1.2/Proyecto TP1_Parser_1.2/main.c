#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>     /* atof */
#include <ctype.h>		/* isdigit */
#include "pcmld.h"



#define MAX_TEST_BAD_ARGS 10
#define NUM_BAD_ARG_TESTS 9

#define EXIT_LOOP 0

#error "poner rango de numeros value ---- usar defines MAX y MIN"

#define MAX_NUM_VALUE	5200
#define	MIN_NUM_VALUE	-200


// compilamos con usual en 1 si se quiere probar el funcionamiento en general sino 0 para desactivarlo
#define USUAL 0
//idem anterior pero con los casos de prueba
#define TEST_BENCH 1

enum type_of_operand { KEY, VALUE, PARAMETER };

typedef struct {

	int cant_valid_options;
	int cant_valid_params;
	int cant_valid_values;

	char *(*p2valid_cmd)[];
	char *(*p2valid_param)[];
	char *(*p2valid_value)[];

} userdata_t;

int parseCallback(char *key, char *value, void *userData);
/*Funciones que utiliza el callback*/
bool is_valid_str(char *str, void *valid_strs, int type_of_operand);
// validate_letterstr devuelve 1 si el string pertenece a la lista de opciones/parametros validos sino 0

bool str_is_number(const char* str); //funcion para fijarse si el string es un numero float (positivo o negativo)

int main(int argc, char*argv[]) {

	char *valid_cmd[] = { "size","font","queue","color" }; // lista de opciones válidas
	char *valid_values[] = { "arial","times","grow" };
	char *valid_param[] = { "hello","bye" };	// lista de parametros válidos

	userdata_t user_info;

	user_info.cant_valid_options = sizeof(valid_cmd) / sizeof(valid_cmd[0]);
	user_info.cant_valid_params = sizeof(valid_param) / sizeof(valid_param[0]);
	user_info.cant_valid_values = sizeof(valid_values) / sizeof(valid_values[0]);


	user_info.p2valid_cmd = &valid_cmd;
	user_info.p2valid_value = &valid_values;
	user_info.p2valid_param = &valid_param;

	pCallback p = parseCallback;


	//  Este printf para cuando parsecmdline es usado con la consola
#if USUAL
	printf("%d\n", parseCmdLine(argc, argv, p, &user_info));
#endif


#if TEST_BENCH
	/*BANCO DE PRUEBAS:
	Aclaración:
	tenemos listas de opciones, parametros y values válidos que por lo pronto
	solo se verifica que esten en la lista, no que las opciones y los values sean
	compatibles entre sí.
	Se añade como ultimo elemento un puntero a null simulando el puntero a null
	que nos da argv[argc]
	*/



	// Casos buenos : 


	char *good_test1[] = { "miprog","-size","grow",NULL }; // option valida y value valido
	char *good_test2[] = { "miprog","-size", "grow", "-font","arial", NULL };
	char *good_test3[] = { "miprog","hello",NULL }; // parametro valido

	printf("%d\n", parseCmdline((sizeof(good_test1) / sizeof(good_test1[0])) - 1, good_test1, p, &user_info));
	printf("%d\n", parseCmdline((sizeof(good_test2) / sizeof(good_test1[0])) - 1, good_test2, p, &user_info));
	printf("%d\n", parseCmdline((sizeof(good_test3) / sizeof(good_test1[0])) - 1, good_test3, p, &user_info));

	// Casos malos:
	char *matriz_prueba[NUM_BAD_ARG_TESTS][MAX_TEST_BAD_ARGS] = { {"miprog", "-", NULL}, // - sin option ni value
												{"miprog","-asd", NULL}, // option no valida sin value
												{"miprog","-size",NULL},// option valida pero sin value
												{"miprog","-size","asd",NULL}, // option valida pero con value invalido

												{"miprog","-size","grow", "-font","asd",NULL}, // primer option valida 2da option valida y value no valido
												{"miprog","-size","grow", "-asd","arial",NULL}, // option valida y segunda option no valida


												{"miprog","asd",NULL}, //parametro no valido
												{"miprog","hello","asd",NULL}, //parametro 1 valido , parametro 2 no valido
												{"miprog","hello","-size",NULL} }; // parametro valido, opcion valida pero sin value (al final)


	int i;
	for (i = 0; i < NUM_BAD_ARG_TESTS; i++) {
		printf("prueba %d:  resultado:%d \n", i + 1, parseCmdline(MAX_TEST_BAD_ARGS, matriz_prueba[i], p, &user_info));
	}

#endif
	return 0;
}


int parseCallback(char *key, char *value, void *userData) {

	bool noerror = true;
	userdata_t * p2userinfo = (userdata_t*)userData;
	if (key) { // errores correspondientes a opciones	
		if (!value) { // si es opcion y es el ultimo argumento -> error

			noerror = false;
		}
		if (noerror) {

			noerror = validate_letterstr(key + 1, userData, KEY);
			// evadimos el OPTION_IDENTIFIER
		}

		if (noerror) {

			noerror = validate_letterstr(value, userData, VALUE);

		}

	}
	else { // errores correspondientes a parametros

		noerror = validate_letterstr(value, userData, PARAMETER);

	}
	return noerror;
}


bool is_valid_str(char *str, void *valid_strs, int type_of_operand) {

	bool noerrorsofar;
	userdata_t *p2udata = (userdata_t*)valid_strs;
	int i;

	if (type_of_operand == KEY) { // si no es una opcion se da por sentado que es un parametro
		i = p2udata->cant_valid_options;
	}
	else if (type_of_operand == VALUE) {
		i = p2udata->cant_valid_values;
	}
	else if (type_of_operand == PARAMETER) {
		i = p2udata->cant_valid_params;
	}

	noerrorsofar = false;


	if (type_of_operand == KEY) {

		while (i--) {
			if (!strcmp(str, *(*(p2udata->p2valid_cmd) + i))) {
				i = EXIT_LOOP;
				noerrorsofar = true;
			}
		}
	}
	else if (type_of_operand == VALUE) {

		while (i--) {
			if (!strcmp(str, *(*(p2udata->p2valid_value) + i))) {
				i = EXIT_LOOP;
				noerrorsofar = true;
			}
			else if (str_is_number(*(*(p2udata->p2valid_value) + i)) == false) //value can be a number
			{
				i = EXIT_LOOP;
				noerrorsofar = true;
			}
			else 
			{
				double number = atof(*(*(p2udata->p2valid_value) + i)); //i know that is a number, but i want to if it's in the rank
				
				if (!((number<MAX_NUM_VALUE) && (number > MIN_NUM_VALUE))) //but value can be a number between a rank
				{
					i = EXIT_LOOP;
					noerrorsofar = true;

				}
			}
		}

	}
	else if (type_of_operand == PARAMETER) {

		while (i--) {

			if (!strcmp(str, *(*(p2udata->p2valid_param) + i))) {
				i = EXIT_LOOP;
				noerrorsofar = true;
			}
		}

	}

	return noerrorsofar;

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
