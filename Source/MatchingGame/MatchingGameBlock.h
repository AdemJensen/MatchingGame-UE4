// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchingGameBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class AMatchingGameBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;
    
    /** Text component for the number display */
    UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UTextRenderComponent* DisplayText;

public:
	AMatchingGameBlock();

	/** Are we currently active? */
	bool bIsActive;
    
    UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
    int32 DisplayNumber;

    UPROPERTY(Category = Data, EditAnywhere, BlueprintReadWrite)
    int32 IndexNumber;

    bool bEnabled = true;
    bool bRefreshed = false;

	/** Pointer to white material used on the focused block */
	UPROPERTY()
	class UMaterial* BaseMaterial;

	/** Pointer to blue material used on inactive blocks */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;

	/** Pointer to orange material used on active blocks */
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class AMatchingGameBlockGrid* OwningGrid;

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	void HandleClicked();
    
    void ToggleActive(bool bOn);

    UFUNCTION()
    void ConditionalDeactivate() { if (ShouldChangeDisplayStatus()) ToggleActive(false); }

    UFUNCTION()
    void SelfDestruct()
    {
        this->SetActorHiddenInGame(true);
        this->DisableBlock();
    }

	void Highlight(bool bOn);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
    
    /** Returns ScoreText subobject **/
    FORCEINLINE class UTextRenderComponent* GetDisplayText() const { return DisplayText; }

    /** Returns ScoreText subobject **/
    int32 GetDisplayNumber() const { return DisplayNumber; }

    /** Returns ScoreText subobject **/
    void SetDisplayNumber(int32 DisplayNumberTemp);

    /** Returns ScoreText subobject **/
    int32 GetIndexNumber() const { return IndexNumber; }

    /** Returns ScoreText subobject **/
    void SetIndexNumber(int32 IndexNumberTemp) { IndexNumber = IndexNumberTemp; }

    /** Returns ScoreText subobject **/
    bool IsEnabled() const { return bEnabled; }
    void EnableBlock() { bEnabled = true; }
    void DisableBlock() { bEnabled = false; }

    void RefreshDisplayStatus() { bRefreshed = true; }
    void NeutralizeRefreshStatus() { bRefreshed = false; }
    bool ShouldChangeDisplayStatus() const { return !bRefreshed; }
    
};



