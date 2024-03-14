#include "TextStyleData.h"
#include "ProjectCalm/Utilities/LogMacros.h"


UTextStyleData::UTextStyleData(const FObjectInitializer &ObjectInitializer)
{
}

FFancyTextStyle UTextStyleData::GetStyleByString(FString StyleName)
{
    for (FFancyTextStyle Style : Styles) {if (Style.Name == StyleName) {return Style;}}

    return Styles[DefaultStyleIdx];
}

TSubclassOf<UFancyText> UTextStyleData::GetWidgetClassByString(FString StyleName)
{
    return GetStyleByString(StyleName).WidgetClass;
}

FFancyTextStyle UTextStyleData::GetDefaultStyle()
{
    if (DefaultStyleIdx < Styles.Num()) {return Styles[DefaultStyleIdx];}

    UE_LOG(LogAssetData, Warning, TEXT("TextStyleData:: No valid default style!"));
    return FFancyTextStyle();
}
