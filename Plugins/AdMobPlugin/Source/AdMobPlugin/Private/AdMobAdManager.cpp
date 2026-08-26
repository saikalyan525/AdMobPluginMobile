#include "AdMobAdManager.h"

#include "AdMobRewardHandler.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidJava.h"
#endif

DEFINE_LOG_CATEGORY_STATIC(LogAdManager, Log, All);

#if PLATFORM_ANDROID
namespace
{
	static jmethodID FindRewardedMethod(JNIEnv* Env, const char* Name, const char* Signature)
	{
		if (!Env)
		{
			UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] JNIEnv is NULL. Method=%hs Signature=%hs"), Name, Signature);
			return nullptr;
		}

		if (!FJavaWrapper::GameActivityClassID)
		{
			UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] GameActivityClassID is NULL. Method=%hs Signature=%hs"), Name, Signature);
			return nullptr;
		}

		if (!FJavaWrapper::GameActivityThis)
		{
			UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] GameActivityThis is NULL. Method=%hs Signature=%hs"), Name, Signature);
			return nullptr;
		}

		UE_LOG(LogAdManager, Log, TEXT("[AdSMS][JNI] Looking up GameActivity method: %hs %hs"), Name, Signature);

		jmethodID Method = FJavaWrapper::FindMethod(
			Env,
			FJavaWrapper::GameActivityClassID,
			Name,
			Signature,
			false
		);

		if (Method)
		{
			UE_LOG(LogAdManager, Log, TEXT("[AdSMS][JNI] SUCCESS: method found: %hs %hs"), Name, Signature);
		}
		else
		{
			UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] FAILED: method NOT found: %hs %hs"), Name, Signature);
		}

		return Method;
	}

	static void JavaCallVoid(const char* Name)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "()V");
			if (Method)
			{
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
			}
		}
	}

	static bool JavaCallBool(const char* Name)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "()Z");
			if (Method)
			{
				return FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method) != JNI_FALSE;
			}
		}
		return false;
	}

	static void JavaCallString(const char* Name, const FString& Value)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "(Ljava/lang/String;)V");
			if (Method)
			{
				FTCHARToUTF8 Utf8(*Value);
				jstring JavaValue = Env->NewStringUTF(Utf8.Get());
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, JavaValue);
				Env->DeleteLocalRef(JavaValue);
			}
		}
	}

	static void JavaCallStringBool(const char* Name, const FString& Value, bool BoolValue)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "(Ljava/lang/String;Z)V");
			if (Method)
			{
				FTCHARToUTF8 Utf8(*Value);
				jstring JavaValue = Env->NewStringUTF(Utf8.Get());
				FJavaWrapper::CallVoidMethod(
					Env,
					FJavaWrapper::GameActivityThis,
					Method,
					JavaValue,
					static_cast<jboolean>(BoolValue ? JNI_TRUE : JNI_FALSE)
				);
				Env->DeleteLocalRef(JavaValue);
			}
		}
	}

	static bool JavaRewardedBool(const char* Name)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "()Z");
			if (!Method)
			{
				UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] JavaRewardedBool failed: %hs"), Name);
				return false;
			}

			jboolean Result = FJavaWrapper::CallBooleanMethod(Env, FJavaWrapper::GameActivityThis, Method);
			UE_LOG(LogAdManager, Log, TEXT("[AdSMS][JNI] JavaRewardedBool %hs -> %s"), Name, Result ? TEXT("true") : TEXT("false"));
			return Result != JNI_FALSE;
		}
		return false;
	}

	static void JavaRewardedVoid(const char* Name)
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, Name, "()V");
			if (Method)
			{
				UE_LOG(LogAdManager, Log, TEXT("[AdSMS][JNI] Calling void method: %hs"), Name);
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
			}
			else
			{
				UE_LOG(LogAdManager, Error, TEXT("[AdSMS][JNI] JavaRewardedVoid failed: %hs"), Name);
			}
		}
	}

	static void JavaLoadRewarded()
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, "AndroidThunkJava_LoadRewardedAd", "(Ljava/lang/String;)V");
			if (Method)
			{
				// Google's official Android rewarded test ad unit.
				jstring AdUnitID = Env->NewStringUTF("ca-app-pub-3940256099942544/5224354917");
				FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, AdUnitID);
				Env->DeleteLocalRef(AdUnitID);
			}
		}
	}

	static int32 JavaGetRewardAmount()
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, "AndroidThunkJava_GetRewardedAmount", "()I");
			return Method ? FJavaWrapper::CallIntMethod(Env, FJavaWrapper::GameActivityThis, Method) : 0;
		}
		return 0;
	}

	static FString JavaGetRewardType()
	{
		if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
		{
			jmethodID Method = FindRewardedMethod(Env, "AndroidThunkJava_GetRewardedType", "()Ljava/lang/String;");
			if (Method)
			{
				jstring Result = (jstring)FJavaWrapper::CallObjectMethod(Env, FJavaWrapper::GameActivityThis, Method);
				return FJavaHelper::FStringFromLocalRef(Env, Result);
			}
		}
		return FString();
	}
}
#endif


void UAdMobManager::InitializeAds()
{
	if (bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("InitializeAds() called, but AdMob is already initialized."));
		return;
	}

	UE_LOG(LogAdManager, Log, TEXT("========================================"));
	UE_LOG(LogAdManager, Log, TEXT("Initializing AdMob system..."));

	// Development/test mode remains enabled.
	bUseTestAds = true;

	UE_LOG(
		LogAdManager,
		Log,
		TEXT("Test advertisements: %s"),
		bUseTestAds ? TEXT("ENABLED") : TEXT("DISABLED")
	);

	RewardHandler = NewObject<UAdMobRewardHandler>(this, UAdMobRewardHandler::StaticClass());

	if (RewardHandler)
	{
		UE_LOG(LogAdManager, Log, TEXT("AdRewardHandler created successfully."));
	}
	else
	{
		UE_LOG(LogAdManager, Error, TEXT("Failed to create AdRewardHandler."));
	}

	bAdsInitialized = true;

	UE_LOG(LogAdManager, Log, TEXT("AdMob Unreal-side manager initialized."));

	// Index 0 = Banner
	// Index 1 = Interstitial
	// Index 2 = Rewarded (reserved for the next stage)
	LoadInterstitial();
	LoadRewarded();

	UE_LOG(LogAdManager, Log, TEXT("Initial Interstitial and Rewarded loading requests submitted."));
	UE_LOG(LogAdManager, Log, TEXT("Banner is available on demand from the test widget."));
	UE_LOG(LogAdManager, Log, TEXT("========================================"));
}

void UAdMobManager::ShutdownAds()
{
	if (!bAdsInitialized)
	{
		return;
	}

	UE_LOG(LogAdManager, Log, TEXT("Shutting down advertisement system."));

	StopInterstitialReadyCheck();
	StopRewardedReadyCheck();
	StopRewardedCompletionCheck();
	bWaitingToShowRewarded = false;

#if PLATFORM_ANDROID
	JavaCallVoid("AndroidThunkJava_CloseAdBanner");
#endif

	bBannerLoaded = false;
	bBannerVisible = false;
	bInterstitialReady = false;
	bRewardedReady = false;
	bWaitingToShowInterstitial = false;

	RewardHandler = nullptr;
	bAdsInitialized = false;

	UE_LOG(LogAdManager, Log, TEXT("Advertisement system shut down."));
}

void UAdMobManager::LoadBanner()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("LoadBanner() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	UE_LOG(LogAdManager, Log, TEXT("Loading Banner through custom AdMob JNI."));
	JavaCallStringBool("AndroidThunkJava_ShowAdBanner", "ca-app-pub-3940256099942544/6300978111", true);
	bBannerLoaded = JavaCallBool("AndroidThunkJava_IsBannerAdAvailable");
	bBannerVisible = true;
#else
	UE_LOG(LogAdManager, Log, TEXT("Banner ads are Android-only. Windows request skipped."));
#endif
}

void UAdMobManager::ShowBanner()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("ShowBanner() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	UE_LOG(LogAdManager, Log, TEXT("Showing Banner through custom AdMob JNI."));
	JavaCallStringBool("AndroidThunkJava_ShowAdBanner", "ca-app-pub-3940256099942544/6300978111", true);
	bBannerLoaded = JavaCallBool("AndroidThunkJava_IsBannerAdAvailable");
	bBannerVisible = true;
#else
	UE_LOG(LogAdManager, Log, TEXT("Banner ads are Android-only. Windows show skipped."));
#endif
}

void UAdMobManager::HideBanner()
{
	if (!bAdsInitialized)
	{
		return;
	}

#if PLATFORM_ANDROID
	UE_LOG(LogAdManager, Log, TEXT("Hiding Banner through custom AdMob JNI."));
	JavaCallVoid("AndroidThunkJava_HideAdBanner");
	bBannerVisible = false;
#else
	UE_LOG(LogAdManager, Log, TEXT("Banner ads are Android-only. Windows hide skipped."));
#endif
}

bool UAdMobManager::IsBannerLoaded() const
{
#if PLATFORM_ANDROID
	return JavaCallBool("AndroidThunkJava_IsBannerAdAvailable");
#else
	return bBannerLoaded;
#endif
}

void UAdMobManager::LoadInterstitial()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("LoadInterstitial() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	if (JavaCallBool("AndroidThunkJava_IsInterstitialAdAvailable"))
	{
		bInterstitialReady = true;
		UE_LOG(LogAdManager, Log, TEXT("Interstitial is already READY."));
		return;
	}

	if (JavaCallBool("AndroidThunkJava_IsInterstitialAdRequested"))
	{
		UE_LOG(LogAdManager, Log, TEXT("Interstitial load is already in progress."));
		return;
	}

	UE_LOG(LogAdManager, Log, TEXT("Loading Interstitial through custom AdMob JNI."));
	JavaCallString("AndroidThunkJava_LoadInterstitialAd", "ca-app-pub-3940256099942544/1033173712");
	bInterstitialReady = false;
#else
	UE_LOG(LogAdManager, Log, TEXT("Interstitial ads are Android-only. Windows load skipped."));
#endif
}

void UAdMobManager::ShowInterstitial()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("ShowInterstitial() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	if (JavaCallBool("AndroidThunkJava_IsInterstitialAdAvailable"))
	{
		StopInterstitialReadyCheck();
		bWaitingToShowInterstitial = false;
		bInterstitialReady = true;

		UE_LOG(LogAdManager, Log, TEXT("Interstitial READY -> SHOWING NOW through custom JNI."));
		JavaCallVoid("AndroidThunkJava_ShowInterstitialAd");
		bInterstitialReady = false;
		return;
	}

	UE_LOG(LogAdManager, Log, TEXT("Interstitial is NOT ready. Requesting it and waiting..."));
	bWaitingToShowInterstitial = true;
	LoadInterstitial();
	StartInterstitialReadyCheck();
#else
	UE_LOG(LogAdManager, Log, TEXT("Interstitial ads are Android-only. Windows show skipped."));
#endif
}

bool UAdMobManager::IsInterstitialReady() const
{
#if PLATFORM_ANDROID
	return JavaCallBool("AndroidThunkJava_IsInterstitialAdAvailable");
#else
	return false;
#endif
}

void UAdMobManager::StartInterstitialReadyCheck()
{
	if (!bAdsInitialized || !bWaitingToShowInterstitial)
	{
		return;
	}

	if (!GEngine)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return;
	}

	if (World->GetTimerManager().IsTimerActive(InterstitialReadyCheckTimerHandle))
	{
		return;
	}

	World->GetTimerManager().SetTimer(
		InterstitialReadyCheckTimerHandle,
		this,
		&UAdMobManager::CheckPendingInterstitialShow,
		0.25f,
		true
	);
}

void UAdMobManager::StopInterstitialReadyCheck()
{
	if (!GEngine)
	{
		return;
	}

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (World)
	{
		World->GetTimerManager().ClearTimer(InterstitialReadyCheckTimerHandle);
	}
}

void UAdMobManager::CheckPendingInterstitialShow()
{
#if PLATFORM_ANDROID
	if (!bAdsInitialized || !bWaitingToShowInterstitial)
	{
		StopInterstitialReadyCheck();
		return;
	}

	if (!JavaCallBool("AndroidThunkJava_IsInterstitialAdAvailable"))
	{
		return;
	}

	StopInterstitialReadyCheck();
	bWaitingToShowInterstitial = false;
	bInterstitialReady = true;

	UE_LOG(LogAdManager, Log, TEXT("Interstitial became READY after button request. Showing now."));
	JavaCallVoid("AndroidThunkJava_ShowInterstitialAd");
	bInterstitialReady = false;
#endif
}

// ============================================================
// REWARDED
// ============================================================

void UAdMobManager::LoadRewarded()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("LoadRewarded() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	if (JavaRewardedBool("AndroidThunkJava_IsRewardedAdAvailable"))
	{
		bRewardedReady = true;
		UE_LOG(LogAdManager, Log, TEXT("Rewarded is already READY."));
		return;
	}

	if (JavaRewardedBool("AndroidThunkJava_IsRewardedAdRequested"))
	{
		UE_LOG(LogAdManager, Log, TEXT("Rewarded load is already in progress."));
		return;
	}

	UE_LOG(LogAdManager, Log, TEXT("Loading Rewarded Ad. Using Google test rewarded ID."));
	JavaLoadRewarded();
	bRewardedReady = false;
#else
	UE_LOG(LogAdManager, Log, TEXT("Rewarded ads are Android-only. Windows load skipped."));
#endif
}

void UAdMobManager::ShowRewarded()
{
	if (!bAdsInitialized)
	{
		UE_LOG(LogAdManager, Warning, TEXT("ShowRewarded() called before InitializeAds()."));
		return;
	}

#if PLATFORM_ANDROID
	if (!JavaRewardedBool("AndroidThunkJava_IsRewardedAdAvailable"))
	{
		UE_LOG(LogAdManager, Log, TEXT("Rewarded is not ready. Requesting it and waiting for READY."));
		bWaitingToShowRewarded = true;
		LoadRewarded();
		StartRewardedReadyCheck();
		return;
	}

	StopRewardedReadyCheck();
	bWaitingToShowRewarded = false;

	UE_LOG(LogAdManager, Log, TEXT("Showing Rewarded Ad."));
	JavaRewardedVoid("AndroidThunkJava_ClearRewardedEarned");
	JavaRewardedVoid("AndroidThunkJava_ShowRewardedAd");
	bRewardedReady = false;
	StartRewardedCompletionCheck();
#else
	UE_LOG(LogAdManager, Log, TEXT("Rewarded ads are Android-only. Windows show skipped."));
#endif
}

bool UAdMobManager::IsRewardedReady() const
{
#if PLATFORM_ANDROID
	return JavaRewardedBool("AndroidThunkJava_IsRewardedAdAvailable");
#else
	return false;
#endif
}

void UAdMobManager::CheckPendingRewardedShow()
{
#if PLATFORM_ANDROID
	if (!bAdsInitialized || !bWaitingToShowRewarded)
	{
		StopRewardedReadyCheck();
		return;
	}

	if (!JavaRewardedBool("AndroidThunkJava_IsRewardedAdAvailable"))
	{
		return;
	}

	StopRewardedReadyCheck();
	bWaitingToShowRewarded = false;
	bRewardedReady = true;

	UE_LOG(LogAdManager, Log, TEXT("Rewarded became READY after button request. Showing now."));
	JavaRewardedVoid("AndroidThunkJava_ClearRewardedEarned");
	JavaRewardedVoid("AndroidThunkJava_ShowRewardedAd");
	bRewardedReady = false;
	StartRewardedCompletionCheck();
#endif
}

void UAdMobManager::StartRewardedReadyCheck()
{
#if PLATFORM_ANDROID
	if (!GEngine) return;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!World) return;

	if (World->GetTimerManager().IsTimerActive(RewardedReadyCheckTimerHandle)) return;

	World->GetTimerManager().SetTimer(
		RewardedReadyCheckTimerHandle,
		this,
		&UAdMobManager::CheckPendingRewardedShow,
		0.25f,
		true
	);

	UE_LOG(LogAdManager, Log, TEXT("Rewarded readiness polling started (250 ms)."));
#endif
}

void UAdMobManager::StopRewardedReadyCheck()
{
#if PLATFORM_ANDROID
	if (!GEngine) return;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (World) World->GetTimerManager().ClearTimer(RewardedReadyCheckTimerHandle);
#endif
}

void UAdMobManager::CheckRewardedCompletion()
{
#if PLATFORM_ANDROID
	if (!bAdsInitialized)
	{
		StopRewardedCompletionCheck();
		return;
	}

	if (!JavaRewardedBool("AndroidThunkJava_IsRewardedAdEarned"))
	{
		if (!JavaRewardedBool("AndroidThunkJava_IsRewardedAdShowing"))
		{
			StopRewardedCompletionCheck();
		}

		return;
	}

	const int32 RewardAmount = JavaGetRewardAmount();
	const FString RewardType = JavaGetRewardType();

	StopRewardedCompletionCheck();

	if (RewardHandler && RewardAmount > 0)
	{
		UE_LOG(
			LogAdManager,
			Log,
			TEXT("Rewarded completion received. Granting reward: %d %s"),
			RewardAmount,
			*RewardType
		);

		RewardHandler->GiveReward(RewardAmount, RewardType);
	}
	else
	{
		UE_LOG(LogAdManager, Warning, TEXT("Rewarded completion received, but reward data is invalid."));
	}

	JavaRewardedVoid("AndroidThunkJava_ConsumeRewardedEarned");
#endif
}

void UAdMobManager::StartRewardedCompletionCheck()
{
#if PLATFORM_ANDROID
	if (!GEngine) return;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (!World) return;

	if (World->GetTimerManager().IsTimerActive(RewardedCompletionCheckTimerHandle)) return;

	World->GetTimerManager().SetTimer(
		RewardedCompletionCheckTimerHandle,
		this,
		&UAdMobManager::CheckRewardedCompletion,
		0.25f,
		true
	);

	UE_LOG(LogAdManager, Log, TEXT("Rewarded completion polling started (250 ms)."));
#endif
}

void UAdMobManager::StopRewardedCompletionCheck()
{
#if PLATFORM_ANDROID
	if (!GEngine) return;

	UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull);
	if (World) World->GetTimerManager().ClearTimer(RewardedCompletionCheckTimerHandle);
#endif
}

bool UAdMobManager::IsAdsInitialized() const
{
	return bAdsInitialized;
}

void UAdMobManager::SetTestAdsEnabled(bool bEnabled)
{
	bUseTestAds = bEnabled;

	UE_LOG(
		LogAdManager,
		Log,
		TEXT("Test advertisements: %s"),
		bUseTestAds ? TEXT("ENABLED") : TEXT("DISABLED")
	);
}

bool UAdMobManager::AreTestAdsEnabled() const
{
	return bUseTestAds;
}

UAdMobRewardHandler* UAdMobManager::GetRewardHandler() const
{
	return RewardHandler;
}