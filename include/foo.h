// Copyright

#ifndef INCLUDE_FOO_H_
#define INCLUDE_FOO_H_

class Foo {
 public:
  explicit Foo(int v) : id_(v) {}

  void hello();
  int getId();

 private:
  int id_ = 10;
};

#endif  // INCLUDE_FOO_H_
