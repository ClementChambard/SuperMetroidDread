#ifndef DOOR_INCLUDED_H
#define DOOR_INCLUDED_H

#include "CollisionMap.h"
#include "Samus.h"

struct DoorHatch;

class Door {
    public:
        Door(int centerPositionX, int centerPositionY, bool isleft, int hatch = 0);
        ~Door();

        void Open();
        void Close();

        void Update(Samus* s);
        void Draw();

    private:
        CollisionAlteration collisions{};
        DoorHatch* hatch = nullptr;
        int openCounter = 300;
        glm::vec2 pos;
};

struct DoorHatch {
    int type;
    bool broken;
};

#endif
