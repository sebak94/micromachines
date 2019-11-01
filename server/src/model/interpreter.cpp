//
// Created by mati on 11/1/19.
//

#include "interpreter.h"

CarState* Interpreter::interpret(PlayerAction pa) {
    switch (pa.getAction()) {
        case A:
            return std::move(Accelerate());
        case L:
            return std::move(Left());
        case R:
            return std::move(Right());
        case B:
            return std::move(Break());
    }
}
