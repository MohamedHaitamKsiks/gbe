#include "EventManager.h"

#include <SDL3/SDL.h>

#include "frontend/rendering/Window.h"
#include "frontend/gui/GuiLayer.h"
#include "io/joypad/Joypad.h"
#include "util/Assert.h"

namespace GBE
{
    EventManager::EventManager(std::shared_ptr<Window> window, std::shared_ptr<Joypad> joypad, std::shared_ptr<GuiLayer> guiLayer):
        m_Window(window),
        m_Joypad(joypad),
        m_GuiLayer(guiLayer)
    {
        GBE_ASSERT(m_Window);
        GBE_ASSERT(m_Joypad);
        GBE_ASSERT(m_GuiLayer);
    }

    void EventManager::ProcessEvents()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // quit event
            if (event.type == SDL_EVENT_QUIT)
            {
                m_Window->Close();
                break;
            }

            // gui layer events
            if (m_GuiLayer->ProcessEvent(event))
                continue;

            // process joypad events
            if (event.type != SDL_EventType::SDL_EVENT_KEY_DOWN && event.type != SDL_EventType::SDL_EVENT_KEY_UP)
                continue;

            JoypadEvent joypadEvent{};
            joypadEvent.Pressed = event.key.down;
            bool doQueueEvent = true;

            switch (event.key.key)
            {
            case SDLK_LEFT:
                joypadEvent.Button = JoypadButton::LEFT;
                break;
            case SDLK_RIGHT:
                joypadEvent.Button = JoypadButton::RIGHT;
                break;
            case SDLK_UP:
                joypadEvent.Button = JoypadButton::UP;
                break;
            case SDLK_DOWN:
                joypadEvent.Button = JoypadButton::DOWN;
                break;
            case SDLK_S:
                joypadEvent.Button = JoypadButton::A;
                break;
            case SDLK_D:
                joypadEvent.Button = JoypadButton::B;
                break;
            case SDLK_SPACE:
                joypadEvent.Button = JoypadButton::SELECT;
                break;
            case SDLK_RETURN:
                joypadEvent.Button = JoypadButton::START;
                break;
            default:
                doQueueEvent = false;
                break;
            }

            if (doQueueEvent)
                m_Joypad->QueueJoypadEvent(joypadEvent);
        }
    }

} // namespace GBE
