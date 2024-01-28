#include "parse_response.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define __RESPONSE_BUFFER_SIZE 65536
#define __FIRST_LINE_FLAG 0x0001
#define __BODY_FLAG 0x0002

static int get_response_code(char *line) {
  regex_t regex;
  int ret;
  regmatch_t pmatch[1];
  return 0;
  ret = regcomp(&regex, "[0-9]{3}", REG_EXTENDED);
  if (ret) {
      return -1;
  }
  ret = regexec(&regex, line, 1, pmatch, 0);
  if (!ret) {
    return *(int *)&line[pmatch[0].rm_so];
  } else {
    return 0;
  }
}

static KeyValue *get_header(char *line) {
  char buffer[__RESPONSE_BUFFER_SIZE];
  KeyValue *kv = malloc(sizeof(KeyValue));
  if (kv == NULL) {
    return NULL;
  }
  int count = 0;
  for (int j = 0; line[j] != '\n'; j++) {
    if (line[j] == ':') {
      kv->key = buffer;
      count = 0;
    }
    buffer[count] = line[j];
    count += 1;
  }
  return kv;
}

HTTPResponse *parse_response(char *response) {
  HTTPResponse *http_response = malloc(sizeof(HTTPResponse));
  if (response == NULL) {
    return NULL;
  }
  http_response->headers = malloc(sizeof(LinkedList));
  char buffer[__RESPONSE_BUFFER_SIZE];
  unsigned int counter = 0;
  int flag = 0;
  memset(buffer, '\0', __RESPONSE_BUFFER_SIZE);
  for (int i = 0; i < __RESPONSE_BUFFER_SIZE && response[i] != '\0'; i++) {
    switch (flag) {
    case 0:
      buffer[counter] = response[i];
      counter += 1;
      if (response[i] == '\n') {
        http_response->status = get_response_code(buffer);
        flag |= __FIRST_LINE_FLAG;
        counter = 0;
        memset(buffer, '\0', __RESPONSE_BUFFER_SIZE);
      }
    case 1:
      buffer[counter] = response[i];
      counter += 1;
      if (response[i] == '\n') {
        addtolist(http_response->headers, get_header(buffer));
        counter = 0;
        memset(buffer, '\0', __RESPONSE_BUFFER_SIZE);
      }
      if (response[i] == '\n' && response[i + 1] == '\n') {
        flag |= __BODY_FLAG;
      }
    default:
      buffer[counter] = response[i];
      counter += 1;
    }
  }
  http_response->body = buffer;
  return http_response;
}
