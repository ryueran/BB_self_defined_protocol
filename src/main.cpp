#include <cstdio>
#include <unistd.h>
#include <vector>

void* ptr = new int(42);
int* iptr = static_cast<int*>(ptr);

void func(int n) {
    std::vector<int> a (n, 0);
}

int main() {
    printf("%d", 123);
}