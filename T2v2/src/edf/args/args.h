#define MAX_ARGS 255
#define MAX_LEN 255

typedef struct args {
  char command[MAX_LEN];
  char *argv[MAX_ARGS];
  int argc;
} Args;

Args* args_init();
void args_get(Args* args);
void args_destroy(Args* args);
