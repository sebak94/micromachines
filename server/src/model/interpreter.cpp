//
// Created by mati on 11/1/19.
//

#include "../../include/model/interpreter.h"
#include "../../include/model/cars/states/accelerate.h"
#include "../../include/model/cars/states/left.h"
#include "../../include/model/cars/states/right.h"
#include "../../include/model/cars/states/break.h"
#include <memory>

CarState* Interpreter::interpret(PlayerAction pa) {
    switch (pa.getAction()) {
        case A: {
            Accelerate a;
            return std::move(&a);
        }
        case L: {
            Left l;
            return std::move(&l);
        }
        case R: {
            Right r;
            return std::move(&r);
        }
        case B: {
            Break b;
            return std::move(&b);
        }
    }
}
