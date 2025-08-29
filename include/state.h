#ifndef STATE_H
#define STATE_H

typedef struct Move {
    int m_count;
    int c_count;
} Move;

typedef struct Side {
    int m_count;
    int c_count;
} Side;

typedef enum BoatSide {
    BOAT_SIDE_LEFT = 0,
    BOAT_SIDE_RIGHT = 1
} BoatSide;

typedef struct State {
    Side left;
    Side right;
    BoatSide boatSide;
} State;

#endif
