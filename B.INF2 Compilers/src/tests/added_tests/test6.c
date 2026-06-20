struct Struct {
  int a;
  char *b;
  float c;
};

int func() {
  struct Struct a;
  return a;
}

int main() { func(); }
