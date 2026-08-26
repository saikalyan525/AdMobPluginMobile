#include "AdMobBlueprintLibrary.h"
#include "AdMobSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

namespace AdMobBlueprintLibrary_Impl
{
    static UAdMobSubsystem* GetSubsystem(const UObject* WorldContextObject)
    {
        if (!WorldContextObject)
        {
            return nullptr;
        }

        UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
        return GameInstance ? GameInstance->GetSubsystem<UAdMobSubsystem>() : nullptr;
    }
}

void UAdMobBlueprintLibrary::InitializeAds(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->InitializeAds();
    }
}

void UAdMobBlueprintLibrary::ShutdownAds(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->ShutdownAds();
    }
}

void UAdMobBlueprintLibrary::LoadBanner(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->LoadBanner();
    }
}

void UAdMobBlueprintLibrary::ShowBanner(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->ShowBanner();
    }
}

void UAdMobBlueprintLibrary::HideBanner(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->HideBanner();
    }
}

bool UAdMobBlueprintLibrary::IsBannerLoaded(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        return S->IsBannerLoaded();
    }
    return false;
}

void UAdMobBlueprintLibrary::LoadInterstitial(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->LoadInterstitial();
    }
}

void UAdMobBlueprintLibrary::ShowInterstitial(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->ShowInterstitial();
    }
}

bool UAdMobBlueprintLibrary::IsInterstitialReady(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        return S->IsInterstitialReady();
    }
    return false;
}

void UAdMobBlueprintLibrary::LoadRewarded(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->LoadRewarded();
    }
}

void UAdMobBlueprintLibrary::ShowRewarded(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        S->ShowRewarded();
    }
}

bool UAdMobBlueprintLibrary::IsRewardedReady(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        return S->IsRewardedReady();
    }
    return false;
}

bool UAdMobBlueprintLibrary::IsAdsInitialized(const UObject* WorldContextObject)
{
    if (UAdMobSubsystem* S = AdMobBlueprintLibrary_Impl::GetSubsystem(WorldContextObject))
    {
        return S->IsAdsInitialized();
    }
    return false;
}
