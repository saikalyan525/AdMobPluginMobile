// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdMobPluginSetupMSGameMode.h"
#include "AdMobPluginSetupMSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdMobPluginSetupMSGameMode::AAdMobPluginSetupMSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
