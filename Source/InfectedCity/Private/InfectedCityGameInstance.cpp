#include "InfectedCityGameInstance.h"
#include "Engine/TextureRenderTarget2D.h"

void UInfectedCityGameInstance::Init()
{
    Super::Init();

    DepthRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    DepthRenderTarget->RenderTargetFormat = RTF_R32f; /* Dx처럼 포맷 설정하는거 */
    DepthRenderTarget->bAutoGenerateMips = false; /* Mipmap 할거냐고 물어보는거일듯*/
    DepthRenderTarget->InitAutoFormat(1024, 1024); /* size */
    DepthRenderTarget->UpdateResourceImmediate(); /* */

}

UTextureRenderTarget2D* UInfectedCityGameInstance::GetDepthRenderTarget()
{
    return DepthRenderTarget;
}
