// Copyright Epic Games, Inc. All Rights Reserved.

#include "MatchingGameBlockGrid.h"
#include "MatchingGameBlock.h"
#include "Components/TextRenderComponent.h"
#include "Engine/World.h"
#include <random>

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

AMatchingGameBlockGrid::AMatchingGameBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 4;
	BlockSpacing = 300.f;
}


void AMatchingGameBlockGrid::BeginPlay()
{
	Super::BeginPlay();

}

void AMatchingGameBlockGrid::startNewGame()
{
    // Number of blocks
    const int32 NumBlocks = Size * Size;
    
    sons.Empty();
    Score = 0;
    ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
    
    std::random_device rd;
    std::default_random_engine gen = std::default_random_engine(rd());
    std::uniform_int_distribution<int> dis(1,NumBlocks / 2);
    
    int nums[NumBlocks / 2];
    for (int i = 0; i < NumBlocks / 2; i++) {
        nums[i] = 0;
    }

    // Loop to spawn each block
    for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
    {
        const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
        const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

        // Make position vector, offset from Grid location
        const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

        // Spawn a block
        AMatchingGameBlock* NewBlock = GetWorld()->SpawnActor<AMatchingGameBlock>(BlockLocation, FRotator(0,0,0));

        // Tell the block about its owner
        if (NewBlock != nullptr)
        {
            NewBlock->OwningGrid = this;
            NewBlock->SetIndexNumber(BlockIndex);
            sons.Add(NewBlock);
            NewBlock->SetActorRotation(FRotator(0.f,180.f,0.f));
            int num = dis(gen);
            while (nums[num - 1] > 1) {
                num = dis(gen);
            }
            nums[num - 1]++;
            NewBlock->SetDisplayNumber(num);
        }
    }
}


void AMatchingGameBlockGrid::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AMatchingGameBlockGrid::DoBlockClickAction(int32 Index) {
    if (!sons[Index]->IsEnabled()) return;
    if (LastIndex == -1) {
        sons[Index]->ToggleActive(true);
        sons[Index]->RefreshDisplayStatus();
        LastIndex = Index;
        return;
    }
    if (LastIndex == Index) {
        LastIndex = -1;
        sons[Index]->ToggleActive(false);
        sons[Index]->RefreshDisplayStatus();
        return;
    }
    sons[Index]->ToggleActive(true);
    sons[Index]->RefreshDisplayStatus();
    if (sons[LastIndex]->GetDisplayNumber() == sons[Index]->GetDisplayNumber()) {
        sons[LastIndex]->DisableBlock();
        sons[Index]->DisableBlock();
        if (GetWorld())
        {
            FTimerHandle Timer_Handle_LI, Timer_Handle_I;
            FTimerDelegate Delegate_LI, Delegate_I;
            Delegate_LI.BindUFunction(sons[LastIndex], FName("SelfDestruct"));
            Delegate_I.BindUFunction(sons[Index], FName("SelfDestruct"));
            GetWorld()->GetTimerManager().SetTimer(Timer_Handle_LI, Delegate_LI, 0.5, false);
            GetWorld()->GetTimerManager().SetTimer(Timer_Handle_I, Delegate_I, 0.5, false);
        }
        LastIndex = -1;
        AddScore();
        if (Score == Size * Size / 2) {
            if (GetWorld())
            {
                FTimerHandle Timer_Handle;
                FTimerDelegate Delegate;
                Delegate.BindUFunction(this, FName("GameEnd"));
                GetWorld()->GetTimerManager().SetTimer(Timer_Handle, Delegate, 0.5, false);
            }
        }
    } else {
        sons[LastIndex]->NeutralizeRefreshStatus();
        sons[Index]->NeutralizeRefreshStatus();
        if (GetWorld())
        {
            FTimerHandle Timer_Handle_LI, Timer_Handle_I;
            FTimerDelegate Delegate_LI, Delegate_I;
            Delegate_LI.BindUFunction(sons[LastIndex], FName("ConditionalDeactivate"));
            Delegate_I.BindUFunction(sons[Index], FName("ConditionalDeactivate"));
            GetWorld()->GetTimerManager().SetTimer(Timer_Handle_LI, Delegate_LI, 0.5, false);
            GetWorld()->GetTimerManager().SetTimer(Timer_Handle_I, Delegate_I, 0.5, false);
        }
        LastIndex = -1;
    }
}

#undef LOCTEXT_NAMESPACE
