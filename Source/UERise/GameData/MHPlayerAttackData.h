#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MHPlayerAttackData.generated.h"


USTRUCT(BlueprintType)
struct FMHPlayerAttackData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PartAtkDamageMul;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitStopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CAMShakePower;
};
