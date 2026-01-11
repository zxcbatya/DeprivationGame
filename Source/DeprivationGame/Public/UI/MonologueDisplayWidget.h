#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/ITextAnimationHandler.h"
#include "MonologueDisplayWidget.generated.h"

UCLASS()
class DEPRIVATIONGAME_API UMonologueDisplayWidget : public UUserWidget, public ITextAnimationHandler
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// ITextAnimationHandler interface
	virtual void AnimateText_Implementation(const FString& Text) override;
	virtual void StopTextAnimation_Implementation() override;

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void SetMonologueText(const FString& Text);

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void HideWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MonologueText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float AnimationSpeed = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	float DisplayDuration = 10.0f;

private:
	FTimerHandle AnimationTimerHandle;
	FTimerHandle HideTimerHandle;
	FString TargetText;
	int32 CurrentCharIndex = 0;
};