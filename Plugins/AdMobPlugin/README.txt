# AdMobPlugin 2.0

Architecture:
- UAdMobSubsystem : UGameInstanceSubsystem
- UAdMobManager : UObject
- UAdMobBlueprintLibrary : direct Blueprint-callable wrapper
- Android implementation through AdMobPlugin_UPL.xml

There is intentionally NO AdMobActor class in this plugin.

Blueprint usage:
- Do not create BP_AdMobActor.
- Do not use Get Actor Of Class for AdMob.
- In a Widget, call:
  Show Banner
  Hide Banner
  Load Banner
  Show Interstitial
  Show Rewarded
  Is Ads Initialized

Initialization is automatic when the GameInstanceSubsystem is created.
The Blueprint Initialize Ads node is retained only as an optional explicit/retry call.

Install:
1. Close Unreal Editor.
2. Delete the old AdMobPlugin folder.
3. Copy this AdMobPlugin folder into Project/Plugins/.
4. Regenerate project files.
5. Delete Project/Intermediate and Project/Binaries.
6. Open the project and compile.
7. Delete BP_AdMobActor from Content if it exists.
8. Remove any old Ad Mob Manager / AdMobActor nodes from test Blueprints.
9. Recreate the Widget nodes by searching for "AdMob".
