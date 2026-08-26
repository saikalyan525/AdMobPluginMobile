#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TimerManager.h"
#include "AdMobAdManager.generated.h"

class UAdMobRewardHandler;

/**
 * Central advertisement manager for AdSMS.
 *
 * Current test scope:
 * - Banner ads
 * - Interstitial ads
 * - Rewarded ads
 */
UCLASS(BlueprintType)
class ADMOBPLUGIN_API UAdMobManager : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads")
	void InitializeAds();

	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads")
	void ShutdownAds();

	// ============================================================
	// BANNER ADS
	// ============================================================

	/** Loads/displays the Banner test ad. */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Banner")
	void LoadBanner();

	/** Shows the loaded Banner ad. If it is not loaded, requests it. */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Banner")
	void ShowBanner();

	/** Hides the Banner without destroying the ad object. */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Banner")
	void HideBanner();

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Banner")
	bool IsBannerLoaded() const;

	// ============================================================
	// INTERSTITIAL ADS
	// ============================================================

	/** Requests the next Interstitial ad if one is not already loaded/requested. */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Interstitial")
	void LoadInterstitial();

	/**
	 * Button-friendly behavior:
	 * - If an Interstitial is ready: show it immediately.
	 * - If it is not ready: request it and wait until ready, then show it.
	 */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Interstitial")
	void ShowInterstitial();

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Interstitial")
	bool IsInterstitialReady() const;

	// ============================================================
	// REWARDED ADS
	// ============================================================

	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Rewarded")
	void LoadRewarded();

	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Rewarded")
	void ShowRewarded();

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Rewarded")
	bool IsRewardedReady() const;

	// ============================================================
	// TEST / DEBUG
	// ============================================================

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads")
	bool IsAdsInitialized() const;

	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads")
	void SetTestAdsEnabled(bool bEnabled);

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads")
	bool AreTestAdsEnabled() const;

	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Rewarded")
	UAdMobRewardHandler* GetRewardHandler() const;

private:

	/** Polls Android until an Interstitial becomes ready after a button press. */
	void CheckPendingInterstitialShow();

	void StartInterstitialReadyCheck();

	void StopInterstitialReadyCheck();

	/** Polls Android until the rewarded callback reports a reward. */
	void CheckRewardedCompletion();

	void StartRewardedCompletionCheck();

	void StopRewardedCompletionCheck();

	/** Polls Android after ShowRewarded() requests an ad that is not ready yet. */
	void CheckPendingRewardedShow();

	void StartRewardedReadyCheck();

	void StopRewardedReadyCheck();

protected:

	UPROPERTY()
	bool bAdsInitialized = false;

	UPROPERTY()
	bool bUseTestAds = true;

	UPROPERTY()
	bool bBannerLoaded = false;

	UPROPERTY()
	bool bBannerVisible = false;

	UPROPERTY()
	bool bInterstitialReady = false;

	UPROPERTY()
	bool bRewardedReady = false;

	/** True while ShowInterstitial is waiting for a newly requested ad. */
	UPROPERTY()
	bool bWaitingToShowInterstitial = false;

	/** Prevents duplicate polling timers. */
	FTimerHandle InterstitialReadyCheckTimerHandle;

	/** True while ShowRewarded is waiting for a newly requested ad. */
	UPROPERTY()
	bool bWaitingToShowRewarded = false;

	/** Prevents duplicate rewarded readiness polling timers. */
	FTimerHandle RewardedReadyCheckTimerHandle;

	/** Prevents duplicate rewarded completion polling timers. */
	FTimerHandle RewardedCompletionCheckTimerHandle;

	UPROPERTY()
	TObjectPtr<UAdMobRewardHandler> RewardHandler;
};