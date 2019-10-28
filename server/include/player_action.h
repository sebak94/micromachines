#ifndef __PLAYER_ACTION_H__
#define __PLAYER_ACTION_H__

// A: accelerate
// B: brake
// L: left
// R: right
enum Action { A, B, L, R };

class PlayerAction {
    private:
    Action action;

    public:
    explicit PlayerAction(char action);
    Action getAction();
    ~PlayerAction();
};

#endif
