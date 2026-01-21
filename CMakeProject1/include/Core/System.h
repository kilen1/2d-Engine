#pragma once

namespace CmakeProject1 {
    class System {
    public:
        virtual ~System() = default;
        virtual bool initialize() = 0;
        virtual void update() = 0;
        virtual void shutdown() = 0;
    };
}