#pragma once

#include"JBF/Base/Base.h"

namespace JBF{
    namespace Manager{
        namespace Stage{
            extern void Init();
            extern void Release();

            extern INLINE void Update(float delta);
            extern INLINE void Draw();

            extern void ChangeStage(Base::Stage* stage);
            extern void PushStage(Base::Stage* stage);
            extern void PopStage();

            extern void Validate();
            extern void Invalidate();
        };
    };
};