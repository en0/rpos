#include <stdint.h>
#define VIDEO_COLOR 0x80

static inline int _set_color(char character, int color) {
    return (((int)character) & 0xFF) | (color << 8);
}

void main() {

    int i;
    char* msg = "!!! I am a external program! - Hello, World !!!\n";
    uint16_t* _video_memory = (void*)0xc00B8000;

    for(i = 0; msg[i] != '\0'; i++)
        _video_memory[i] = _set_color(msg[i], VIDEO_COLOR);

    for(;;);
}
