#include <stdio.h>
#include "bitmap.hh"
//using namespace std;

int main()
{
    Bitmap b(10);
    for (int i = 0; i < 10; i++) {
        b.set_bit(i);
    }
    printf("%d", b.is_full());
    return 0;
}
