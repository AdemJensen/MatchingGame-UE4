// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchingGamePlayerController.h"

AMatchingGamePlayerController::AMatchingGamePlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}
