#ifndef CAMERA_INCLUDED_H
#define CAMERA_INCLUDED_H

class Camera {
    public:
        Camera();
        ~Camera();

        static Camera* GameCamera();
        void Cleanup();

        void SetBoundaries(float x1, float y1, float x2, float y2);
        void SetPos(float x, float y);
        void SetZ(float z);

    private:

        void GetViewSize(float& w, float& h);

        float bx1, by1, bx2, by2;
        float z;
        static Camera* gameCamera;
};

#endif
