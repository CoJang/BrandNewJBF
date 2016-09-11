#pragma once

namespace JBF{
    namespace Base{
        class Stage{
        public:
            Stage();
            virtual ~Stage();

        public:
            virtual void Init();
            virtual void Cleanup();
        public:
            virtual void Update(float delta);
            virtual void Draw();

        public:
            virtual void Pause();
            virtual void Resume();
        };
    };
};