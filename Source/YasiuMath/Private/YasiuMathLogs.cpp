#include "YasiuMathLogs.h"

#include "UObject/Object.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"


DEFINE_LOG_CATEGORY(YasiuMathPlugin);


FString ReadObName( const class UObject* ob )
{
    if ( ob ) {
        return ob->GetFName().ToString();
    }
    return "InvalidObject";
}


FString ReadObName( const AActor* act )
{
    if ( act ) {
        return act->GetActorNameOrLabel();
    }
    return "InvalidActor";
}

FString ReadObName( const UActorComponent* cmp )
{
    if ( cmp ) {
        if ( auto owner = cmp->GetOwner() ) {
            return ReadObName(owner);
        }

        return cmp->GetFName().ToString();
    }
    return "InvalidCmp";
}
