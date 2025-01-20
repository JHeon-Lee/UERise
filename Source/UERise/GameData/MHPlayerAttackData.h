#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MHPlayerAttackData.generated.h"


USTRUCT(BlueprintType)
struct FMHPlayerAttackData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMHPlayerAttackData() :Description(TEXT("")), DamageMul(0.0f), PartAtkDamageMul(0.0f), HitStopTime(0.0f), CAMShakePower(1) {}

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
