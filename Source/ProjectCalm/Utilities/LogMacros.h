#pragma once

#define CHECK_NULLPTR(Pointer, LogCategory, ErrorMsg) {if (Pointer == nullptr){UE_LOG(LogCategory, Error, TEXT(ErrorMsg));}}
#define CHECK_NULLPTR_RET(Pointer, LogCategory, ErrorMsg) {CHECK_NULLPTR(Pointer, LogCategory, ErrorMsg); return;}
#define CHECK_NULLPTR_RETVAL(Pointer, LogCategory, ErrorMsg, RetVal) {CHECK_NULLPTR(Pointer, LogCategory, ErrorMsg); return RetVal;}