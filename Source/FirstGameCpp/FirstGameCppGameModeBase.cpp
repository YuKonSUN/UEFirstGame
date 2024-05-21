// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstGameCppGameModeBase.h"

void AFirstGameCppGameModeBase::InitGameState()
{
	Super::InitGameState();

	if (DefaultPawnClass == ADefaultPawn::StaticClass() || !DefaultPawnClass)
	{
		DefaultPawnClass = CustomXPCharacterClass;
	}
}
