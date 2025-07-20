## 1.4.3



## 1.4.2

- changed how avatar initialization works slightly, to account for more situations where players are loaded in during the game, including remote players
	- you should hopefully notice less occasions where players mistakenly don't have their custom model on.
- merged /avi-refresh and /avi-resend to speed up debugging; doing either one will now run both. (/avi-resend and /avi-rs are now deprecated, but still work)
- fixed known bug with setting your avatar with /avi and mod config; it should no longer mess up your avatar info, and in fact, if you were always using fully-qualified avatar IDs (:modname:aviname), you probably haven't run into it to begin with. Please let me know if anything still messes up with them now!

## 1.4.1

- fixes helmet showing up / avatar showing improperly in decoupled-camera photo mode
- possibly fixed avi sometimes not loading properly on initial world join? (can still fix with an /avi-reset chat command)
- changed long-form command names for consistency with short-form

# 1.4.0

- Changed command names to be less likely to conflict with other mods; see reference at bottom of readme
- Added extensively improved locker customization
  - float and color paramaters on the materials used by a custom avatar will be automatically read and displayed for customization by the player
  - mesh options (blendshapes) are also parsed
  - customization data is saved per avatar for each player locally, and synced with other players
- EXPERIMENTAL: avatar creators may now hide equipment that they deem incompatible with the avatar (cosmetic; may introduce a user-facing override later)

## Details for Creators
#### (This can all be found in the guide as well)
- material vector (color) + scalar (float/number) parameters are read, displayed, and saved, and can be restricted to a set / reordered using the optional name list in the avatar definition
  - if you don't specify anything in the whitelist, all parameters not inherited from vanilla materials will be shown, vectors first, and scalars after, ordered however unreal provides them.
  - If you do specify parameter names, make sure they match exactly. They will be displayed in the order given, regardless of type.
  - Vanilla parameters can technically be selected this way, but most vanilla parameters have ways of setting themselves that work outside the scope of this mod, so they may not work well. If you want to use colors from the default pioneer, simply set those on the pioneer and it'll carry over.
  - If a parameter on one material shares the same type and name as a parameter on another material, they will be modified in sync.
- blendshapes are read from the model (no filtering options) and displayed at the bottom below the material stuff in game
- blendshapes and displayed material scalars are all expected to have valid value ranges from 0-1
  - if you have a material parameter which does not fit this range, use the LinearInterpolation node in the material graph to fit it to the range. Only values within the range are selectable.
  - this should already be standard practice for blendshapes
- EXPERIMENTAL: you can select a set of equipment pieces to forcefully hide from view per avatar (in case they don't work with your mesh for whatever reason)

Outstanding Known Bugs:
- using the /avi command to set your avatar, or doing it from the config menu, instead of the locker, will do funky things to your customization info; may not change or sync properly for a lil while. Changing avatars with the locker a few times will fix it, but you might not retain customization settings. Try to only use the locker for now; the other methods are more for debugging anyways.
- I've found an occasional inconsistent bug where a client player may spawn in with the wrong avatar / a previously saved avatar on a server. Sometimes other occasional instances of the avatar not syncing correctly on initial join. Changing your avatar to something else and back should fix it, and it's very unlikely to happen unless you change avatars frequently.
- force hiding equipment works intermittently, and is currently considered at experimental support stage. Feel free to use it, it won't break anything, I just can't guarantee it will hide properly in all situations and for all clients at this point in time.

These will be addressed in later 1.4.x patches; they shouldn't be too bothersome for the majority of users, and I wanted to get this out now for people to enjoy 1.1

# 1.3.0

- Avatars can now include material overrides for 1p/3p meshes
  - This is QoL for model makers, allowing you to define different avatars using the same mesh(es) without having to copy the mesh completely just to change the assigned materials

- Fixed bug with first person default pioneer when you change the FOV
  - Custom avatars will still look weird if you change the FOV; technically possible to fix in theory if you know how to use panini maps

- Fixed the helmet appearing momentarily in the locker on custom avis with it disabled

# 1.2.0

- Integration with the Customization Locker

- Avatars can now include an item descriptor to customize how they show up in the locker
  - Existing models will not show up until they are updated.

# 1.1.0

- Initial Re-Release for Satisfactory 1.0

- Existing model mods should work with a simple re-compile
Note: Process for obtaining modding / mesh / armature etc. resources from the game may have changed slightly.
Existing models continue to work, however, textures or materials taken from base game will will need to be updated.

- added debug command to set offline player avatars
	- may or may not work as a non-server client, but it's just for debugging

- custom models should show up in hub locker customizer

- added more material templates, modified sampler type of some existing ones to fit the data type

- renamed /model/m command to /avatar or /avi (/model still works)

- Implemented custom avatars on the title screen because it's funny and cool (main pioneer is your selected mesh, others are random others you have)

- Added config option to set an avatar to render when the desired avatar cannot be found for whatever reason (usually a mistyped or outdated id, or an uninstalled mod)

- changed ID format to be `:ModRef:avatarname` (IDs starting with : will always be matched against a full ID)
	- any full-format avatar names will need to be set again in the config

- removed unused C++ module

- updated README guide with 1.0 process

### 1.0.1

- added template Materials for easy instancing
 - instance these in your avatar mods for convenience, and a small performance gain (might also possibly fix weird bugs of fresh materials not working)
- simplified default avatar name

# 1.0.0

- Initial Feature Release.
