#ifndef	__SCREEN_H
#define __SCREEN_H
extern "C"
{
	#include "sys.h"
	#include "usart.h"
}

class	screen
{
public:
	screen(u32 baudrate = 9600);
	~screen();

	void update(u8 vol,u8 channel = 0);
	void clear_screen(void);
	void txt_send(u16 val,const char* id,const char* name);
	void info_send(const char* info,const char* id);
	
private:

};

#endif
