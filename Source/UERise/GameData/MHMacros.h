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