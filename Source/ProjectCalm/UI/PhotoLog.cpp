#include "PhotoLog.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/UI/TextTreeNodeWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "Components/Image.h"
#include "Components/Button.h"

void UPhotoLog::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
    CHECK_NULLPTR_RET(PlayerCharacter, LogPlayerCharacter, "UPhotoLog:: Failed to get PlayerCharacter!");

    
}
