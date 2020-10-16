// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchingGameGameMode.h"
#include "MatchingGamePlayerController.h"
#include "MatchingGamePawn.h"

AMatchingGameGameMode::AMatchingGameGameMode()
{
	// no pawn by default
	DefaultPawnClass = AMatchingGamePawn::StaticClass();
	// use our own player controller class
	PlayerControllerClass = AMatchingGamePlayerController::StaticClass();
}
