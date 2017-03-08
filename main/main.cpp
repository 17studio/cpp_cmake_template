// Copyright 2017

#include <uv.h>
#include "./foo.h"

int counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
  counter++;

  if (counter >= 10e3)
    uv_idle_stop(handle);
}

int main(int argc, char** argv) {
  Foo foo(1);
  foo.hello();

  uv_loop_t* loop = uv_default_loop();

  uv_idle_t idler;
  uv_idle_init(loop, &idler);
  uv_idle_start(&idler, wait_for_a_while);

  uv_run(loop, UV_RUN_DEFAULT);
  uv_loop_close(loop);

  return 0;
}
