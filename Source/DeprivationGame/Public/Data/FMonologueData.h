#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FMonologueData.generated.h"

USTRUCT(BlueprintType)
struct FMonologueData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	FName MonologueID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	FString AudioPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	TArray<FString> Subtitles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	float DurationPerLine = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monologue")
	float TotalDisplayTime = 10.0f;
};
