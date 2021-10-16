// (c) 2021 Andreas Van Vooren

#pragma once

#include "UMG/Public/Components/PanelSlot.h"

#include "U3DElementSlot.generated.h"

UCLASS()
class U3DElementSlot : public UPanelSlot
{
	GENERATED_BODY()

public:

	// UPanelSlot interface
	virtual void SynchronizeProperties() override;
	// End of UPanelSlot interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
};