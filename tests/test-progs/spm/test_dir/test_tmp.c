#include <stdio.h>
#include <stdlib.h>
#include "m5/m5op.h"
int main()
{
    m5_checkpoint(0,0);
    double *p = (double *) spm_malloc(1024*1024,1);
    m5_dumpreset_stats(0,0);

    return 0;
}
