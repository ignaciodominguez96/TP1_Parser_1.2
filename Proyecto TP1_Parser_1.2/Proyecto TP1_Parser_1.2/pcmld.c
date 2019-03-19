#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pcmld.h"

int parseCmdline(int argv, char* argc[], pCallback p, void* userData)
{
	int opciones = 0, parametros = 0;		//inicializo contadores de parametros y opciones

	if (argv == 1)		//marca error si esta solo el nombre
	{
		printf("No hay parametros ni opciones\n");
		return PARSER_ERROR;
	}

	for (int i = 1; i < argv; ++i)		//recorro argc
	{
		if (*argc[i] == '-')		//si es una opcion
		{
			if (argc[i][1] == '\0') //marca error si la opcion no tiene nombre (hay un '-' solo)
			{
				printf("Opcion sin nombre\n");
				return PARSER_ERROR;
			}
			
			else if ((i + 1) == argv)		//marca error si no tiene valor
			{
				printf("Opcion sin valor\n");
				return PARSER_ERROR;
			}
			else
			{
				argc[i] = &argc[i][1];		//tomo el nombre de la opcion, sin el '-'
				if (!p(argc[i], argc[i + 1], userData)) //si lafuncion callback tira error, termina
				{
					return PARSER_ERROR;
				}
				i++;
				opciones++;
			}
		}
		else
		{
			if (!p(NULL, argc[i], userData))	//si la funcion callback tira error, termina
			{
				return PARSER_ERROR;
			}
			parametros++;
		}
	}
	return (opciones + parametros);		//devuelve la suma entre la cantidad de parametros y opciones
}