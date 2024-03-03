# Aria's C HTTP Client

**Overview:**

Aria's C HTTP Client is a simple HTTP client library for the C programming language. The library allows users to easily send HTTP requests and handle responses. Please note that the project is still in development and may undergo further enhancements.

**Features:**

- **HTTP Request Parsing:** The library provides functionality to parse HTTP requests and generate request strings.
- **HTTP Response Parsing:** Users can parse HTTP responses and extract relevant information.
- **TCP-based Client:** A basic TCP client is included for sending HTTP requests and receiving responses.

**Usage:**

1. **HTTP Request:**
    - To create an HTTP request, use the `HTTPRequest` structure to specify the method, path, headers, and body.

    Example:
    ```c
    HashTable *headers = createhash();
    HTTPRequest *request = malloc(sizeof(HTTPRequest));

    request->body = "test body";
    request->method = "GET";
    request->path = "/";
    request->headers = headers;
    addtohash(headers, "key1", "val1");

    ```

2. **HTTP Response:**
    - The library supports parsing HTTP responses using the `parse_response` function.
    - The parsed response is stored in the `HTTPResponse` structure, providing access to status codes, headers, and the response body.

    Example:
    ```c
    char *response_string = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body>Hello, World!</body></html>";
    HTTPResponse *response = parse_response(response_string);
    ```

3. **TCP Client:**
    - Use the `shttp` function to send an HTTP request to a specified server and port using a TCP socket.
    - The function returns an `HTTPResponse` structure containing the parsed response.

    Example:
    ```c
    HTTPResponse *response = shttp(request, "127.0.0.1", 8080, 0);
    ```

4. **Full Usage:**

```c
// Create and mount the request
HashTable *headers = createhash();
HTTPRequest *request = malloc(sizeof(HTTPRequest));

request->body = "test body";
request->method = "GET";
request->path = "/";
request->headers = headers;
addtohash(headers, "key1", "val1");

// Call the target server
HTTPResponse *response = shttp(request, "127.0.0.1", 8080, 0);

// Use the response
printf("%s\n", response->body);
printf("%d\n", response->status);

kv = (KeyValue *)getfromindex(response->headers, 0);

printf("key: %s\n", kv->key);
printf("val: %s\n", kv->value);
destroy_response(response);
```

5. **Full Usage With SSL**

```c
// Incluse SSL libraries
#include <openssl/ssl.h>
#include <openssl/evp.h>
...
// Create and mount the request
HashTable *headers = createhash();
HTTPRequest *request = malloc(sizeof(HTTPRequest));

request->body = "test body";
request->method = "GET";
request->path = "/";
request->headers = headers;
addtohash(headers, "key1", "val1");

// Call the target server
HTTPResponse *response = shttp(request, "127.0.0.1", 8080, 1);

// Use the response
printf("%s\n", response->body);
printf("%d\n", response->status);

kv = (KeyValue *)getfromindex(response->headers, 0);

printf("key: %s\n", kv->key);
printf("val: %s\n", kv->value);
destroy_response(response);
```

*Don't forget to link `-lcrypto` and `-lss` when compiling.*

**Building:**

The library can be integrated into your C project by including the necessary header files and source files.

**Next Steps and Improvements:**

- [x] **Support for TLS/SSL Security:** Enhance the library to support secure communication using TLS/SSL.
- [ ] **Support for Encoded Responses:** Implement support for decoding various content encodings in HTTP responses.
- [ ] **Support for Cache:** Integrate caching mechanisms to improve performance and reduce redundant requests.

**Contributions:**

Contributions to the project are welcome. If you find any issues, have feature requests, or want to contribute to the development, please create an issue or submit a pull request.

**License:**

This project is licensed under the MIT License.

**Disclaimer:**

Aria's C HTTP Client is still in development, and its features may be subject to change. Use it at your own discretion.
