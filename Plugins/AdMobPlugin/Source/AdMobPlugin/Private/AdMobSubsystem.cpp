#include "AdMobSubsystem.h"
#include "AdMobAdManager.h"

void UAdMobSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    if (!AdMobManager)
    {
        AdMobManager = NewObject<UAdMobManager>(this, UAdMobManager::StaticClass());
    }

    // No GameInstance Blueprint event is required.
    // The subsystem owns initialization automatically.
    InitializeAds();
}

void UAdMobSubsystem::Deinitialize()
{
    ShutdownAds();
    AdMobManager = nullptr;
    Super::Deinitialize();
}

void UAdMobSubsystem::InitializeAds()
{
    if (AdMobManager)
    {
        AdMobManager->InitializeAds();
    }
}

void UAdMobSubsystem::ShutdownAds()
{
    if (AdMobManager)
    {
        AdMobManager->ShutdownAds();
    }
}

void UAdMobSubsystem::LoadBanner()
{
    if (AdMobManager)
    {
        AdMobManager->LoadBanner();
    }
}

void UAdMobSubsystem::ShowBanner()
{
    if (AdMobManager)
    {
        AdMobManager->ShowBanner();
    }
}

void UAdMobSubsystem::HideBanner()
{
    if (AdMobManager)
    {
        AdMobManager->HideBanner();
    }
}

bool UAdMobSubsystem::IsBannerLoaded() const
{
    return AdMobManager ? AdMobManager->IsBannerLoaded() : false;
}

void UAdMobSubsystem::LoadInterstitial()
{
    if (AdMobManager)
    {
        AdMobManager->LoadInterstitial();
    }
}

void UAdMobSubsystem::ShowInterstitial()
{
    if (AdMobManager)
    {
        AdMobManager->ShowInterstitial();
    }
}

bool UAdMobSubsystem::IsInterstitialReady() const
{
    return AdMobManager ? AdMobManager->IsInterstitialReady() : false;
}

void UAdMobSubsystem::LoadRewarded()
{
    if (AdMobManager)
    {
        AdMobManager->LoadRewarded();
    }
}

void UAdMobSubsystem::ShowRewarded()
{
    if (AdMobManager)
    {
        AdMobManager->ShowRewarded();
    }
}

bool UAdMobSubsystem::IsRewardedReady() const
{
    return AdMobManager ? AdMobManager->IsRewardedReady() : false;
}

bool UAdMobSubsystem::IsAdsInitialized() const
{
    return AdMobManager ? AdMobManager->IsAdsInitialized() : false;
}
