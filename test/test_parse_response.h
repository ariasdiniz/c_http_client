#include "../include/httpclient.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_get_response_code() {
  HTTPResponse *response = parse_response("HTTP/1.1 200 OK\r\n");
  // Test a valid response code
  assert(response->status == 200);
  destroy_response(response);
  printf("test_get_response_code passed.\n");
}

void test_get_header() {
  // Test a valid header line
  char *valid_response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
  
  HTTPResponse *response = parse_response(valid_response);
  KeyValue *kv = (KeyValue *)getfromindex(response->headers, 0);
  assert(strcmp(kv->key, "Content-Type") == 0);
  assert(strcmp(kv->value, "application/json") == 0);
  destroy_response(response);
  printf("test_get_header passed.\n");
}

void test_parse_response() {
  // Test parsing a valid HTTP response string
  char *valid_response_str = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
  HTTPResponse *valid_response = parse_response(valid_response_str);
  assert(valid_response != NULL);
  assert(valid_response->status == 200);
  assert(strcmp(valid_response->body, "Hello, World!") == 0);
  destroy_response(valid_response);

  // Test parsing an invalid HTTP response string (should return NULL)
  char *invalid_response_str = "Invalid Response";
  assert(parse_response(invalid_response_str) == NULL);

  // Test parsing a NULL HTTP response string (should return NULL)
  assert(parse_response(NULL) == NULL);

  // Test parsing a response with an invalid response code (should return NULL)
  char *invalid_code_response_str = "HTTP/1.1 InvalidCode OK\r\n\r\nHello, World!";
  assert(parse_response(invalid_code_response_str) == NULL);

  printf("test_parse_response passed.\n");
}

void run_parse_response_test() {
  printf("----------STARTING PARSE RESPONSE TEST-----------\n");
  test_get_response_code();
  test_get_header();
  test_parse_response();
  printf("----------FINISHING PARSE RESPONSE TEST----------\n");
}
