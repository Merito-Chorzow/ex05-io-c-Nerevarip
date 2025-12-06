#include "ringbuf.h"

void rb_init(rb_t* r){ r->head = r->tail = r->dropped = 0; }

size_t rb_count(const rb_t* r){
    // Arytmetyka modulo dla bufora cyklicznego
    if (r->head >= r->tail) return r->head - r->tail;
    return RB_SIZE + r->head - r->tail;
}

size_t rb_free(const rb_t* r){
    // Zostawiamy 1 bajt wolny, aby odróżnić pełny od pustego (head==tail to pusty)
    return RB_SIZE - 1 - rb_count(r);
}

int rb_put(rb_t* r, uint8_t b){
    if (rb_free(r) == 0){
        r->dropped++; // POLITYKA: DROP-NEW (odrzucamy nadchodzący bajt)
        return 0;
    }
    r->q[r->head] = b;
    r->head = (r->head + 1) % RB_SIZE;
    return 1;
}

int rb_get(rb_t* r, uint8_t* out){
    if (r->head == r->tail) return 0; // Pusty
    *out = r->q[r->tail];
    r->tail = (r->tail + 1) % RB_SIZE;
    return 1;
}