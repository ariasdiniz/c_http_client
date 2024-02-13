#include "parse_response.h"
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>

#define __FIRST_LINE_FLAG 0x0001
#define __BODY_FLAG 0x0002

/**
 * @brief Extracts the HTTP response code from the given line using regex.
 *
 * @param line The line containing the HTTP response code.
 * @return The HTTP response code as an integer, or -1 if an error occurs.
 */
static int get_response_code(char *line) {
  regex_t regex;
  int ret;
  regmatch_t pmatch[1];

  // Compile the regular expression for matching the response code
  ret = regcomp(&regex, "([0-9]{3})", REG_EXTENDED);
  if (ret) {
    return -1;
  }

  // Execute the regular expression
  ret = regexec(&regex, line, 1, pmatch, 0);
  if (!ret) {
    char code_str[4] = {0};
    // Extract the matched response code substring
    strncpy(code_str, line + pmatch[0].rm_so, 3);
    regfree(&regex);
    code_str[3] = '\0';
    // Convert the response code string to an integer
    return strtol(code_str, '\0', 10);
  } else {
    regfree(&regex);
    return -1;
  }
}

/**
 * @brief Parses a header line and returns a KeyValue structure.
 *
 * @param line The header line to be parsed.
 * @return A KeyValue structure containing the parsed key-value pair.
 */
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
      // Copy the key from the buffer
      memcpy(kv->key, buffer, count);
      count = 0;
      memset(buffer, '\0', count);
      j++;
      continue;
    }
    buffer[count] = line[j];
    count += 1;
  }
  // Copy the value from the buffer
  memcpy(kv->value, buffer, count);
  return kv;
}

/**
 * @brief Frees the memory allocated for the HTTPResponse structure.
 *
 * @param response The HTTPResponse structure to be destroyed.
 * @return 0 on success, -1 if the input is NULL.
 */
int destroy_response(HTTPResponse *response) {
  if (response == NULL) {
    return -1;
  }
  KeyValue *item;
  // Free memory for each header key-value pair
  for (int i = 0; i < response->headers->size; i++) {
    item = (KeyValue *)getfromindex(response->headers, i);
    free(item->key);
    free(item->value);
    free(item);
  }
  // Delete the list containing header key-value pairs
  deletelist(response->headers);
  // Free memory for the response body
  free(response->body);
  // Free memory for the entire HTTPResponse structure
  free(response);
  return 0;
}

/**
 * @brief Parses the entire HTTP response and returns an HTTPResponse structure.
 *
 * @param response The raw HTTP response string to be parsed.
 * @return An HTTPResponse structure containing parsed information, or NULL on error.
 */
HTTPResponse *parse_response(char *response) {
  if (response == NULL) {
    return NULL;
  }
  HTTPResponse *http_response = malloc(sizeof(HTTPResponse));
  if (http_response == NULL || response == NULL) {
    return NULL;
  }

  int buffer_size = strlen(response);
  http_response->body = malloc(sizeof(char) * buffer_size);
  if (http_response->body == NULL) {
    return NULL;
  }

  // Create a list to store header key-value pairs
  http_response->headers = createlist();
  char buffer[buffer_size];
  KeyValue *temp;
  unsigned int counter = 0;
  int flag = 0;
  memset(buffer, '\0', buffer_size);

  for (int i = 0; i < buffer_size; i++) {
    buffer[counter] = response[i];
    counter++;
    switch (flag) {

    case 0:
      if (response[i] == '\r') {
        // Parse the first line to extract the response code
        http_response->status = get_response_code(buffer);
        if (http_response->status == -1) {
          destroy_response(http_response);
          return NULL;
        }
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
        // Parse header lines and add them to the list
        temp = get_header(buffer);
        if (temp == NULL) {
          destroy_response(http_response);
          return NULL;
        }
        addtolist(http_response->headers, temp);
        counter = 0;
        memset(buffer, '\0', buffer_size);
      }
      if (response[i] == '\r' && response[i + 1] == '\n' && response[i + 2] == '\r' && response[i + 3] == '\n') {
        // Detect the end of headers and start of the body
        counter = 0;
        flag |= __BODY_FLAG;
        i += 3;
      }
    }
  }
  if (flag < 1) {
    destroy_response(http_response);
    return NULL;
  }
  // Copy the remaining content as the response body
  strncpy(http_response->body, buffer, buffer_size-1);
  return http_response;
}
