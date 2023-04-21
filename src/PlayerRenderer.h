#ifndef PLAYERRENDERER_INCLUDED_H
#define PLAYERRENDERER_INCLUDED_H

#include "Samus.h"

class PlayerRenderer {
    public:
        PlayerRenderer();
        ~PlayerRenderer();

        void RenderSamus(Samus* s);
};

#endif
