#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define ENVF_MAX_PATH 4096
#define ENVF_MAX_VAR_LEN 1024

bool is_var_char(char c) {
  return (c == '_') || (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

void read_replace(FILE *source, FILE *dest) {
  int buffer_pos = 0;
  char buffer[ENVF_MAX_VAR_LEN] = {'\0'};

  int current_char = 0;

  char *variable;
  bool in_variable = false;

  while ((current_char = fgetc(source)) != EOF) {
    if (!in_variable && current_char == '%') {
      in_variable = true;
      continue;
    }

    // When we get a closing % try and replace the string with the env
    if (in_variable && current_char == '%') {
      buffer[buffer_pos] = '\0';
      variable = getenv(buffer);
      if (variable != NULL) {
        fprintf(dest, "%s", variable);
      } else {
        fprintf(dest, "%%%s%%", buffer);
      }
      in_variable = false;
      buffer[0] = '\0';
      buffer_pos = 0;
      continue;
    }

    if (in_variable) {
      buffer[buffer_pos++] = current_char;
      buffer[buffer_pos] = '\0';

      // If we're about to exhaust our buffer, give up on this variable and print
      if ((buffer_pos + 1 >= ENVF_MAX_VAR_LEN) || !is_var_char(current_char)) {
        fprintf(dest, "%%%s", buffer);
        in_variable = false;
        buffer[0] = '\0';
        buffer_pos = 0;
      }
      continue;
    }

    fprintf(dest, "%c", current_char);
  }
}


int main(int argc, char *argv[]) {
  FILE *source;
  FILE *dest;
  char destination[ENVF_MAX_PATH] = {'\0'};
  char *extension;

  if (argc <= 1) {
    printf("Missing file argument.\n");
    exit(EXIT_FAILURE);
  }

  extension = strrchr(argv[1], '.');
  if (extension == NULL || strcmp(extension, ".env")) {
    printf("Filename missing extension .env %s, found %s.\n", argv[1], extension);
    exit(EXIT_FAILURE);
  }

  source = fopen(argv[1], "r");
  if (source == NULL) {
    printf("Could not open source file %s.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  strcpy(destination, argv[1]);
  destination[strlen(destination) - strlen(extension)] = '\0';
  dest = fopen(destination, "w+");
  if (dest == NULL) {
    printf("Could not open destination file %s.\n", destination);
    fclose(source);
    exit(EXIT_FAILURE);
  }

  read_replace(source, dest);


  fclose(source);
  fclose(dest);
  exit(EXIT_SUCCESS);
}
