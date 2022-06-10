#include <stdio.h>

typedef char *byte_pointer;

void show_bytes(byte_pointer *start, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++) {
        printf(" %x", start[i]);
    }
    printf("\n");
}

void show_int(int x)
{
    show_bytes((byte_pointer*) &x, sizeof(int));
}

void show_float(float x)
{
    show_bytes((byte_pointer*) &x, sizeof(float));
}

void show_pointer(void *x)
{
    show_bytes((byte_pointer*)&x, sizeof(void *));
}

int main()
{

    int i = 12345;
    //9 = 8+1
    //00000000 00000000 00000000 00010001
    //0x00     0x00     0x00     0x010x

    float j = (float)i;
    show_int(i);

    show_float(j);

    show_pointer(&i);

    printf("sizeof(int)=%d\n", sizeof(int));
    printf("sizeof(float)=%d\n", sizeof(float));

    return 0;
}

