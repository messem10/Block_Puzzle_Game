// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject6.h"
#include "MyProject6BlockGrid.h"
#include "MyProject6Block.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

/*
Description: The BlockGrid.cpp file for the lights off puzzle game in Unreal Engine 4.9
Name: Michael Messer
Date: September 13, 2015
Class: CSC-4360
*/

AMyProject6BlockGrid::AMyProject6BlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(-600.f,-300.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Turns: {0}"), FText::AsNumber(0)));
	ScoreText->SetXScale(1.5);
	ScoreText->SetYScale(1.5);
	ScoreText->AttachTo(DummyRoot);

	// Set defaults
	Size = 5;
	BlockSpacing = 300.f;
}


void AMyProject6BlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	const int32 numInRow = Size + 2;
	GridArr = new AMyProject6Block**[numInRow];

	for (int i = 0; i < numInRow; i++)
		GridArr[i] = new AMyProject6Block*[numInRow];

	for (int i = 0; i < numInRow; i++){
		for (int j = 0; j < numInRow; j++)
			GridArr[i][j] = NULL;
	}
	

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder
		
		/*
			20 21 22 23 24
			15 16 17 18 19
			10 11 12 13 14
			05 06 07 08 09
			00 01 02 03 04
		*/

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		// NOTE: This is lost in memory after the next call!!!
		AMyProject6Block* NewBlock = GetWorld()->SpawnActor<AMyProject6Block>(BlockLocation, FRotator(0,0,0));
		

		// Tell the block about its owner
		if(NewBlock != NULL)
		{
			NewBlock->OwningGrid = this;
			/* IMPORTANT */
			// Puts the pointer within the "frame" in the grid array. Allows us to access the block later on.
			GridArr[(BlockIndex/Size)+1][(BlockIndex%Size)+1] = NewBlock;
		}
	}
}


void AMyProject6BlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Turns: {0}"), FText::AsNumber(Score)));
}

#undef LOCTEXT_NAMESPACE
