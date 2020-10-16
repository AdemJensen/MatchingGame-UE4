// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchingGameBlock.h"
#include "MatchingGameBlockGrid.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstance.h"

AMatchingGameBlock::AMatchingGameBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterial> BaseMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, BaseMaterial(TEXT("/Game/Puzzle/Meshes/BaseMaterial.BaseMaterial"))
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
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &AMatchingGameBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &AMatchingGameBlock::OnFingerPressedBlock);

	// Save a pointer to the orange material
	BaseMaterial = ConstructorStatics.BaseMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
    
    // Create static mesh component
    DisplayText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("DisplayText0"));
    DisplayText->SetRelativeLocation(FVector(110.f,50.f,60.f));
    DisplayText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
    DisplayText->SetRelativeScale3D(FVector(8.f,8.f,8.f));
    DisplayText->SetupAttachment(DummyRoot);
    DisplayText->SetVisibility(false);
}

void AMatchingGameBlock::SetDisplayNumber(int32 DisplayNumberTemp)
{
    DisplayNumber = DisplayNumberTemp;
    DisplayText->SetText(FText::AsNumber(DisplayNumber));
}


void AMatchingGameBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	HandleClicked();
}


void AMatchingGameBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	HandleClicked();
}

void AMatchingGameBlock::HandleClicked()
{
    RefreshDisplayStatus();
    if (OwningGrid != nullptr)
    {
        OwningGrid->DoBlockClickAction(GetIndexNumber());
    }
    /*/ Check we are not already active
    if (!bIsActive)
    {
        bIsActive = true;

        // Change material
        BlockMesh->SetMaterial(0, OrangeMaterial);

        // Tell the Grid
        if (OwningGrid != nullptr)
        {
            OwningGrid->AddScore();
        }
    }
    /*/
}

void AMatchingGameBlock::ToggleActive(bool bOn)
{
    bIsActive = bOn;
    if (bOn) {
        BlockMesh->SetMaterial(0, OrangeMaterial);
        DisplayText->SetVisibility(true);
    } else {
        BlockMesh->SetMaterial(0, BlueMaterial);
        DisplayText->SetVisibility(false);
    }
}

void AMatchingGameBlock::Highlight(bool bOn)
{
	// Do not highlight if the block has already been activated.
	if (bIsActive)
	{
		return;
	}

	if (bOn)
	{
		BlockMesh->SetMaterial(0, BaseMaterial);
	}
	else
	{
		BlockMesh->SetMaterial(0, BlueMaterial);
	}
}
