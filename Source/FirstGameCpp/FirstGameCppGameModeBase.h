// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "XP_Character.h"
#include "GameFramework/DefaultPawn.h"
#include "FirstGameCppGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FIRSTGAMECPP_API AFirstGameCppGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void InitGameState() override;

	UPROPERTY(EditAnywhere, NoClear)
	TSubclassOf<AXP_Character> CustomXPCharacterClass = AXP_Character::StaticClass();
	
};
