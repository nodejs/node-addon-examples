#include "libHelloWorld.h"
#include <string.h>

char * HelloWorldConcat (char * cHello, char * cWorld){
	char * cHelloWorld = NULL;
	
	cHelloWorld = strcat(cHello, cWorld);
	
	return cHelloWorld;
}
