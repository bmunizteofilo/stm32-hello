#include <assert.h>
#include <stdio.h>

int main(void) {
    printf("[sanity] rodando teste básico...\n");
    assert(2 + 2 == 4);
    printf("[sanity] OK!\n");
    return 0;
}
