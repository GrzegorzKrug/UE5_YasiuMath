#pragma once


#define RunVerb Log
#define CompVerb Log


#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"


DECLARE_LOG_CATEGORY_EXTERN(YasiuMathPlugin, Log, Log);


FString ReadObName( const class UObject* ob );

FString ReadObName( const class AActor* act );

FString ReadObName( const class UActorComponent* cmp );

#if ENGINE_MAJOR_VERSION == 5  &&  ENGINE_MINOR_VERSION <= 1
#include "Logging/LogMacros.h"

template<typename T>
FString ToFString( const T& Val )
{
    if constexpr ( TIsPointer<T>::Value && TIsDerivedFrom<T, UObject>::Value ) {
        return Val ? Val->GetName() : TEXT("nullptr");
    }
    else if constexpr ( TIsEnum<T>::Value ) {
        return FString::Printf(TEXT("%d "), static_cast<int>(Val)); // cast enum to int
    }
    else {
        return FString::Printf(TEXT("%s "), *LexToString(Val));
    }
}template<typename ... Args>
FString BuildLogMessage( const FString& Msg, Args&& ... args )
{
    TArray<FString> Parts = {ToFString(args) ...};
    return Msg + TEXT(", Values: ") + FString::Join(Parts, TEXT(", "));
}

#define YASIUMATHLOG(LogVerb, LogMsg) \
do { \
UE_LOG(YasiuMathPlugin, LogVerb, TEXT(LogMsg)); \
} while(0)

#define YASIUMATHLOG_MULTI(LogVerb, LogMsg, ...) \
do { \
const FString FormattedMsg = FString::Format(TEXT("{0} {1}"), { ReadObName(this), LogMsg }); \
const FString MessageWithValues = BuildLogMessage(FormattedMsg, __VA_ARGS__);\
UE_LOG(YasiuMathPlugin, LogVerb, TEXT("%s"), *MessageWithValues); \
} while(0)
#else
#include "Logging/StructuredLog.h"

#define YASIUMATHLOG(LogVerb, LogMsg) \
UE_LOGFMT(YasiuMathPlugin, LogVerb, "{nm}({funcName}) " LogMsg, \
ReadObName(this), \
__FUNCTION__)

#define YASIUMATHLOG_MULTI(LogVerb, LogMsg, ...) \
UE_LOGFMT(YasiuMathPlugin, LogVerb, "{nm}({funcName}) " LogMsg, \
ReadObName(this), \
__FUNCTION__ \
, __VA_ARGS__)
#endif
