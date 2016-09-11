#pragma once

namespace JBF{
    namespace Global{
        namespace Math{
            template<typename T> struct Point{ T x, y; };
            template<typename T> struct Rect{ T left, top, right, bottom; };

            template<typename T> extern INLINE T BitRotateLeft(T value, T count){
                const T mask = (CHAR_BIT * sizeof(T) - 1);
                count &= mask;
                return (value << count) | (value >> ((-count) & mask));
            }
            template<typename T> extern INLINE T BitRotateRight(T value, T count){
                const T mask = (CHAR_BIT * sizeof(T) - 1);
                count &= mask;
                return (value >> count) | (value << ((-count) & mask));
            }
        };
    };
};