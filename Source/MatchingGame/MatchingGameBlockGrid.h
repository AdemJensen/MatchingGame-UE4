// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchingGameBlock.h"
#include "MatchingGameBlockGrid.generated.h"

/** Class used to spawn blocks and manage score */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverAction);
UCLASS(minimalapi)
class AMatchingGameBlockGrid : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

public:
	AMatchingGameBlockGrid();

	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;
    
    UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
    TArray<AMatchingGameBlock*> sons;

    int32 LastIndex = -1;

    UPROPERTY(Category=Actions, BlueprintAssignable)
    FGameOverAction Action;

    UFUNCTION()
    void GameEnd() { Action.Broadcast(); }

protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:
    UFUNCTION(BlueprintCallable)
    void startNewGame();

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }
    
    void DoBlockClickAction(int32 Index);
    
};



