#include "AdMobRewardHandler.h"

DEFINE_LOG_CATEGORY_STATIC(LogAdRewardHandler, Log, All);

void UAdMobRewardHandler::GiveReward(
	int32 RewardAmount,
	const FString& RewardType
)
{
	if (RewardAmount <= 0)
	{
		UE_LOG(
			LogAdRewardHandler,
			Warning,
			TEXT("Invalid reward amount: %d"),
			RewardAmount
		);

		return;
	}

	TotalRewardsGranted++;
	TotalRewardAmount += RewardAmount;

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("========================================")
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("REWARDED AD COMPLETED")
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("Reward Type: %s"),
		*RewardType
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("Reward Amount: %d"),
		RewardAmount
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("Total Rewards Granted: %d"),
		TotalRewardsGranted
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("Total Reward Amount: %d"),
		TotalRewardAmount
	);

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("========================================")
	);

	/*
	 * IMPORTANT:
	 *
	 * This is currently only the reward-system foundation.
	 *
	 * Later, your actual game system can be connected here.
	 *
	 * Example:
	 *
	 * PlayerInventory->AddCoins(RewardAmount);
	 *
	 * or:
	 *
	 * PlayerStats->AddXP(RewardAmount);
	 */
}

int32 UAdMobRewardHandler::GetTotalRewardsGranted() const
{
	return TotalRewardsGranted;
}

int32 UAdMobRewardHandler::GetTotalRewardAmount() const
{
	return TotalRewardAmount;
}

void UAdMobRewardHandler::ResetRewardStats()
{
	TotalRewardsGranted = 0;
	TotalRewardAmount = 0;

	UE_LOG(
		LogAdRewardHandler,
		Log,
		TEXT("Reward statistics reset.")
	);
}