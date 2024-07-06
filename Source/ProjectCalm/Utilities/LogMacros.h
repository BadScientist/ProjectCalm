#pragma once

#define CHECK_NULLPTR(Pointer, LogCategory, ErrorMsg) {if (Pointer == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg));}}
#define CHECK_NULLPTR_RET(Pointer, LogCategory, ErrorMsg) {if (Pointer == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg)); return;}}
#define CHECK_NULLPTR_RETVAL(Pointer, LogCategory, ErrorMsg, RetVal) {if (Pointer == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg)); return RetVal;}}

#define CHECK_SOFTPTR(Pointer, LogCategory, ErrorMsg) {if (Pointer.Get() == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg));}}
#define CHECK_SOFTPTR_RET(Pointer, LogCategory, ErrorMsg) {if (Pointer.Get() == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg)); return;}}
#define CHECK_SOFTPTR_RETVAL(Pointer, LogCategory, ErrorMsg, RetVal) {if (Pointer.Get() == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg)); return RetVal;}}

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, All, All)
DECLARE_LOG_CATEGORY_EXTERN(LogUserWidget, All, All)
DECLARE_LOG_CATEGORY_EXTERN(LogEquipment, All, All)
DECLARE_LOG_CATEGORY_EXTERN(LogInteractable, All, All)
DECLARE_LOG_CATEGORY_EXTERN(LogQuest, All, All)
DECLARE_LOG_CATEGORY_EXTERN(LogItemData, All, All)