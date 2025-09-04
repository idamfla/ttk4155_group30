#include <avr/io.h>

int main(void) {
    DDRC |= 1;
    while (1)
    {
        PORTC = 1;
    }
    
    return 0;
}
