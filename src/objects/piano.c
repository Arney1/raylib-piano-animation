#include "piano.h"

bool states[7*7 + 1];

void Piano_Init(void) {
    for (int i = 0; i < 7 * 7 + 1; i++) {
        states[i] = false;
    }
}
void Piano_Update(int *flip) {

}
void Piano_Draw(Vector2 position_start, Vector2 position_end) {

}
void Piano_Unload(void) {

}
