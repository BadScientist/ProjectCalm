#pragma once

#include "ComponentVisualizer.h"
#include "HitProxies.h"

struct HVisualizerProxy : public HComponentVisProxy
{
    DECLARE_HIT_PROXY();

    HVisualizerProxy(const UActorComponent* InComponent) 
        : HComponentVisProxy(InComponent, HPP_Wireframe){};

};