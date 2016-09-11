#include"pch.h"

#define DIRECTINPUT_VERSION 0x0800

#include<XInput.h>
#include<dinput.h>
#include"JBF/JBFramework.h"
#include"JBF/Debug/Debug.h"
#include"JBF/Core/Core.h"

#pragma comment(lib,"xinput9_1_0.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace JBF{
    namespace Core{
        namespace Input{
            // Inner variable(s) definition
            ///////////////////////////////////////////
            static bool ins_btnState[kNumDigitalInputs * 2];
            static bool* ins_btnOldState = &ins_btnState[0];
            static bool* ins_btnCurState = &ins_btnState[kNumDigitalInputs];

            static float ins_holdDuration[kNumDigitalInputs] = { 0.0f };
            static float ins_analogs[kNumAnalogInputs];
            static float ins_analogsTC[kNumAnalogInputs];

            static IDirectInput8* ins_device = nullptr;
            static IDirectInputDevice8* ins_keyboard = nullptr;
            static IDirectInputDevice8* ins_mouse = nullptr;

            static _DIMOUSESTATE2 ins_mouseState;
            static byte ins_keyBuffer[256];
            static byte ins_keyMap[kNumKeys]; // map DigitalInput enum to DX key codes 
            ///////////////////////////////////////////

            // Inner function(s) definition
            ///////////////////////////////////////////
            static INLINE float ins_filterAnalogInput(int val, int deadZone){
                return
                    val < 0 ?
                    (
                        val > -deadZone ?
                        0.f : (val + deadZone) / (32768.f - deadZone)
                        ) :
                    (
                        val < deadZone ?
                        0.f : (val - deadZone) / (32767.f - deadZone)
                        );
            }

            static void ins_keyMapping(){
                ins_keyMap[DK_ESC] = 1;
                ins_keyMap[DK_1] = 2;
                ins_keyMap[DK_2] = 3;
                ins_keyMap[DK_3] = 4;
                ins_keyMap[DK_4] = 5;
                ins_keyMap[DK_5] = 6;
                ins_keyMap[DK_6] = 7;
                ins_keyMap[DK_7] = 8;
                ins_keyMap[DK_8] = 9;
                ins_keyMap[DK_9] = 10;
                ins_keyMap[DK_0] = 11;
                ins_keyMap[DK_MINUS] = 12;
                ins_keyMap[DK_EQUALS] = 13;
                ins_keyMap[DK_BACK] = 14;
                ins_keyMap[DK_TAB] = 15;
                ins_keyMap[DK_Q] = 16;
                ins_keyMap[DK_W] = 17;
                ins_keyMap[DK_E] = 18;
                ins_keyMap[DK_R] = 19;
                ins_keyMap[DK_T] = 20;
                ins_keyMap[DK_Y] = 21;
                ins_keyMap[DK_U] = 22;
                ins_keyMap[DK_I] = 23;
                ins_keyMap[DK_O] = 24;
                ins_keyMap[DK_P] = 25;
                ins_keyMap[DK_LBRACKET] = 26;
                ins_keyMap[DK_RBRACKET] = 27;
                ins_keyMap[DK_RETURN] = 28;
                ins_keyMap[DK_LCTRL] = 29;
                ins_keyMap[DK_A] = 30;
                ins_keyMap[DK_S] = 31;
                ins_keyMap[DK_D] = 32;
                ins_keyMap[DK_F] = 33;
                ins_keyMap[DK_G] = 34;
                ins_keyMap[DK_H] = 35;
                ins_keyMap[DK_J] = 36;
                ins_keyMap[DK_K] = 37;
                ins_keyMap[DK_L] = 38;
                ins_keyMap[DK_SEMICOLON] = 39;
                ins_keyMap[DK_APOSTROPHE] = 40;
                ins_keyMap[DK_GRAVE] = 41;
                ins_keyMap[DK_LSHIFT] = 42;
                ins_keyMap[DK_BACKSLASH] = 43;
                ins_keyMap[DK_Z] = 44;
                ins_keyMap[DK_X] = 45;
                ins_keyMap[DK_C] = 46;
                ins_keyMap[DK_V] = 47;
                ins_keyMap[DK_B] = 48;
                ins_keyMap[DK_N] = 49;
                ins_keyMap[DK_M] = 50;
                ins_keyMap[DK_COMMA] = 51;
                ins_keyMap[DK_PERIOD] = 52;
                ins_keyMap[DK_SLASH] = 53;
                ins_keyMap[DK_RSHIFT] = 54;
                ins_keyMap[DK_MULTIPLY] = 55;
                ins_keyMap[DK_LALT] = 56;
                ins_keyMap[DK_SPACE] = 57;
                ins_keyMap[DK_CAPITAL] = 58;
                ins_keyMap[DK_F1] = 59;
                ins_keyMap[DK_F2] = 60;
                ins_keyMap[DK_F3] = 61;
                ins_keyMap[DK_F4] = 62;
                ins_keyMap[DK_F5] = 63;
                ins_keyMap[DK_F6] = 64;
                ins_keyMap[DK_F7] = 65;
                ins_keyMap[DK_F8] = 66;
                ins_keyMap[DK_F9] = 67;
                ins_keyMap[DK_F10] = 68;
                ins_keyMap[DK_NUMLOCK] = 69;
                ins_keyMap[DK_SCROLLLOCK] = 70;
                ins_keyMap[DK_NUM7] = 71;
                ins_keyMap[DK_NUM8] = 72;
                ins_keyMap[DK_NUM9] = 73;
                ins_keyMap[DK_SUBTRACT] = 74;
                ins_keyMap[DK_NUM4] = 75;
                ins_keyMap[DK_NUM5] = 76;
                ins_keyMap[DK_NUM6] = 77;
                ins_keyMap[DK_ADD] = 78;
                ins_keyMap[DK_NUM1] = 79;
                ins_keyMap[DK_NUM2] = 80;
                ins_keyMap[DK_NUM3] = 81;
                ins_keyMap[DK_NUM0] = 82;
                ins_keyMap[DK_DECIMAL] = 83;
                ins_keyMap[DK_F11] = 87;
                ins_keyMap[DK_F12] = 88;
                ins_keyMap[DK_NUMENTER] = 156;
                ins_keyMap[DK_RCTRL] = 157;
                ins_keyMap[DK_DIVIDE] = 181;
                ins_keyMap[DK_PRINTSCR] = 183;
                ins_keyMap[DK_RALT] = 184;
                ins_keyMap[DK_PAUSE] = 197;
                ins_keyMap[DK_HOME] = 199;
                ins_keyMap[DK_UP] = 200;
                ins_keyMap[DK_PAGEUP] = 201;
                ins_keyMap[DK_LEFT] = 203;
                ins_keyMap[DK_RIGHT] = 205;
                ins_keyMap[DK_END] = 207;
                ins_keyMap[DK_DOWN] = 208;
                ins_keyMap[DK_PAGEDOWN] = 209;
                ins_keyMap[DK_INSERT] = 210;
                ins_keyMap[DK_DELETE] = 211;
                ins_keyMap[DK_LWIN] = 219;
                ins_keyMap[DK_RWIN] = 220;
                ins_keyMap[DK_APPS] = 221;
            }
            static void ins_keyZerofill(){
                ZeroMemory(&ins_mouseState, sizeof ins_mouseState);
                ZeroMemory(ins_keyBuffer, sizeof ins_keyBuffer);
            }

            static void ins_keyInit(){
                ins_keyMapping();

                auto hWnd = JBF::GetHandle();

                if (FAILED(DirectInput8Create(
                    JBF::GetInstance(),
                    DIRECTINPUT_VERSION,
                    IID_IDirectInput8,
                    (void**)&ins_device,
                    nullptr
                    )))ASSERT(false, _T("Failed to create DirectInput8 device."));

                {
                    if (FAILED(ins_device->CreateDevice(
                        GUID_SysKeyboard,
                        &ins_keyboard,
                        nullptr
                        )))ASSERT(false, _T("Failed to create keyboard device."));

                    if (FAILED(ins_keyboard->SetDataFormat(
                        &c_dfDIKeyboard
                        )))ASSERT(false, _T("Keyboard SetDataFormat failed."));

                    if (FAILED(ins_keyboard->SetCooperativeLevel(
                        hWnd,
                        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
                        )))ASSERT(false, _T("Keyboard SetCooperativeLevel failed."));

                    DIPROPDWORD dipdw;
                    dipdw.diph.dwSize = sizeof DIPROPDWORD;
                    dipdw.diph.dwHeaderSize = sizeof DIPROPHEADER;
                    dipdw.diph.dwObj = 0;
                    dipdw.diph.dwHow = DIPH_DEVICE;
                    dipdw.dwData = 10;
                    if (FAILED(ins_keyboard->SetProperty(
                        DIPROP_BUFFERSIZE,
                        &dipdw.diph
                        )))ASSERT(false, _T("Failed to set keyboard buffer size."));
                }

                {
                    if (FAILED(ins_device->CreateDevice(
                        GUID_SysMouse,
                        &ins_mouse,
                        nullptr
                        )))ASSERT(false, _T("Failed to create mouse device."));

                    if (FAILED(ins_mouse->SetDataFormat(
                        &c_dfDIMouse2
                        )))ASSERT(false, _T("Mouse SetDataFormat failed."));

                    if (FAILED(ins_mouse->SetCooperativeLevel(
                        hWnd,
                        DISCL_FOREGROUND | DISCL_EXCLUSIVE
                        )))ASSERT(false, _T("Mouse SetCooperativeLevel failed."));
                }

                ins_keyZerofill();
            }
            static void ins_keyRelease(){
                if (ins_keyboard){
                    ins_keyboard->Unacquire();
                    RELEASE(ins_keyboard);
                }
                if (ins_mouse){
                    ins_mouse->Unacquire();
                    RELEASE(ins_mouse);
                }
                if (ins_device){
                    RELEASE(ins_device);
                }
            }
            static void ins_keyUpdate(){
                auto foreground = GetForegroundWindow();
                if (foreground != JBF::GetHandle() || IsWindowVisible(foreground) == FALSE){
                    ins_keyZerofill();
                }
                else{
                    ins_mouse->Acquire();
                    ins_mouse->GetDeviceState(sizeof ins_mouseState, &ins_mouseState);
                    ins_keyboard->Acquire();
                    ins_keyboard->GetDeviceState(sizeof ins_keyBuffer, ins_keyBuffer);
                }
            }
            ///////////////////////////////////////////

            void Init(){
                ZeroMemory(&ins_btnState, sizeof ins_btnState);
                ZeroMemory(&ins_analogs, sizeof ins_analogs);

                ins_keyInit();
            }
            void Release(){
                ins_keyRelease();
            }
            void Update(float delta){
                std::swap(ins_btnOldState, ins_btnCurState);

                XINPUT_STATE inpState;
                if (XInputGetState(0, &inpState) == ERROR_SUCCESS){
                    if (inpState.Gamepad.wButtons & (1 << 0))ins_btnCurState[DK_PAD_UP] = true;
                    if (inpState.Gamepad.wButtons & (1 << 1))ins_btnCurState[DK_PAD_DOWN] = true;
                    if (inpState.Gamepad.wButtons & (1 << 2))ins_btnCurState[DK_PAD_LEFT] = true;
                    if (inpState.Gamepad.wButtons & (1 << 3))ins_btnCurState[DK_PAD_RIGHT] = true;
                    if (inpState.Gamepad.wButtons & (1 << 4))ins_btnCurState[DK_PAD_START] = true;
                    if (inpState.Gamepad.wButtons & (1 << 5))ins_btnCurState[DK_PAD_BACK] = true;
                    if (inpState.Gamepad.wButtons & (1 << 6))ins_btnCurState[DK_PAD_LTHUMBCLICK] = true;
                    if (inpState.Gamepad.wButtons & (1 << 7))ins_btnCurState[DK_PAD_RTHUMBCLICK] = true;
                    if (inpState.Gamepad.wButtons & (1 << 8))ins_btnCurState[DK_PAD_LSHOULDER] = true;
                    if (inpState.Gamepad.wButtons & (1 << 9))ins_btnCurState[DK_PAD_RSHOULDER] = true;
                    if (inpState.Gamepad.wButtons & (1 << 12))ins_btnCurState[DK_PAD_A] = true;
                    if (inpState.Gamepad.wButtons & (1 << 13))ins_btnCurState[DK_PAD_B] = true;
                    if (inpState.Gamepad.wButtons & (1 << 14))ins_btnCurState[DK_PAD_X] = true;
                    if (inpState.Gamepad.wButtons & (1 << 15))ins_btnCurState[DK_PAD_Y] = true;

                    ins_analogs[AK_PAD_LTRIGGER] = inpState.Gamepad.bLeftTrigger / 255.0f;
                    ins_analogs[AK_PAD_RTRIGGER] = inpState.Gamepad.bRightTrigger / 255.0f;
                    ins_analogs[AK_PAD_LX] = ins_filterAnalogInput(inpState.Gamepad.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                    ins_analogs[AK_PAD_LY] = ins_filterAnalogInput(inpState.Gamepad.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
                    ins_analogs[AK_PAD_RX] = ins_filterAnalogInput(inpState.Gamepad.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                    ins_analogs[AK_PAD_RY] = ins_filterAnalogInput(inpState.Gamepad.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
                }
                else{
                    ZeroMemory(&ins_btnCurState[DK_PAD_UP], (DK_PAD_Y - DK_PAD_UP + 1) * sizeof(*ins_btnCurState));
                    ZeroMemory(&ins_analogs[AK_PAD_LTRIGGER], (AK_PAD_RY - AK_PAD_LTRIGGER + 1) * sizeof(*ins_analogs));
                }

                ins_keyUpdate();

                for (uint32_t i = 0; i < kNumKeys; ++i){
                    ins_btnCurState[i] = (ins_keyBuffer[ins_keyMap[i]] & 0x80) != 0;
                }
                for (uint32_t i = 0; i < _countof(ins_mouseState.rgbButtons); ++i){
                    if (ins_mouseState.rgbButtons[i] > 0)ins_btnCurState[DK_MUS_0 + i] = true;
                }

                ins_analogs[AK_MUS_X] = ins_mouseState.lX * 0.0018f;
                ins_analogs[AK_MUS_Y] = ins_mouseState.lY * -0.0018f;

                if (ins_mouseState.lZ > 0)ins_analogs[AK_MUS_SCROLL] = 1.f;
                else if (ins_mouseState.lZ < 0)ins_analogs[AK_MUS_SCROLL] = -1.f;

                // update time duration for buttons pressed
                for (uint32_t i = 0; i < kNumDigitalInputs; ++i){
                    if (ins_btnCurState[i]){
                        if (!ins_btnOldState[i])ins_holdDuration[i] = 0.0f;
                        else ins_holdDuration[i] += delta;
                    }
                }
                for (uint32_t i = 0; i < kNumAnalogInputs; ++i){
                    ins_analogsTC[i] = ins_analogs[i] * delta;
                }
            }

            INLINE bool KeyPressed(DigitalInput key){
                return ins_btnCurState[key] && !ins_btnOldState[key];
            }
            INLINE bool KeyReleased(DigitalInput key){
                return !ins_btnCurState[key] && ins_btnOldState[key];
            }
            INLINE bool KeyDown(DigitalInput key){
                return ins_btnCurState[key];
            }
            INLINE bool KeyUp(DigitalInput key){
                return !ins_btnCurState[key];
            }

            INLINE float GetDurationDown(DigitalInput key){
                return ins_holdDuration[key];
            }

            INLINE float GetAnalogInput(AnalogInput key){
                return ins_analogs[key];
            }
            INLINE float GetTimeCorrectedAnalogInput(AnalogInput key){
                return ins_analogsTC[key];
            }
        };
    };
};