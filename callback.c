#include <stdio.h>
#include <string.h>
#include "pcmdl.h"

#define FALSE 0
#define TRUE 1

/********************************************************************
*						TABLA DE ERRORES							*
*********************************************************************
* #0 Sin errores													*
* #1 Clave correcta,valor inválido									*
* #2 Clave inválida, valor correcto									*
* #3 Clave y valor inválidos										*
* #4 Parámetro inválido												*
********************************************************************/

/********************************************************************
*	Datos del usuario como tipo de error y otras preferencias o		*
*	información que necesite ser resguardada durante el proceso		*
********************************************************************/
typedef struct {

	int error_type;

} userdata_t;
/************************************************************************
*							parseCallback								*
*************************************************************************
*	Recibe una clave y valor o un parámetro, ademas de los datos del	*
*	usuario. Los revisa según criterios específicos y devuelve un 0		*
*	si todo está bien o un 1 si no coinciden con lo que se espera.		*
*	imprime además el tipo de error que ocurrió, que puede formar parte	*
*	de una de las categorías de la tabla.								*
************************************************************************/
int parseCallback(char *key, char *value, void *userData);
/************************************************************************
*								filter1									*
*************************************************************************
*	Recibe un parámetro y revisa que se adecúe a los criterios deseados.*
*	en user_info deja constacia si hay un error							*
************************************************************************/
void filter1(char* value);
/************************************************************************
*								filter2									*
*************************************************************************
*	Recibe una opción y un valor y analiza que coincidan con lo pedido.	*
*	En user_info deja constancia de si hay un error y la naturaleza del	*
*	mismo.																*
************************************************************************/
void filter2(char *key, char* value);


int main(int argc, char*argv[]) {


	userdata_t user_info;
	void *pinfo = &user_info;
	pCallback p = parseCallback;

	int test_num;
	int result;
	/************************************************************************
	*							BANCO DE PRUEBAS							*
	************************************************************************/
	int parseCmdline(int argv, char *argc[], pCallback p, void *pinfo);
	/************************************************************************
	*							Prueba error 1								*
	************************************************************************/
	char * test1[] = { "Test1", "-font" };
	test_num = 2;
	result = parseCmdline(test_num, test1, p, pinfo);
	if (result < 0)
		printf("Error, parámetros inválidos", result);
	else
		printf("resultado: %i opciones y parámetros", result);
	/************************************************************************
	*							Prueba error 2								*
	************************************************************************/
	char * test2[] = { "Test2", "-", "432" };
	test_num = 3;
	result = parseCmdline(test_num, test2, p, pinfo);
	if (result < 0)
		printf("Error, parámetros inválidos", result);
	else
		printf("resultado: %i opciones y parámetros", result);
	/************************************************************************
	*							Prueba error 3								*
	************************************************************************/
	char * test3[] = { "Test3", "432" };
	test_num = 3;
	result = parseCmdline(test_num, test3, p, user_info);
	if (result < 0)
		printf("Error, parámetros inválidos", result);
	else
		printf("resultado: %i opciones y parámetros", result);
	return 0;
}


int parseCallback(char *key, char *value, void *userData) {
	char * clave = key;
	char * valor = value;

	/************************************************************************
	*	en user data guardaremos el tipo de error si lo hay y otra info		*
	*	relevante															*
	************************************************************************/
	userdata_t * p2userinfo = (userdata_t*)userData;
	/************************************************************************
	*	Revisa las opciones y si hay un error lo marca						*
	************************************************************************/
	if (key == NULL)
	{
		filter1(char* valor);
	}
	else
	{
		filter2(char *clave, char* valor);
	}

	//como no hay filtros esta vez se va a establecer como que todo lo recibido esta bien
	p2userinfo->error_type = 0;

	if (p2userinfo->error_type == 0)
	{
		printf("No hay errores");
		return 1;
	}
	else
	{
		printf("Error número %i", p2userinfo->error_type);
		return 0;
	}

}
/************************************************************************
*								FILTER 1								*
*************************************************************************
*	función hipotética para filtrar los parámetros según los criterios	*
*	deseados por el usuario												*
************************************************************************/
void filter1(char* value)
{

}
/************************************************************************
*								FILTER 2								*
*************************************************************************
*	función hipotética para filtrar las opciones y valores según los	*
*	criterios deseados por el usuario									*
************************************************************************/
void filter2(char *key, char* value)
{

}