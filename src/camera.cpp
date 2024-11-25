#include "camera.hpp"

// Init //

CameraView::CameraView(const Window &window) : m_baseZoom(window.width / BASE_WINDOW_WIDTH), m_maxX(window.width / 2), m_maxY(window.height / 2) {
  m_camera.offset.x = window.width / 2;
  m_camera.offset.y =  window.height / 2;
  m_camera.zoom = m_baseZoom;
}


// Public Methods //

void CameraView::Update(const PlayerArray &players) {
  const Vector2 p0Position = players[0].GetCentre();
  const Vector2 p1Position = players[1].GetCentre();

  //Set Camera Zoom
  float cameraScaleX = (p0Position.x - p1Position.x) / m_maxX;
  float cameraScaleY = (p0Position.y - p1Position.y) / m_maxY;
  if (cameraScaleX < 0) cameraScaleX *= -1;
  if (cameraScaleY < 0) cameraScaleY *= -1;

  if (cameraScaleX > cameraScaleY && cameraScaleX > 1) m_camera.zoom = m_baseZoom / cameraScaleX;
  else if (cameraScaleY > 1) m_camera.zoom = m_baseZoom / cameraScaleY;
  else m_camera.zoom = m_baseZoom;

  if (m_camera.zoom < m_baseZoom / 1.5) m_camera.zoom = m_baseZoom / 1.5;

  //Set Camera Target
  m_camera.target.x = (p0Position.x + p1Position.x) / 2;
  m_camera.target.y = (p0Position.y + p1Position.y) / 2;
}


// Getters //

Camera2D CameraView::GetCamera() const {return m_camera;}
