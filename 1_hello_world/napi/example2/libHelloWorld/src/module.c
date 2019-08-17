#include <node_api.h>
#include <stdlib.h>
#include <stdio.h>
#include "libHelloWorld.h"

napi_value hello_world_concat (napi_env env, napi_callback_info info) {

  size_t argc = 2;
  size_t iTotalSize = 0;

  napi_value args[2];

  napi_get_cb_info(env, info, &argc, args, NULL, NULL);

  size_t str_size;
  size_t str_size_read;
  
  napi_get_value_string_utf8(env, args[0], NULL, 0, &str_size);
  char * cHello;
  cHello = (char*)calloc(str_size + 1, sizeof(char));
  str_size = str_size + 1;
  napi_get_value_string_utf8(env, args[0], cHello, str_size, &str_size_read);

  iTotalSize = iTotalSize + str_size_read;

  napi_get_value_string_utf8(env, args[1], NULL, 0, &str_size);
  char * cWorld;
  cWorld = (char*)calloc(str_size + 1, sizeof(char));
  str_size = str_size + 1;
  napi_get_value_string_utf8(env, args[1], cWorld, str_size, &str_size_read);

  iTotalSize = iTotalSize + str_size_read;

  char * cResult = HelloWorldConcat(cHello, cWorld);
  //printf("%s, %s \n", "Hello", cResult); // Debug output: Hello, world

  napi_value result;

  napi_create_string_utf8(env, cResult, iTotalSize, &result);
  free(cHello);
  free(cWorld);
  return result;
}

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_value fn;

  status = napi_create_function(env, NULL, 0, hello_world_concat, NULL, &fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to wrap native function");
  }

  status = napi_set_named_property(env, exports, "helloWorldConcat", fn);
  if (status != napi_ok) {
    napi_throw_error(env, NULL, "Unable to populate exports");
  }

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
