# Character Replacer

## Install

View on the modding site: (link)

## Overview

This mod is a library mod to make it easier to use custom avatars with your friends (or alone) in Satisfactory.

No custom avatars are included when you install this mod alone. It is designed to handle the behavior needed to manage syncing, selecting, and replacing player meshes which have been set up and registered using your own mod(s).

## How to Make Compatible Avatar Mods

I've created the mod as a beginner to both unreal and satisfactory modding, but I've tried to have it take care of as much of the process as possible, which still encompassed a good few weeks of debugging and programming... but if you want to add your own models, you _will_ have to nonetheless get your hands a little dirty setting up the all the raw assets in unreal, and then doing a quick attachment to my mod, before packaging yours.

The API to register your own avatar(s) is fairly simple, consisting of four main steps, only two of which I'll cover here:
1. **Prereq:** You have your avatar re-rigged (as with player mesh replacement in most games)
1. **Prereq:** You've gone through the modding env set up as described on the satisfactory modding wiki/docs
1. You import your model(s) into unreal
1. You set up a quick interface with CharacterReplacer to register your avatar(s).

Re-rigging a model can be complex, and obtaining the armature to rig to can also take time; but it shouldn't be anything unusual to someone who has replaced rigged models in other games before. There are many guides out there and many communities which will help. **However**, it is worth note that you will need to make **two separate models: for first and third person**, and in a couple places their bone structure *does vary slightly*, so make sure you pay attention and get both versions of the rig for reference.

Throughout this doc I'll be short-handing "first person" to "1p" and "third person" to "3p". Just for reference.

The Satisfactory modding community will also gladly help with any problems you may have setting up the modding environment. It is a somewhat long process, but it is unfortunately required, as there is no way to add an fbx or other model file to the game without running it through the full unreal packing process in the specific custom engine version used by the satisfactory devs and modified further by the modding team. The modding team are working on tools to automate the process for you as much as possible, though. So that's very nice of them. :)

## Tips On the Re-Rig process

You will need:
- FModel
- Blender 4.0
- The psk importer plugin for blender found here: https://github.com/DarklightGames/io_scene_psk_psa

### FModel
We will start with the FModel process. (skipping over a bit for now, other modding communities like palworld can teach you how to set up fmodel)
- Ensure fmodel's browse folder is set to the `FactoryGame/Content/Paks` folder inside your Satisfactory install folder.
- Set the engine version to Unreal 5.2 (GAME_UE5_2)
- If you wish (allows you to preview assets), you may enable "Local Mapping File" and put the path to the mapping file kindly provided by the Coffee Stain devs, inside your install folder: `.../CommunityResources/FactoryGame.usmap`
- Set the output directory to somewhere easy of your choice, or leave it as default.

The Character Meshes can be found in `FactoryGame/Content/FactoryGame/Character/Player/Mesh/`. You'll want to right-click and select "Save Model" for:
- `Character.uasset` (the third person / 3p model)
- `1PCharacter.uasset` (the first person / 1p model)

If you want to have the game textures as well, for reference, you can find them in the `Textures` folder next to the `Mesh` folder. Be sure to select "Save Textures" instead.

### Blender
From there, you should be able to open blender, and install the plugin. If it doesn't install correctly, use a legacy steam branch or other means to get specifically blender 4.0, which should work. Then you can just import the PSK(s).

A quick note about the model armatures, that you'll have to modify before export:
- The models will import at 100x scale. Just a funny unreal thing. You can leave it as is, but when you export to fbx, export at 0.01 scale, _or_, in unreal, import at 0.01 scale. Otherwise you'll find, like I did, that when you try to drop items in game, they drop 200 feet in the air, and when you die (or get hit), your ragdoll spazzes into the next dimension.

3. When you assign the skeleton later in unreal, the bones **won't match unless you do a few things first.** For **both the 1p and 3p models:**
   1. Go into armature edit mode, and delete the root bone.
   2. Rename the armature object (the green object under "Pose") to `Root` (capital R).
   3. Rename the parent armature object (the top orange one) to `Root` as well.

You should then be good to export it as an fbx. Disable "Add leaf bones" and set the scale to 0.01.
I'd recommend exporting the basic model as is, as a test, and make sure you can get this new "custom" avatar to show up and animate in game, before working on re-rigging your custom model to this rig.

Some final tips when you're doing your re-rigging:
- Keep the head on the 3p model, and remove it on the 1p model. Unless you want to try and use the base-game helmet. Then remove it on both.
- The 1p model is structured a fair bit different, as the arms are severed from the body bone-wise. For, you know, first-person reasons.
- For the 3p model, the head *must* be all weighted to a single head bone. Like, you *can* keep other bones on it, but... don't expect the physics engine to like you. It might be possible to find a way to make said bones work well, but I haven't figured it out. Let me know on the discord if you figure something out!
- If you can export it with materials, feel free; it will save time on set up in unreal, allowing you to just import the materials instead of recreating them.

Note: Getting the model configured at this stage (basically getting a correctly-configured fbx file) is by far the most touchy, annoying, buggy step. If something doesn't work, it was probably because of this. Even if you follow everything, it might need some blood sacrifices. And tinkering. And comparing against the main model and skeleton. Good luck!

## After Completing the PreReqs

With all the above said and done, once you have your FBXs, and your environment is set up, you can make a new, simple blueprint mod, and get started on the couple steps you need. Everything below will be done inside the `Content` folder of a new blueprint mod you've made via the Alpakit wizard. Remember, refer to the Modding docs or discord for how all that stuff works.

Note: **Always remember to save your assets!!!** Unreal does not automatically save changes to assets like imported models, textures, materials, or blueprints or anything else.
You *must* do this yourself, and if you forget, and then pack your mod, it *will* behave strangely and you *will* be confused. And I _will_ be sad. Go to `File -> Save All` for easy saving.

### Importing your Model into Unreal

You're going to want to do the following for both your first person and third person models (although ideally they can share the same textures).
Side note, as you create your assets, if you are adding more than one avatar, I'd recommend using subfolders for each one as you drag stuff in; it will get messy otherwise. :)

1. Drag your fbx into the editor, and you'll be greeted with the import dialog. Ensure the following non-default options are changed:
   1. Set "Use T0 As Ref Pose"
   2. Look under the "Material" Section lower down; assuming your 1p model materials are a subset of the 3p model materials, you'll want to import the 3p model with the "Material Import Method" as "Create New Materials" and then for the 1p, select "Do not create materials" and don't import the textures. Then you can edit the 1p mesh and drag the materials in. Or you can just... Import it for both, and then delete one... or do whatever... I won't stop you.
   3. Hit "Import All".
1. Unreal will create a Skeletal Mesh, a Skeleton, a PhysicsAsset, and material slots according to your avatar, if you opted to create them. The skeleton and physics asset won't be used, but having them generated is important.
   - If your model didn't come with the materials attached, you'll have to recreate them and import the textures. You can configure the materials as you wish in a fairly intuitive manner (it's a shader graph like blender, but different). I won't be covering how to do that here.
1. Right Click the skeletal mesh (NOT the skeleton), and click "Assign Skeleton"
1. For the 1p model, look for `1PCharacter_Skeleton`; for the 3p model, look for `Character_Skeleton`. Assign this to your skeletal mesh.
   - You may want to watch out for any errors in this step; it may lead you to realize if any of your bones are missing. It won't show _every_ possible bone problem though.

### Registering your Model as an in-game Avatar

Once you've imported your model, assigned the skeleton, and recreated the textures, we can get on to the CharacterReplacer-specific stuff that does the magic.

1. Download/Clone this repository source as a mod next to your new mod, and restart the editor to load the classes and data.
1. For each Avatar you want to add...
   1. Create a new `Misc/Data Asset` in your Content folder, and parent it to the `AvatarDefinition` type. Name it after your model (doesn't particularly matter).
   1. Open the asset, and fill in the fields; drag in or select your skeletal mesh asset for first and third person, and select "keep helmet" only if you want the base pioneer's head to be rendered in addition to your mesh... for whatever reason. Maybe you like how it looks, idk. The head is a separate, static mesh in the game, normally, but you can ignore that and keep your head attached to your 3p model, if it doesn't fit in the helmet. Your head should still be all one bone, though.
1. Create a new blueprint class in your Content folder and select it to be a `GameInstanceModule`; make sure when you open it to mark it as the root module, at the bottom.
1. Open the full blueprint editor for your new blueprint class.
1. Create a new variable called "AvatarDescriptors" or something like that, and type it as a map of `Name` -> `AvatarDefinition`.
1. Add an entry for every `AvatarDefinition` you set up, and choose an ID for the key for each avatar. This will be how you identify each avatar in game... prefixed by your ModReference ID. So keep that in mind.
1. Go to Class Settings and implement the `AvatarProvider` interface.
1. In the `Get Avatars to Register` function implementation for `AvatarProvider`, drag your AvatarDescriptors map variable out and connect it to the output.

And... you should be more or less done! If you compile/install your mod along with CharacterReplacer, it should automatically detect and load your registered avatars from your new mod and any others you have, and from there, it will take care of letting you select one from the mod config page and play with others with their own selected avatars! Just make sure everyone has the same set of avatar mods.

(show image of the Root GameInstance settings, and an image of the AvatarDefinition class)

( talk about the footprint overrides at some point maybe )

## Troubleshooting

**Did you remember to save your unreal assets before packaging?** If not, go to File -> Save All. And try again. Trust me, I've forgotten *so many times.* It causes the *dumbest* bugs.

### Problems with the Model behaving in game

If the model shows up in game, but it looks weird, behaves weird, etc, then you most likely messed up something either:
- during the re-rigging process in blender / your modelling software of choice, or
- while importing the model into unreal.

If interaction is behaving weirdly, and items are not dropping on the ground, and the model is scaled way up in the editor... make sure it isn't scaled way up. Sometimes while getting the model, it ends up at 100x scale, so you'll need to scale it back down either while exporting the fbx or importing it to unreal.

If the model is broken, or doesn't show up, or doesn't move or animate, try making sure all the bones match exactly as expected, and that your model *does have* all the bones of the actual pioneer model. Certain methods of obtaining the base model from the game may end up with some socket bones or other bones missing, so double check against the actual skeleton in unreal if possible.

On that note, when assigning the original skeleton to your custom mesh, keep an eye out for anything red or off or missing in the bone list during assignment.

Note that extra bones on your avatar are not simulated at the moment... until I figure out how to make it work in game. If you know about unreal physics assets in game, and have magic to make it work, feel free to give it a go, and tell me if you have any success!

### Problems Assigning the Skeleton

If you get a bunch of bone merge errors when you try to assign the skeleton, then you're missing bones, or you structured your armature wrong. Make sure everything matches.

If it prompts you "hey these bones are missing, should we automatically add them?" generally that means your avatar has more bones than the base game, and it *should* merge fine. It's only an issue if _your_ model is missing bones from the _game_ skeleton.

### Problems with Registering the Model / Mod

If your problem is with getting your model to show up in the list of registered models, make sure you've implemented the AvatarProvider interface in your Root GameInstance Module, you made an array and it contains your avatar definition(s), and you connected your blueprint properly so that the array is being returned by the AvatarProvider function.

### Debug Commands

If you every have issues with things, or want to get more information, there are a few chat commands available, which you can also see with /help:

- /listav /la /ls
  - lists all locally registered avatar IDs in the game console, accessible with backtick/tilde key: `
- /model /m <id>
  - acts as if you had set your avatar id in the mod config.
- /refresh /r
  - Locally refreshes each player's avatar, according to the selected ID that's been replicated from the server. Sometimes fixes caching issues.
- /resend /rs /rsall
  - Reads your config and resends its ID to the server as if you'd changed avatars. For /rsall, forces all players to do the same. Helpful if something wasn't replicated properly.
