#include "../include/httpclient.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void test_get_response_code() {
  HTTPResponse *buffer = malloc(sizeof(HTTPResponse));
  buffer->body = malloc(sizeof(char) * 50);
  parse_response("HTTP/1.1 200 OK\r\n", buffer);
  // Test a valid response code
  assert(buffer->status == 200);
  destroy_response(buffer);
  printf("test_get_response_code passed.\n");
}

void test_get_header() {
  // Test a valid header line
  char *valid_response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
  
  HTTPResponse *response = malloc(sizeof(HTTPResponse));
  response->body = malloc(sizeof(char) * 50);
  parse_response(valid_response, response);
  KeyValue *kv = (KeyValue *)getfromindex(response->headers, 0);
  assert(strcmp(kv->key, "Content-Type") == 0);
  assert(strcmp(kv->value, "application/json") == 0);
  destroy_response(response);
  printf("test_get_header passed.\n");
}

void test_parse_response() {
  // Test parsing a valid HTTP response string
  char *valid_response_str = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, World!";
  HTTPResponse *buffer = malloc(sizeof(HTTPResponse));
  buffer->body = malloc(sizeof(char) * 100);
  parse_response(valid_response_str, buffer);
  assert(buffer != NULL);
  assert(buffer->status == 200);
  assert(strcmp(buffer->body, "Hello, World!") == 0);

  // Parse another response with same buffer
  char *another_valid_response_str = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nThis is Valid";
  parse_response(another_valid_response_str, buffer);
  KeyValue *kv = (KeyValue *)getfromindex(buffer->headers, 0);
  assert(strcmp(kv->key, "Content-Type") == 0);
  assert(strcmp(kv->value, "text/plain") == 0);
  assert(buffer->status == 200);
  assert(strcmp(buffer->body, "This is Valid") == 0);

  // // Test parsing an invalid HTTP response string (should return NULL)
  char *invalid_response_str = "Invalid Response";
  assert(parse_response(invalid_response_str, buffer) == NULL);

  // Test parsing a NULL HTTP response string (should return NULL)
  assert(parse_response(NULL, buffer) == NULL);

  // Test parsing a response with an invalid response code (should return NULL)
  char *invalid_code_response_str = "HTTP/1.1 InvalidCode OK\r\n\r\nHello, World!";
  assert(parse_response(invalid_code_response_str, buffer) == NULL);

  destroy_response(buffer);

  printf("test_parse_response passed.\n");
}

void run_parse_response_test() {
  printf("----------STARTING PARSE RESPONSE TEST-----------\n");
  test_get_response_code();
  test_get_header();
  test_parse_response();
  printf("----------FINISHING PARSE RESPONSE TEST----------\n");
}
