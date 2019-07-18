#include "LSTD_TYPES.h"
#include "LUTILS.h"

#include "BootLoader_interface.h"

void main (void)
{
	BootLoader_voidInit();
	BootLoader_voidStart();
	while (1);
}
