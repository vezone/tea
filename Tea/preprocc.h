#pragma once


#include <stdarg.h>

#define DEBUG 1
#if DEBUG == 1
	#define Print(x, param, ...) {\
		va_list args;\
		g_print(x, va_arg(args, param), va_arg(args, int));\
	}
#else
	#define Print(x) 
#endif


#include <pthread.h>

#define str(x) #x
#define log(x) g_print(str(x));
#define con2(x,y) x##y
#define con3(x,y,z) x##y##z

void Action(void)
{
	log(Action comleted!\n);
}

void main_tread(void)
{
	/*
	log(All is working!\n);
	log(con2(Some, thing)\n);
	
	pthread_t t_id;
	pthread_attr_t t_attr;
	*/
	
	//pthread_attr_init(&t_attr);
	//error
	//pthread_create(&t_id, &t_attr, Action, 1);
	//pthread_join(t_id, NULL);
}
