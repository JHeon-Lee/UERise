#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
//#include "MHMacros.generated.h"

#define PROPERTY_WITH_ACCESSORS_AND_DELEGATE(PropertyType, PropertyName)     \
private:                                                                    \
    PropertyType PropertyName;                                              \
public:                                                                     \
    UPROPERTY(BlueprintAssignable, Category = "Property")                  \
    FOn##PropertyName##Changed On##PropertyName##Changed;                   \
                                                                            \
public:                                                                     \
    UFUNCTION(BlueprintCallable, Category = "Property")                    \
    PropertyType Get##PropertyName() const                                 \
    {                                                                       \
        return PropertyName;                                                \
    }                                                                       \
                                                                            \
    UFUNCTION(BlueprintCallable, Category = "Property")                    \
    void Set##PropertyName(PropertyType NewValue)                          \
    {                                                                       \
        if (PropertyName != NewValue)                                       \
        {                                                                   \
            PropertyName = NewValue;                                        \
            On##PropertyName##Changed.Broadcast(NewValue);                  \
        }                                                                   \
    }

#define LOG_LOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetLocalRole()))
#define LOG_REMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetRemoteRole()))
#define LOG_SUBLOCALROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetLocalRole()))
#define LOG_SUBREMOTEROLEINFO *(UEnum::GetValueAsString(TEXT("Engine.ENetRole"), GetOwner()->GetRemoteRole()))

#define LOG_NETMODEINFO ((GetNetMode() == ENetMode::NM_Client) ? *FString::Printf(TEXT("CLIENT%d"), GPlayInEditorID) : ((GetNetMode() == ENetMode::NM_Standalone) ? TEXT("STANDALONE") : TEXT("SERVER"))) 
#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define MH_LOG(Format, ...) UE_LOG(LogTemp, Log, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_LOCALROLEINFO, LOG_REMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
#define MH_SUBLOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("[%s][%s/%s] %s %s"), LOG_NETMODEINFO, LOG_SUBLOCALROLEINFO, LOG_SUBREMOTEROLEINFO, LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))
