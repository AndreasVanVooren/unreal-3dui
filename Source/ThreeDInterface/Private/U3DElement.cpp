// (c) 2021 Andreas Van Vooren

#include "U3DElement.h"
#include "S3DElement.h"
#include "U3DElementSlot.h"

FRotator U3DElement::GetRotation() const
{
	if (My3DElement.IsValid())
	{
		return My3DElement->GetRotation();
	}
	return Rotation;
}

void U3DElement::SetRotation(const FRotator& InRotation)
{
	Rotation = InRotation;
	RotationDelegate.Clear();
	if (My3DElement.IsValid())
	{
		My3DElement->SetRotation(InRotation);
	}
}

FVector U3DElement::GetOffset() const
{
	if (My3DElement.IsValid())
	{
		return My3DElement->GetOffset();
	}
	return Offset;
}

void U3DElement::SetOffset(const FVector& InOffset)
{
	Offset = InOffset;
	OffsetDelegate.Clear();
	if (My3DElement.IsValid())
	{
		My3DElement->SetOffset(InOffset);
	}
}

float U3DElement::GetDepth() const
{
	if (My3DElement.IsValid())
	{
		return My3DElement->GetDepth();
	}
	return Depth;
}

void U3DElement::SetDepth(float InDepth)
{
	Depth = InDepth;
	DepthDelegate.Clear();
	if (My3DElement.IsValid())
	{
		My3DElement->SetDepth(InDepth);
	}
}

float U3DElement::GetFieldOfView() const
{
	if (My3DElement.IsValid())
	{
		return My3DElement->GetFieldOfView();
	}
	return FieldOfView;
}

void U3DElement::SetFieldOfView(float InFieldOfView)
{
	FieldOfView = InFieldOfView;
	FieldOfViewDelegate.Clear();
	if (My3DElement.IsValid())
	{
		My3DElement->SetFieldOfView(InFieldOfView);
	}
}

bool U3DElement::IsActingAsRoot() const
{
	if (My3DElement.IsValid())
	{
		return My3DElement->IsActingAsRoot();
	}
	return bActAsRoot;
}

void U3DElement::SetActingAsRoot(bool bInActAsRoot)
{
	bActAsRoot = bInActAsRoot;
	if (My3DElement.IsValid())
	{
		My3DElement->SetActingAsRoot(bInActAsRoot);
	}
}

void U3DElement::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (My3DElement.IsValid())
	{
		My3DElement->SetRotation(PROPERTY_BINDING(FRotator, Rotation));
		My3DElement->SetOffset(PROPERTY_BINDING(FVector, Offset));
		My3DElement->SetDepth(PROPERTY_BINDING(float, Depth));
		My3DElement->SetFieldOfView(PROPERTY_BINDING(float, FieldOfView));
		My3DElement->SetActingAsRoot(bActAsRoot);
	}
}

void U3DElement::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);
	My3DElement.Reset();
}

TSharedRef<SWidget> U3DElement::RebuildWidget()
{
	My3DElement = SNew(S3DElement);

	U3DElementSlot* SlotAs3D = Cast<U3DElementSlot>(GetContentSlot());
	if (SlotAs3D && SlotAs3D->Content)
	{
		My3DElement->SetContent(SlotAs3D->Content->TakeWidget());
	}

	return My3DElement.ToSharedRef();
}

UClass* U3DElement::GetSlotClass() const
{
	return U3DElementSlot::StaticClass();
}

void U3DElement::OnSlotAdded(UPanelSlot* NewSlot)
{
	if (My3DElement.IsValid())
	{
		U3DElementSlot* SlotAs3D = Cast<U3DElementSlot>(NewSlot);
		if (SlotAs3D)
		{
			My3DElement->SetContent(SlotAs3D->Content ? SlotAs3D->Content->TakeWidget() : SNullWidget::NullWidget);
		}
	}
}

void U3DElement::OnSlotRemoved(UPanelSlot* OldSlot)
{
	if (My3DElement.IsValid())
	{
		My3DElement->SetContent(SNullWidget::NullWidget);
	}
}
