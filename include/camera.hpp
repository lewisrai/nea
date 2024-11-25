#ifndef HEADER_CAMERA
#define HEADER_CAMERA

#include <raylib.h>
#include "player.hpp"
#include "window.hpp"


// Define //

class CameraView {
  public:
    // Init
    CameraView(const Window &window);

    // Method - updates camera zoom + position
    void Update(const PlayerArray &players);

    // Getter
    Camera2D GetCamera() const;

  private:
    Camera2D m_camera = {{0, 0}, {0, 0}, 0, 0}; // Setting - offset, position, zoom, rotation

    const float m_baseZoom; // Limit
    const float m_maxX;     // Limit
    const float m_maxY;     // Limit
};


#endif
