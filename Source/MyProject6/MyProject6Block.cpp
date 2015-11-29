// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MyProject6.h"
#include "MyProject6Block.h"
#include "MyProject6BlockGrid.h"
#include "EngineUtils.h"

/*
	Description: The Block.cpp file for the lights off puzzle game in Unreal Engine 4.9
	Name: Michael Messer
	Date: September 13, 2015
	Class: CSC-4360
*/

AMyProject6Block::AMyProject6Block()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));

	int colorType = rand() % 2 + 1;
	if (colorType == 1)
	{
		BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get()); // Default mesh color.
		bIsActive = false;
	}
	else{
		BlockMesh->SetMaterial(0, ConstructorStatics.OrangeMaterial.Get());
		bIsActive = true;
	}
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMyProject6Block::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMyProject6Block::OnFingerPressedBlock);

	// Save a pointer to the orange material
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	// Save a pointer to the blue material
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
}

void AMyProject6Block::BlockClicked(UPrimitiveComponent* ClickedComp)
{
	// If the player has already won, do not do anything except for the lower code.
	if (OwningGrid->GridArr[1][1]->wonCond == false)
	{
		int blockX = 0;
		int blockY = 0;
		// Current Block
		int length = 7;

		for (int i = 1; i < length - 1; i++)
		{
			for (int j = 1; j < length - 1; j++)
			{
				if (OwningGrid->GridArr[i][j] == this)
				{
					blockX = i;
					blockY = j;
					break;
				}
			}
		}

		toggleBlock(this); // Always toggle initial block.

		if (blockX != 1)
			toggleBlock(OwningGrid->GridArr[blockX - 1][blockY]); // LEFT
		if (blockX != length - 1)
			toggleBlock(OwningGrid->GridArr[blockX + 1][blockY]); // RIGHT
		if (blockY != 1)
			toggleBlock(OwningGrid->GridArr[blockX][blockY - 1]); // DOWN
		if (blockY != length - 1)
			toggleBlock(OwningGrid->GridArr[blockX][blockY + 1]); // UP

		// Tell the Grid
		if (OwningGrid != NULL)
			OwningGrid->AddScore();

		bool firstBlock = OwningGrid->GridArr[1][1]->bIsActive;
		bool lost = false;

		for (int i = 1; i < length - 1; i++)
		{
			for (int j = 1; j < length - 1; j++)
			{
				if (OwningGrid->GridArr[i][j]->bIsActive != firstBlock)
					lost = true;
			}
		}

		if (!lost)
			OwningGrid->GridArr[1][1]->wonCond = true;
	}

	if(OwningGrid->GridArr[1][1]->wonCond == true)
	{
		//Winner
		OwningGrid->GridArr[1][1]->wonCond = true; // If not already set to won, set to won.

		//Do something fun when clicked.
		for (int i = 1; i < 6; i++)
		{
			for (int j = 1; j < 6; j++)
			{
				int colorType = i % 2 + rand()%2;
				if (colorType == 1)
				{
					OwningGrid->GridArr[i][j]->BlockMesh->SetMaterial(0, BlueMaterial);
					bIsActive = false;
				}
				else{
					OwningGrid->GridArr[i][j]->BlockMesh->SetMaterial(0, OrangeMaterial);
					bIsActive = true;
				}
			}
		}

	}

}

void AMyProject6Block::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent);
}

void AMyProject6Block::toggleBlock(AMyProject6Block* toToggle)
{
	// Only change once per call
	bool toggled = false;

	// Saves conditional statements before calling
	if (toToggle != NULL) 
	{
		// Checks whether or not it is active
		if (toToggle->bIsActive == false && !toggled)
		{
			toToggle->bIsActive = true;
			toggled = true;

			// Change material
			toToggle->BlockMesh->SetMaterial(0, OrangeMaterial);
		}

		if (toToggle->bIsActive == true && !toggled)
		{
			toToggle->bIsActive = false;
			toggled = true;

			toToggle->BlockMesh->SetMaterial(0, BlueMaterial);
		}
	}
}
