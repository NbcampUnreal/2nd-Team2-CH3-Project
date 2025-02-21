#include "InfectedCityGameInstance.h"
#include "Engine/TextureRenderTarget2D.h"

void UInfectedCityGameInstance::Init()
{
    Super::Init();

    DepthRenderTarget = NewObject<UTextureRenderTarget2D>(this);
    DepthRenderTarget->RenderTargetFormat = RTF_R32f; /* Dxó�� ���� �����ϴ°� */
    DepthRenderTarget->bAutoGenerateMips = false; /* Mipmap �Ұųİ� ����°��ϵ�*/
    DepthRenderTarget->InitAutoFormat(1024, 1024); /* size */
    DepthRenderTarget->UpdateResourceImmediate(); /* */

}

UTextureRenderTarget2D* UInfectedCityGameInstance::GetDepthRenderTarget()
{
    return DepthRenderTarget;
}
