#include "CHUD.h"
#include "Global.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

ACHUD::ACHUD()
{
	CHelpers::GetAsset<UTexture2D>(&Texture, "Texture2D'/Game/Textures/T_Crosshair.T_Crosshair'");



}

void ACHUD::DrawHUD()
{
	Super::DrawHUD();
	CheckFalse(bDraw);
	CheckNull(Texture);

	FVector2D center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	FVector2D size(Texture->GetSurfaceWidth() * 0.5f, Texture->GetSurfaceHeight() * 0.5f);
	FVector2D position = center - size;

	FCanvasTileItem item(position, Texture->Resource, FLinearColor::White);
	item.BlendMode = SE_BLEND_Translucent;

	Canvas->DrawItem(item);

}
