#pragma once
#include"ECS/Types.h"
namespace ECS {
    struct Camera {
        bool active{ true };
        EntityID followTarget{ NULL_ENTITY };
        float followSpeed{ 5.f };
        float zoom{ 1.f };
    };

}