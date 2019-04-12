#include "kconfig.h"

int a = 0;

#ifdef CONFIG_B
int b = 0;
#endif

// ifdef
void
set_values(void)
{
    a = 5;
#ifdef CONFIG_B
    b = 5;
#endif
}

// dead function
void
reset_values(void)
{
    a = 0;
#ifdef CONFIG_B
    b = 0;
#endif
}

int
main(){
    set_values();
    int c = add(a,a);
    return 0;
}
