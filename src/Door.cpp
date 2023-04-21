#include "Door.h"
#include "Area.h"

Door::Door(int x, int y, bool L, int hatch)
{
    if (hatch != 0) {this->hatch = new DoorHatch(); this->hatch->type = hatch; this->hatch->broken = false; }
    collisions.w = 1; collisions.h = 3;
    collisions.startx = x; collisions.starty = y;
    collisions.tiles = { 0, 0, 0};
    pos = {x*24,y*24};
    Area::GetRoom()->getCol()->Alter(&collisions);
}
Door::~Door() {}

void Door::Open() { collisions.tiles = { 16, 16, 16}; openCounter = 0; }
void Door::Close() { collisions.tiles = {0, 0, 0}; openCounter = 301; }

void Door::Update(Samus* s)
{
    if (openCounter < 300) openCounter++;
    else
    {
        if (openCounter == 300) Close();
        Bullet* samusbullets = s->getBullets();
        for (int i = 0; i < 50; i++)
        {
            if (samusbullets[i].isActive())
            {
                glm::vec2 p = (-Area::GetRoom()->worldPos + samusbullets[i].pos+samusbullets[i].spd)*glm::vec2(1,-1);
                std::cout << p.x << "," << p.y << " " << pos.x << "," << pos.y << "\n";
                //check for open
                if (p.x < pos.x+24 && p.x > pos.x &&
                    p.y < pos.y+72 && p.y > pos.y) {
                    samusbullets[i].Break();
                    Open();
                }
            }
        }
    }
}

void Door::Draw()
{

}
