#include <stdio.h>
#include <stdbool.h>
#include "Main.h"
#include <string.h>

#include "list.h"

void run(){
  printf("%s\n", "> Welcome to the Amogus Shell! Type exit or CTRL+D to exit the shell");
  while (1) {
      printf("> ");

      char d_buffer[512];

      if (fgets(d_buffer, 512, stdin) == NULL)
          break;
      if (strcmp(d_buffer, "exit\n") == 0)
          break;

      const char *s = " ;\t|><&;";
      char *token;
      /* get the first token */
      token = strtok(d_buffer, s);
      /* walk through other tokens */
      while (token != NULL) {
          printf(" %s%s\n", "<", token);

          token = strtok(NULL, s);
      }
  }
  printf("\nExiting...\n");
}
