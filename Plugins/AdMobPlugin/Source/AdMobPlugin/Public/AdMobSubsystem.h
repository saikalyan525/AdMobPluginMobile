#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AdMobSubsystem.generated.h"

class UAdMobManager;

UCLASS()
class ADMOBPLUGIN_API UAdMobSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    UFUNCTION(BlueprintCallable, Category="AdMob|Ads")
    void InitializeAds();

    UFUNCTION(BlueprintCallable, Category="AdMob|Ads")
    void ShutdownAds();

    UFUNCTION(BlueprintCallable, Category="AdMob|Banner")
    void LoadBanner();

    UFUNCTION(BlueprintCallable, Category="AdMob|Banner")
    void ShowBanner();

    UFUNCTION(BlueprintCallable, Category="AdMob|Banner")
    void HideBanner();

    UFUNCTION(BlueprintPure, Category="AdMob|Banner")
    bool IsBannerLoaded() const;

    UFUNCTION(BlueprintCallable, Category="AdMob|Interstitial")
    void LoadInterstitial();

    UFUNCTION(BlueprintCallable, Category="AdMob|Interstitial")
    void ShowInterstitial();

    UFUNCTION(BlueprintPure, Category="AdMob|Interstitial")
    bool IsInterstitialReady() const;

    UFUNCTION(BlueprintCallable, Category="AdMob|Rewarded")
    void LoadRewarded();

    UFUNCTION(BlueprintCallable, Category="AdMob|Rewarded")
    void ShowRewarded();

    UFUNCTION(BlueprintPure, Category="AdMob|Rewarded")
    bool IsRewardedReady() const;

    UFUNCTION(BlueprintPure, Category="AdMob|Ads")
    bool IsAdsInitialized() const;

private:
    UPROPERTY()
    TObjectPtr<UAdMobManager> AdMobManager;
};
