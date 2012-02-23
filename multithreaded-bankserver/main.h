#ifndef MAIN_H
#define MAIN_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "errors.h"
#include "requests.h"


int appserver(int argc, char **argv, int defaultLockMode);

#endif
