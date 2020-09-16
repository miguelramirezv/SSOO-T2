#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"

static void remove_trailing(char *string, char c)
{
  int last_index = strlen(string) - 1;
  if (string[last_index] == c)
  {
    string[last_index] = 0;
  }
}

static void argv_init(Args* args)
{
  for (size_t i = 0; i < MAX_ARGS; i++)
  {
    args->argv[i] = calloc(MAX_LEN, sizeof(char));
  }
}

static void argv_destroy(Args *args)
{
  for (size_t i = 0; i < MAX_ARGS; i++)
  {
    if (args->argv[i]) {
      free(args->argv[i]);
    }
  }
}

Args* args_init()
{
  Args* args = malloc(sizeof(Args));
  argv_init(args);  
  args->argc = -1;
  
  return args;
}

void args_get(Args* args)
{
  printf("$\n$ ");

  // Reset argv
  argv_destroy(args);
  argv_init(args);

  // Get command
  scanf("%s", args->command);
  remove_trailing(args->command, '\n');

  // Get arguments
  char line[255];
  fgets(line, 255, stdin);
  remove_trailing(line, '\n');

  // Divide by space
  int index = 0;
  char *arg;
  arg = strtok(line, " ");

  while( arg != NULL ) {
    strcpy(args->argv[index++], arg);
    arg = strtok(NULL, " ");
  }
  free(args->argv[index]);
  args->argv[index] = NULL;

  // Set amount of arguments
  args->argc = index;
}

void args_destroy(Args* args)
{
  if (!args) return;
  argv_destroy(args);
  free(args);
}