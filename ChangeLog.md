# 1.2.0

- Integration with the Customization Locker

- Avatars can now include an item descriptor to customize how they show up in the locker
  - Existing models will have a default icon for now until they are updated.

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
