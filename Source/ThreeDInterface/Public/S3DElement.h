// (c) 2021 Andreas Van Vooren

#pragma once

#include "Core/Public/Containers/Array.h"
#include "SlateCore/Public/Widgets/DeclarativeSyntaxSupport.h"
#include "SlateCore/Public/Widgets/SCompoundWidget.h"

class THREEDINTERFACE_API S3DElement : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(S3DElement)
		: _Content()
		, _Rotation(FRotator::ZeroRotator)
		, _Depth(0.0f)
		, _FieldOfView(90.0f)
		, _ActAsRoot(false)
	{}
	SLATE_DEFAULT_SLOT(FArguments, Content)
	SLATE_ATTRIBUTE(FRotator, Rotation)
	SLATE_ATTRIBUTE(FVector, Offset)
	SLATE_ATTRIBUTE(float, Depth)
	SLATE_ATTRIBUTE(float, FieldOfView)
	SLATE_ARGUMENT(bool, ActAsRoot)
	SLATE_END_ARGS()

	S3DElement();
	virtual ~S3DElement();

	void Construct(const FArguments& InArgs);

	virtual void OnArrangeChildren(
		const FGeometry& AllottedGeometry
		, FArrangedChildren& ArrangedChildren
	) const override;

	virtual int32 OnPaint(
		const FPaintArgs& Args
		, const FGeometry& AllottedGeometry
		, const FSlateRect& MyCullingRect
		, FSlateWindowElementList& OutDrawElements
		, int32 LayerId
		, const FWidgetStyle& InWidgetStyle
		, bool bParentEnabled
	) const override;

	void SetContent(TSharedRef<SWidget> InContent);
	
	FRotator GetRotation() const;
	void SetRotation(const TAttribute<FRotator>& InRotation);

	FVector GetOffset() const;
	void SetOffset(const TAttribute<FVector>& InOffset);

	float GetDepth() const;
	void SetDepth(const TAttribute<float>& InDepth);

	float GetFieldOfView() const;
	void SetFieldOfView(const TAttribute<float>& InFieldOfView);

	bool IsActingAsRoot() const;
	void SetActingAsRoot(bool bInActAsRoot);

protected:
	// The rotation of the widget, in 3D. This perpetuates across 3D elements.
	TAttribute<FRotator> Rotation;
	// The offset of the widget, in 3D. This perpetuates across 3D elements.
	TAttribute<FVector> Offset;
	// The simulated depth of the root 3D element. 
	// This is only relevant for the most top-level 3D elements, or elements explicitly marked as root.
	TAttribute<float> Depth;
	// The simulated field of view of the root 3D element. 
	// This is only relevant for the most top-level 3D elements, or elements explicitly marked as root.
	TAttribute<float> FieldOfView;
	// Whether the widget should act as a root. 
	// 3D elements will use the root 3D element as reference to simulate a viewport.
	bool bActAsRoot;
};
