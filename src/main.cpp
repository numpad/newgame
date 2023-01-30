#include <bgfx/bgfx.h>
#include <stdio.h>

int main() {
	printf(" .~* Hello *~.\n");
#ifdef HELLO_WORLD
	printf(" `~. World .~´\n");
#endif
	//bgfx::init();
	
	return 0;
}

