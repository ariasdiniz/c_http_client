#include "test_parse_request.h"
#include "test_parse_response.h"
#include <stdio.h>

int main() {
  run_parse_requests_test();
  run_parse_response_test();
  printf("All tests passed.\n");
  return 0;
}
