#include "../include/player_action.h"

PlayerAction::PlayerAction(char action) {
    switch (action) {
        case 'A': this->action = A; break;
        case 'B': this->action = B; break;
        case 'L': this->action = L; break;
        case 'R': this->action = R; break;
    }
}

Action PlayerAction::getAction() {
    return action;
}

PlayerAction::~PlayerAction() {}
