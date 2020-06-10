#ifndef ARGUS_API_H_

#define ARGUS_API_H_

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include "task_struct.h"
#include "argus_client.h"

void export_task(Task t);
Task read_task(char* buffer);
char** separate_commands(char* cmds);

#endif

