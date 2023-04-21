#ifndef AREA_INCLUDED_H
#define AREA_INCLUDED_H

#include "Room.h"
#include <string>
#include <vector>

class Area {
    public:
        Area(const std::string& file) : areaFile(file) {}
        ~Area() {}

        void Load();

	void LoadFromFile(const std::string& file);
        static void Cleanup();

        static void Update();
        static void Draw();
        static void RoomTransition(unsigned int room, uint8_t transitionFlags);
        static Room* GetRoom() { return loadedRooms[currentRoomIn]; }

        static bool isTransitionning() { return transitionCtr > 0 && transitionCtr < TRANSITION_WAIT; }

    private:
        std::string areaFile;

        static std::vector<Room*> loadedRooms;
        static unsigned int currentRoomIn;
        static unsigned int nextRoom;

        static int transitionCtr;
        static constexpr int TRANSITION_WAIT = 10;

};

#endif
