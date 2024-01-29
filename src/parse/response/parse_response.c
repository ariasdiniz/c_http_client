#include "parse_response.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>

#define __FIRST_LINE_FLAG 0x0001
#define __BODY_FLAG 0x0002

static int get_response_code(char *line) {
  regex_t regex;
  int ret;
  regmatch_t pmatch[1];
  
  ret = regcomp(&regex, "([0-9]{3})", REG_EXTENDED);
  if (ret) {
    return -1;
  }

  ret = regexec(&regex, line, 1, pmatch, 0);
  if (!ret) {
    char code_str[4] = {0};
    strncpy(code_str, line + pmatch[0].rm_so, 4);
    regfree(&regex);
    code_str[3] = '\0';
    return strtol(code_str, '\0', 10);
  } else {
    regfree(&regex);
    return 0;
  }
}

static KeyValue *get_header(char *line) {
  int line_len = strlen(line);
  char buffer[line_len];
  KeyValue *kv = malloc(sizeof(KeyValue));
  if (kv == NULL) {
    return NULL;
  }
  kv->key = malloc(sizeof(char) * line_len);
  if (kv->key == NULL) {
    return NULL;
  }
  kv->value = malloc(sizeof(char) * line_len);
  if (kv->value == NULL) {
    return NULL;
  }
  memset(buffer, '\0', line_len);
  memset(kv->key, '\0', line_len);
  memset(kv->value, '\0', line_len);
  int count = 0;

  for (int j = 0; line[j] != '\r' && line[j + 1] != '\n'; j++) {
    if (line[j] == ':') {
      memcpy(kv->key, buffer, count);
      count = 0;
      memset(buffer, '\0', count);
      j++;
      continue;
    }
    buffer[count] = line[j];
    count += 1;
  }
  memcpy(kv->value, buffer, count);
  return kv;
}

HTTPResponse *parse_response(char *response) {
  HTTPResponse *http_response = malloc(sizeof(HTTPResponse));
  if (response == NULL) {
    return NULL;
  }

  int buffer_size = strlen(response);
  http_response->headers = createlist();
  char buffer[buffer_size];
  unsigned int counter = 0;
  int flag = 0;
  memset(buffer, '\0', buffer_size);

  for (int i = 0; i < buffer_size; i++) {
    buffer[counter] = response[i];
    counter++;
    switch (flag) {

    case 0:
      if (response[i] == '\r') {
        http_response->status = get_response_code(buffer);
        flag |= __FIRST_LINE_FLAG;
        counter = 0;
        i++;
        memset(buffer, '\0', buffer_size);
        continue;
      }
      break;

    case 1:
      if (response[i] == '\r') {
        i++;
        addtolist(http_response->headers, get_header(buffer));
        counter = 0;
        memset(buffer, '\0', buffer_size);
      }
      if (response[i] == '\r' && response[i + 1] == '\n' && response[i + 2] == '\r' && response[i + 3] == '\n') {
        counter = 0;
        flag |= __BODY_FLAG;
        i += 3;
      }
    }
  }
  http_response->body = buffer;
  return http_response;
}

int destroy_response(HTTPResponse *response) {
  if (response == NULL) {
    return -1;
  }
  KeyValue *item;
  for (int i = 0; i < response->headers->size; i++) {
    item = (KeyValue *)getfromindex(response->headers, i);
    free(item->key);
    free(item->value);
    free(item);
  }
  deletelist(response->headers);
  free(response);
  return 0;
}
