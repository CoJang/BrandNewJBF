#pragma once

#include"JBF/Definitions.h"

namespace JBF{
    namespace Core{
        namespace Input{
            enum DigitalInput : unsigned char{
                // keyboard
                DK_ESC = 0,
                DK_1,
                DK_2,
                DK_3,
                DK_4,
                DK_5,
                DK_6,
                DK_7,
                DK_8,
                DK_9,
                DK_0,
                DK_MINUS,
                DK_EQUALS,
                DK_BACK,
                DK_TAB,
                DK_Q,
                DK_W,
                DK_E,
                DK_R,
                DK_T,
                DK_Y,
                DK_U,
                DK_I,
                DK_O,
                DK_P,
                DK_LBRACKET,
                DK_RBRACKET,
                DK_RETURN,
                DK_LCTRL,
                DK_A,
                DK_S,
                DK_D,
                DK_F,
                DK_G,
                DK_H,
                DK_J,
                DK_K,
                DK_L,
                DK_SEMICOLON,
                DK_APOSTROPHE,
                DK_GRAVE,
                DK_LSHIFT,
                DK_BACKSLASH,
                DK_Z,
                DK_X,
                DK_C,
                DK_V,
                DK_B,
                DK_N,
                DK_M,
                DK_COMMA,
                DK_PERIOD,
                DK_SLASH,
                DK_RSHIFT,
                DK_MULTIPLY,
                DK_LALT,
                DK_SPACE,
                DK_CAPITAL,
                DK_F1,
                DK_F2,
                DK_F3,
                DK_F4,
                DK_F5,
                DK_F6,
                DK_F7,
                DK_F8,
                DK_F9,
                DK_F10,
                DK_NUMLOCK,
                DK_SCROLLLOCK,
                DK_NUM7,
                DK_NUM8,
                DK_NUM9,
                DK_SUBTRACT,
                DK_NUM4,
                DK_NUM5,
                DK_NUM6,
                DK_ADD,
                DK_NUM1,
                DK_NUM2,
                DK_NUM3,
                DK_NUM0,
                DK_DECIMAL,
                DK_F11,
                DK_F12,
                DK_NUMENTER,
                DK_RCTRL,
                DK_DIVIDE,
                DK_PRINTSCR,
                DK_RALT,
                DK_PAUSE,
                DK_HOME,
                DK_UP,
                DK_PAGEUP,
                DK_LEFT,
                DK_RIGHT,
                DK_END,
                DK_DOWN,
                DK_PAGEDOWN,
                DK_INSERT,
                DK_DELETE,
                DK_LWIN,
                DK_RWIN,
                DK_APPS,

                kNumKeys,

                // gamepad
                DK_PAD_UP = kNumKeys,
                DK_PAD_DOWN,
                DK_PAD_LEFT,
                DK_PAD_RIGHT,
                DK_PAD_START,
                DK_PAD_BACK,
                DK_PAD_LTHUMBCLICK,
                DK_PAD_RTHUMBCLICK,
                DK_PAD_LSHOULDER,
                DK_PAD_RSHOULDER,
                DK_PAD_A,
                DK_PAD_B,
                DK_PAD_X,
                DK_PAD_Y,

                // mouse
                DK_MUS_0,
                DK_MUS_1,
                DK_MUS_2,
                DK_MUS_3,
                DK_MUS_4,
                DK_MUS_5,
                DK_MUS_6,
                DK_MUS_7,

                kNumDigitalInputs
            };
            enum AnalogInput : unsigned char{
                // gamepad
                AK_PAD_LTRIGGER,
                AK_PAD_RTRIGGER,
                AK_PAD_LX,
                AK_PAD_LY,
                AK_PAD_RX,
                AK_PAD_RY,

                // mouse
                AK_MUS_X,
                AK_MUS_Y,
                AK_MUS_SCROLL,

                kNumAnalogInputs
            };

            extern void Init();
            extern void Release();
            extern void Update(float delta);

            extern INLINE bool KeyPressed(DigitalInput key);
            extern INLINE bool KeyReleased(DigitalInput key);
            extern INLINE bool KeyDown(DigitalInput key);
            extern INLINE bool KeyUp(DigitalInput key);

            extern INLINE float GetDurationDown(DigitalInput key);

            extern INLINE float GetAnalogInput(AnalogInput key);
            extern INLINE float GetTimeCorrectedAnalogInput(AnalogInput key);
        };
    };
};