#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE

#include "Camera.h"
#include <iostream>
#include <math.h> /* fabs */
#include "../CoreManager.h"



Camera::Camera()
{
    camera_x = 0;
}

void Camera::zoomViewAt(sf::Vector2i pixel, float new_zoom, sf::View& view)
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();

    const sf::Vector2f beforeCoord{window->mapPixelToCoords(pixel)};
    view.zoom(new_zoom);
    window->setView(view);
    sf::Vector2f afterCoord{window->mapPixelToCoords(pixel)};
    sf::Vector2f offsetCoords{beforeCoord - afterCoord};

    SPDLOG_DEBUG("zoomViewAt before: {} {}, after: {} {}", beforeCoord.x, beforeCoord.y, afterCoord.x, afterCoord.y);
    SPDLOG_DEBUG("zoomSpeed {} new_zoom {} zoomUntil {} dest_zoom {} activateZoom {} zoomedTotal {} ? {}", zoomSpeed, new_zoom, zoomUntil, dest_zoom, activateZoom, zoomedTotal, 330.0 / zoomedTotal);

    zoom_x += offsetCoords.x;
    zoom_y += offsetCoords.y;
}

void Camera::Work(sf::View& view, float dest_zoom_over)
{
    sf::RenderWindow* window = CoreManager::getInstance().getWindow();
    InputController* inputCtrl = CoreManager::getInstance().getInputController();
    float fps = CoreManager::getInstance().getCore()->getFPS();

    camera_y = static_cast<float>(window->getSize().y) / 2;

    dest_zoom = dest_zoom_over;
    manual_x_dest = 0;

    /** Mission controls **/

    if (!missionEnd && !lockMovement)
    {
        if (inputCtrl->isKeyHeld(Input::Keys::LTRIGGER))
        {
            manual_x_dest = -move_cam_x;
        }

        if (inputCtrl->isKeyHeld(Input::Keys::RTRIGGER))
        {
            manual_x_dest = move_cam_x;
        }
        
        if (inputCtrl->isKeyHeld(Input::Keys::LEFT))
        {
            dest_zoom = 1.002f;
        }

        if (inputCtrl->isKeyHeld(Input::Keys::RIGHT))
        {
            dest_zoom = 0.998f;
        }
    }

    if(activateZoom)
    {
        if(zoomSpeed > 1 && zoomedTotal <= zoomUntil)
        {
            if(strictZoom)
            {
                //SPDLOG_INFO("Zoom left: {}", abs(zoomUntil - zoomedTotal));
                dest_zoom = 1 + (abs(1-zoomSpeed)*abs(zoomUntil - zoomedTotal));
            }
            else
            {
                dest_zoom = zoomSpeed;
            }
        }

        if(zoomSpeed < 1 && zoomedTotal >= zoomUntil)
        {
            if(strictZoom)
            {
                //SPDLOG_INFO("Zoom left: {}", abs(zoomUntil - zoomedTotal));
                dest_zoom = 1 - (abs(1-zoomSpeed)*abs(zoomUntil - zoomedTotal));
            }
            else
            {
                dest_zoom = zoomSpeed;
            }
        }
    }

    if(zoomSpeed > 1 && zoomedTotal >= zoomUntil)
    {
        activateZoom = false;
    }
    if(zoomSpeed < 1 && zoomedTotal <= zoomUntil)
    {
        activateZoom = false;
    }

    /** Move camera **/

    //camera_x += camera_xspeed / fps;
    camera_x_dest = followobject_x + (1800 * CoreManager::getInstance().getCore()->resRatio);
    camera_x += ((camera_x_dest - camera_x)) / fps;
    //camera_x += ((camera_x_dest - camera_x) * 5) / fps;

    /** Manual camera movement (L/R in Patapon) **/
    manual_x += ((manual_x_dest - manual_x) * 5) / fps;


    /** Manual camera movement up/down **/
    manual_y += ((manual_y_dest - manual_y) * 5) / fps;

    /** Debug camera movement **/
    debug_x += ((debug_x_dest - debug_x) * 5) / fps;

    /** Apply zoom **/
    zoom += ((dest_zoom - zoom)) / fps;

    if (dest_zoom == 1)
    {
        float zoomLower, zoomUpper;

        if (fps >= 60)
        {
            zoomLower = 0.9999f + (0.01f / (fps));
            zoomUpper = 1.0001f - (0.01f / (fps));
        }
        else
        {
            zoomLower = 0.9999f;
            zoomUpper = 1.0001f;
        }

        if ((zoom > zoomLower) && (zoom < zoomUpper))
            zoom = 1;
    }

    if (dest_zoom > 1)
    {
        if (zoom >= dest_zoom)
            zoom = dest_zoom;
    } else if (dest_zoom < 1)
    {
        if (zoom <= dest_zoom)
            zoom = dest_zoom;
    }

    //SPDLOG_DEBUG("zoomSpeed {} zoom {} zoomUntil {} dest_zoom {} activateZoom {} zoomedTotal {} ? {}", zoomSpeed, zoom, zoomUntil, dest_zoom, activateZoom, zoomedTotal, 330.0/ zoomedTotal);

    //SPDLOG_DEBUG("[zoom] zoom: {} dest_zoom: {}", zoom, dest_zoom);
    zoomedTotal *= zoom;

    if(zoom != dest_zoom)
    zoomViewAt(sf::Vector2i(1920*static_cast<int>(CoreManager::getInstance().getCore()->resRatio), 2160*static_cast<int>(CoreManager::getInstance().getCore()->resRatio)), zoom, view);

    /** Apply camera position **/

    //sf::View view{window->getView()};
    view.setCenter(sf::Vector2f(camera_x + zoom_x + manual_x + debug_x - 300, camera_y + zoom_y + manual_y));
    window->setView(view);
}
