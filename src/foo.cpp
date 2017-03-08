// Copyright 2017

#include "./foo.h"

#include <stdio.h>

void Foo::hello() {
  printf("hello:%i\n", this->id_);
}

int Foo::getId() {
  return this->id_;
}
