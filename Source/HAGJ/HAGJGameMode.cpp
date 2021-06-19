// Copyright Epic Games, Inc. All Rights Reserved.

#include "HAGJGameMode.h"
#include "HAGJPlayerController.h"
#include "HAGJCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHAGJGameMode::AHAGJGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AHAGJPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	// if (PlayerPawnBPClass.Class != nullptr)
	// {
	// 	DefaultPawnClass = PlayerPawnBPClass.Class;
	// }
}