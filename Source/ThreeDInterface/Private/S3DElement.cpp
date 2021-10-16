// (c) 2021 Andreas Van Vooren

#include "S3DElement.h"
#include "Core/Public/UObject/NameTypes.h"

namespace S3DElementInternals
{
	static const FName TypeName{ "S3DElement" };
	static FMatrix PerspectiveMatrix{FMatrix::Identity};
	static FTransform TotalTransform{FTransform::Identity};
}


S3DElement::S3DElement()
	: SCompoundWidget()
	, Rotation{}
	, Offset{}
	, Depth{}
	, FieldOfView{}
	, bActAsRoot{false}
{

}

S3DElement::~S3DElement()
{

}

void S3DElement::Construct(const FArguments& InArgs)
{
	Rotation = InArgs._Rotation;
	Offset = InArgs._Offset;
	Depth = InArgs._Depth;
	FieldOfView = InArgs._FieldOfView;
	bActAsRoot = InArgs._ActAsRoot;
	ChildSlot[InArgs._Content.Widget];
}

void S3DElement::OnArrangeChildren(
	const FGeometry& AllottedGeometry
	, FArrangedChildren& ArrangedChildren
) const {
	SCompoundWidget::OnArrangeChildren(AllottedGeometry, ArrangedChildren);
}

int32 S3DElement::OnPaint(
	const FPaintArgs& Args
	, const FGeometry& AllottedGeometry
	, const FSlateRect& MyCullingRect
	, FSlateWindowElementList& OutDrawElements
	, int32 LayerId
	, const FWidgetStyle& InWidgetStyle
	, bool bParentEnabled
) const {
	// TODO : do we actually want stuff to act as root independently from the root?
	bool bFoundParent = bActAsRoot;
	if (!bFoundParent)
	{
		for (TSharedPtr<SWidget> ptr = GetParentWidget(); ptr.IsValid(); ptr = ptr->GetParentWidget())
		{
			if (ptr->GetType() == S3DElementInternals::TypeName)
			{
				bFoundParent = true;
				break;
			}
		}
	}

	// If we need to act as a parent, we need an update to perspective.
	const bool bNeedsPerspectiveUpdate = bActAsRoot || !bFoundParent;

	const FMatrix PerspectiveMatrix = bNeedsPerspectiveUpdate 
									? FReversedZPerspectiveMatrix{
										FMath::DegreesToRadians(FieldOfView.Get()) * 0.5f
										, 1
										, 1
										, 0.1f}
									: S3DElementInternals::PerspectiveMatrix;
	
	const TGuardValue<FMatrix> PerspectiveGuard{ S3DElementInternals::PerspectiveMatrix, PerspectiveMatrix };

	const FVector FinalTranslation3D = FVector{0, 0, Depth.Get()} + Offset.Get();
	const FTransform FinalTransform = S3DElementInternals::TotalTransform * FTransform{ Rotation.Get(), FinalTranslation3D };
	const TGuardValue<FTransform> TransformGuard{S3DElementInternals::TotalTransform, FinalTransform};
	const FMatrix Final3DMatrix{ /*FinalTransform.ToMatrixWithScale() */ PerspectiveMatrix };
	// Change the matrix from a 3D matrix to a 2D matrix.
	// Given the following matrix calculation:
	// [X Y Z 1] [M00 M01 M02 M03] = [X']
	//           [M10 M11 M12 M13]   [Y']
	//           [M20 M21 M22 M23]   [Z']
	//           [M30 M31 M32 M33]   [W']
	// We now that X' = X*M00 + Y*M01 + Z*M02 + M03;
	//             Y' = X*M10 + Y*M11 + Z*M12 + M13; etc.
	// We only need X and Y, and Z is 0, or at least constant,
	// So we can just add Z*M02 + M03 to X and Z*M12 + M13 to Y
	const FMatrix2x2 Final2DMatrix{
		  Final3DMatrix.M[0][0]
		, Final3DMatrix.M[0][1]
		, Final3DMatrix.M[1][0]
		, Final3DMatrix.M[1][1]
	};

	constexpr const float ZValue = 0.0f;

	const FVector2D FinalTranslation2D{
		  ZValue*Final3DMatrix.M[0][2] + Final3DMatrix.M[0][3] 
		, ZValue*Final3DMatrix.M[1][2] + Final3DMatrix.M[1][3] 
	};
	//const FTransform2D SlateTransform{ Final2DMatrix, FinalTranslation2D };
	const FTransform2D SlateTransform{ Final2DMatrix, FinalTranslation2D };
	const FVector2D Pivot{ EForceInit::ForceInitToZero };
	FGeometry NewGeometry = AllottedGeometry.MakeChild(SlateTransform, Pivot);
	
	return SCompoundWidget::OnPaint(
		Args
		, NewGeometry
		, MyCullingRect
		, OutDrawElements
		, LayerId
		, InWidgetStyle
		, bParentEnabled
	);
}

void S3DElement::SetContent(TSharedRef<SWidget> InContent)
{
	ChildSlot[InContent];
}

FRotator S3DElement::GetRotation() const
{
	return Rotation.Get();
}

void S3DElement::SetRotation(const TAttribute<FRotator>& InRotation)
{
	SetAttribute(Rotation, InRotation, EInvalidateWidgetReason::Paint);
}

FVector S3DElement::GetOffset() const
{
	return Offset.Get();
}

void S3DElement::SetOffset(const TAttribute<FVector>& InOffset)
{
	SetAttribute(Offset, InOffset, EInvalidateWidgetReason::Paint);
}

float S3DElement::GetDepth() const
{
	return Depth.Get();
}

void S3DElement::SetDepth(const TAttribute<float>& InDepth)
{
	SetAttribute(Depth, InDepth, EInvalidateWidgetReason::Paint);
}

float S3DElement::GetFieldOfView() const
{
	return FieldOfView.Get();
}

void S3DElement::SetFieldOfView(const TAttribute<float>& InFieldOfView)
{
	SetAttribute(FieldOfView, InFieldOfView, EInvalidateWidgetReason::Paint);
}

bool S3DElement::IsActingAsRoot() const
{
	return bActAsRoot;
}

void S3DElement::SetActingAsRoot(bool bInActAsRoot)
{
	bActAsRoot = bInActAsRoot;
	Invalidate(EInvalidateWidgetReason::Layout);
}
