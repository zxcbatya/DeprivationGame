#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MonologueDisplayWidget.generated.h"

UCLASS()
class DEPRIVATIONGAME_API UMonologueDisplayWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void AnimateText(const FString& Text, const FString& CharacterName);

	UFUNCTION(BlueprintCallable, Category = "Monologue")  
	void SetMonologueText(const FString& Text, const FString& CharacterName);

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void HideWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CharacterNameText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MonologueText;
	
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float AnimationSpeed = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Display")
	float DisplayDuration = 10.0f;

private:
	FTimerHandle AnimationTimerHandle;
	FTimerHandle HideTimerHandle;
	FTimerHandle FadeInTimerHandle;
	FTimerHandle FadeOutTimerHandle;
	FString TargetText;
	int32 CurrentCharIndex = 0;
	
	void FadeInWidget();
	void FadeOutWidget();
};