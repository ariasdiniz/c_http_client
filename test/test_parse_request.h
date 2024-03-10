#include "../include/httpclient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void test_iterate_over_null_hash() {
  HTTPRequest *request = (HTTPRequest *)malloc(sizeof(HTTPRequest));
  request->body = "body";
  request->headers = NULL;
  request->method = "GET";
  request->path = "/test";
  char *result = parse_request(request);
  char *expected_result = "GET /test HTTP/1.1\r\n\r\nbody\r\n";
  assert(strcmp(result, expected_result) == 0);
  free(result);
  printf("test_iterate_over_null_hash passed.\n");
}

void test_iterate_over_hash_empty() {
  HashTable *headers = createhash();
  HTTPRequest *request = (HTTPRequest *)malloc(sizeof(HTTPRequest));
  request->body = "body";
  request->headers = headers;
  request->method = "GET";
  request->path = "/test";
  char *result = parse_request(request);
  char *expected_result = "GET /test HTTP/1.1\r\n\r\nbody\r\n";
  assert(strcmp(result, expected_result) == 0);
  free(result);
  printf("test_iterate_over_empty_hash passed.\n");
}

void test_iterate_over_hash() {
  HashTable *headers = createhash();
  HTTPRequest *request = (HTTPRequest *)malloc(sizeof(HTTPRequest));
  addtohash(headers, "key1", "value1");
  addtohash(headers, "key1", "value");
  addtohash(headers, "key2", "value2");
  request->body = "body";
  request->headers = headers;
  request->method = "GET";
  request->path = "/test";
  char *result = parse_request(request);
  char *expected_result = "GET /test HTTP/1.1\r\nkey1: value1\r\nkey1: value\r\nkey2: value2\r\n\r\nbody\r\n";
  assert(strcmp(result, expected_result) == 0);
  free(result);
  printf("test_iterate_over_hash passed.\n");
}

void run_parse_requests_test() {
  printf("----------STARTING PARSE REQUEST TEST-----------\n");
  test_iterate_over_null_hash();
  test_iterate_over_hash_empty();
  test_iterate_over_hash();
  printf("----------FINISHING PARSE REQUEST TEST----------\n");
}
