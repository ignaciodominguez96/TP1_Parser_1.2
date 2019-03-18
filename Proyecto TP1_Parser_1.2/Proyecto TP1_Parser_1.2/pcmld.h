#ifndef PCMDL_H
#define PCMDL_H

#define PARSER_ERROR -1

typedef int(*pCallback) (char *, char*, void *);

int parseCmdline(int argv, char *argc[], pCallback p, void *userData);

#endif  PCMDL_H
