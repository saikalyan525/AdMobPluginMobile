#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AdMobRewardHandler.generated.h"

/**
 * Handles rewards earned from rewarded advertisements.
 *
 * This class is intentionally separate from AdManager so that
 * advertisement display logic and game reward logic remain separate.
 */
UCLASS(BlueprintType)
class ADMOBPLUGIN_API UAdMobRewardHandler : public UObject
{
	GENERATED_BODY()

public:

	/**
	 * Called when a rewarded advertisement has been completed.
	 *
	 * @param RewardAmount Amount of reward earned.
	 * @param RewardType Type/name of the reward.
	 */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Rewarded")
	void GiveReward(int32 RewardAmount, const FString& RewardType);

	/**
	 * Returns the total number of rewards granted during this
	 * application session.
	 */
	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Rewarded")
	int32 GetTotalRewardsGranted() const;

	/**
	 * Returns the total reward amount granted during this
	 * application session.
	 */
	UFUNCTION(BlueprintPure, Category = "AdSMS|Ads|Rewarded")
	int32 GetTotalRewardAmount() const;

	/**
	 * Resets the session reward counters.
	 */
	UFUNCTION(BlueprintCallable, Category = "AdSMS|Ads|Rewarded")
	void ResetRewardStats();

protected:

	/** Number of rewarded ads that successfully granted a reward. */
	UPROPERTY()
	int32 TotalRewardsGranted = 0;

	/** Total reward value granted during this session. */
	UPROPERTY()
	int32 TotalRewardAmount = 0;
};