// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject6.h"
#include "MyProject6GameMode.h"
#include "MyProject6PlayerController.h"

AMyProject6GameMode::AMyProject6GameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = AMyProject6PlayerController::StaticClass();
}
