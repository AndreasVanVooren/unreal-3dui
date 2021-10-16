// (c) 2021 Andreas Van Vooren

#pragma once

#include "UMG/Public/Components/ContentWidget.h"

#include "U3DElement.generated.h"

class S3DElement;

UCLASS()
class U3DElement : public UContentWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_DELEGATE_RetVal(FRotator, FGetRotator);
	DECLARE_DYNAMIC_DELEGATE_RetVal(FVector, FGetVector);

	UFUNCTION(BlueprintPure, BlueprintGetter)
		FRotator GetRotation() const;

	UFUNCTION(BlueprintSetter)
		void SetRotation(const FRotator& InRotation);

	UFUNCTION(BlueprintGetter)
		FVector GetOffset() const;

	UFUNCTION(BlueprintSetter)
		void SetOffset(const FVector& InOffset);

	UFUNCTION(BlueprintPure, BlueprintGetter)
		float GetDepth() const;

	UFUNCTION(BlueprintSetter)
		void SetDepth(float InDepth);

	UFUNCTION(BlueprintGetter)
		float GetFieldOfView() const;

	UFUNCTION(BlueprintSetter)
		void SetFieldOfView(float InFieldOfView);

	UFUNCTION(BlueprintGetter)
		bool IsActingAsRoot() const;

	UFUNCTION(BlueprintSetter)
		void SetActingAsRoot(bool bInActAsRoot);

protected:
	// The rotation of the widget, in 3D. This perpetuates across 3D elements.
	UPROPERTY(EditAnywhere, BlueprintGetter = GetRotation, BlueprintSetter = SetRotation, Category = "3D")
		FRotator Rotation;
	UPROPERTY()
		FGetRotator RotationDelegate;

	// The offset of the widget, in 3D. This perpetuates across 3D elements.
	UPROPERTY(EditAnywhere, BlueprintGetter = GetOffset, BlueprintSetter = SetOffset, Category = "3D")
		FVector Offset;
	UPROPERTY()
		FGetVector OffsetDelegate;

	// The simulated depth of the 3D element. 
	UPROPERTY(EditAnywhere, BlueprintGetter = GetDepth, BlueprintSetter = SetDepth, Category = "3D")
		float Depth;
	UPROPERTY()
		FGetFloat DepthDelegate;

	// The simulated field of view of the root 3D element.
	UPROPERTY(EditAnywhere, BlueprintGetter = GetFieldOfView, BlueprintSetter = SetFieldOfView, Category = "3D")
		float FieldOfView;
	UPROPERTY()
		FGetFloat FieldOfViewDelegate;

	// Whether the widget should act as a root.
	UPROPERTY(EditAnywhere, BlueprintGetter = IsActingAsRoot, BlueprintSetter = SetActingAsRoot, Category = "3D")
		bool bActAsRoot;

	PROPERTY_BINDING_IMPLEMENTATION(FRotator, Rotation);
	PROPERTY_BINDING_IMPLEMENTATION(FVector, Offset);
	PROPERTY_BINDING_IMPLEMENTATION(float, Depth);
	PROPERTY_BINDING_IMPLEMENTATION(float, FieldOfView);

public:
	//~ Begin UWidget Interface
	virtual void SynchronizeProperties() override;
	//~ End UWidget Interface

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

	// UPanelWidget
	virtual UClass* GetSlotClass() const override;
	virtual void OnSlotAdded(UPanelSlot* NewSlot) override;
	virtual void OnSlotRemoved(UPanelSlot* OldSlot) override;
	// End UPanelWidget

protected:
	TSharedPtr<S3DElement> My3DElement;
};