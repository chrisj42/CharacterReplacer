# Satisfactory: Character Replacer

## Install

This mod will usually be installed as a dependency of actual model mods; installing it alone serves little purpose. Still, it is available on the SML registry as "CharacterReplacer".

If you want to make an avatar mod of your own however, you're going to want to download / clone this repo to your local dev mods folder.

## Overview

This mod is a library mod to make it easier to use custom avatars with your friends (or alone) in Satisfactory.

No custom avatars are included when you install this mod alone. It is designed to handle the behavior needed to manage syncing, selecting, and replacing player meshes which have been set up and registered using your own mod(s).

## How to Make Compatible Avatar Mods

I've created the mod as a beginner to both unreal and satisfactory modding, but I've tried to have it take care of as much of the process as possible, which still encompassed a good few weeks of debugging and programming... but if you want to add your own models, you _will_ have to nonetheless get your hands a little dirty setting up the all the raw assets in unreal, and then doing a quick attachment to my mod, before packaging yours.

The API to register your own avatar(s) is fairly simple, consisting of a couple main steps, only some of which I'll cover here:
1. You obtain the game rig that you'll need to conform your model to
1. **Not Covered** (mostly): You have your avatar re-rigged (as with player mesh replacement in most games)
1. **Not Covered**: You've gone through the modding env set up as described on the satisfactory modding wiki/docs
1. You import your model(s) into unreal
1. You set up a quick interface with CharacterReplacer to register your avatar(s).

---

## 1. Obtaining The Player Rigs

Re-rigging a model can be complex, and obtaining the armature to rig to can also take time; but it shouldn't be anything unusual to someone who has replaced rigged models in other games before. There are many guides out there and many communities which will help.

> [!IMPORTANT]
> **You will need to make *two separate models: for first and third person***. Their bone structure *does vary a bit*, and you won't want head stuff on your first-person model. So make sure you pay attention and get both versions of the rig for reference.

Just a note before we start, throughout this doc, I'll be short-handing "first person" to "1p" and "third person" to "3p". Just for reference.

So, to get a rig (and create your test avatar) you will need:
- FModel
- Blender 4.0
- The psk importer plugin for blender found here: https://github.com/DarklightGames/io_scene_psk_psa

I'm not saying it's impossible to get your assets from other sources, but this (should) work. The modding community has an asset dumper, but I found that the models I got from that were missing a lot of bones, and it caused me many problems.

**Update Note:** As of FModel 4.4.3.6, they seem to have fixed gltf/glb exports, which blender natively supports importing. So, you might not need the version restrictions or the plugin this way. It also doesn't seem to import at 100x scale. However, when I tested it, it rendered really glitchy, so there's probably a setting somewhere that needs to be set; but I can't be bothered to debug it. Let me know if you choose to go this route and successfully get a model to behave correctly, I'd be happy to add that here.

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

At this point, you should have 2 PSK files, called `Character.psk` and `1PCharacter.psk`.

Then, open blender, and install the `io_scene_psk_psa` plugin. If it doesn't install correctly, use a legacy steam branch or other means to get specifically blender 4.0, which should work. Then you can just import each psk to a separate blend file, and you should have the rigs available for your use.

> [!NOTE]
> The models will likely import at 100x scale. Just an artifact of the whole fmodel/psk thing it seems. Select all and **S**cale down to 0.01, then you should be good to go. 

---

## 2. Rigging Requirements in Blender

Here is where I'll talk about what modelers need to know to make a compatible FBX in the game. This has nothing to do with my mod, it's just how satisfactory's player meshes work.

If you want to test the rest of the process out before getting into the nitty-gritty of re-rigging (an idea which I highly recommend), skip the tips below, and continue with the [Final Edits](#final-edits-and-export), using the pioneer meshes you just imported.

### Tips on Re-Rigging

> [!IMPORTANT]
> To re-iterate, **you need to ensure you have both player models.** 1p and 3p *have important distinctions.* You can probably copy a lot of your work from one to the other, but **you will be exporting two distinct FBX files.**

Now then, for those who are ready to make their own avatar (and who ideally already have experience in this area), here are some tips mostly specific to satisfactory:
- Keep the head on the 3p model, and remove it on the 1p model. Unless you want to try and use the base-game helmet. Then remove it on both.
- The 1p model is structured a fair bit different, as the arms are severed from the body bone-wise. For, you know, first-person reasons.
- For the 3p model, the head *must* be all weighted to a single head bone. Like, you *can* keep other bones on it, but... don't expect the physics engine to like you. It might be possible to find a way to make said bones work well, but I haven't figured it out. Let me know on the discord if you figure something out!
- If you can export it with materials, feel free; it will save time on set up in unreal, allowing you to just import the materials instead of recreating them.

### Final Edits and Export

The armatures exported from FModel aren't actually *exactly* what the game expects, so you need to make a couple edits first. For **both the 1p and 3p models:**
   - Go into armature edit mode, and delete the root bone.
   - Rename the armature object (the green object under "Pose") to `Root` (capital R).
   - Rename the parent armature object (the top orange one) to `Root` as well.

You should then be good to export it as an fbx. Open the export to FBX dialog and disable "Add leaf bones", then click "Export FBX".

> [!NOTE]
> Getting the model configured at this stage (basically getting a correctly-configured fbx file) is by far the most touchy, annoying, buggy step. If something doesn't work, it was probably because of this. Even if you follow everything, it might need some blood sacrifices. And tinkering. And comparing against the main model and skeleton. Good luck!

---

## 3. Getting Ready for Unreal

With all the above said and done, and you have your FBX files in hand, it'll be time to start putting things into the Unreal Engine. This is where I will direct you to the modding docs for setting up a starter project, so come back when you've got that all sorted. https://docs.ficsit.app/satisfactory-modding/latest/Development/BeginnersGuide/index.html

The Satisfactory modding community will gladly help with any problems you may have setting up the modding environment. It is a somewhat long process, but it is unfortunately required, as there is no way to add an fbx or other model file to the game without running it through the full unreal packing process in the specific custom engine version used by the satisfactory devs, and modified further by the modding team. But modding team are working on tools to automate as much as possible the process of setting up this env for you. So that's very nice of them. :)

## Creating your Avatar Mod

Once your environment is set up, you can make a new, simple blueprint mod, and get started on the couple steps you need. Everything below will be done inside the `Content` folder of a new blueprint mod you've made via the Alpakit wizard. Remember, refer to the Modding docs or discord for how all that stuff works.

Note: **Always remember to save your assets!!!** Unreal does not automatically save changes to assets like imported models, textures, materials, or blueprints or anything else.
You *must* do this yourself, and if you forget, and then pack your mod, it *will* behave strangely and you *will* be confused. And I _will_ be sad. Go to `File -> Save All` for easy saving.

### Importing your Model into Unreal

You're going to want to do the following for both your first person and third person models.
Side note, as you create your assets, if you are adding more than one avatar, I'd recommend using subfolders for each one as you drag stuff in; it will get messy otherwise. :)

1. Drag your fbx into the editor, and you'll be greeted with the import dialog. Ensure the following non-default options are changed:
   1. Set "Use T0 As Ref Pose"
   2. Look under the "Material" Section lower down; typically, you can probably just leave these alone, allowing it to create new materials for you based on the model. But if they're the same materials for both the 1p and 3p models, you probably don't need to import the same materials for both. Just reuse one of them for the other.
   3. Hit "Import All".
1. Unreal will create a Skeletal Mesh, a Skeleton, a PhysicsAsset, and material slots according to your avatar, if you opted to create them.
   - If your model didn't come with the materials attached, you'll have to recreate them and import the textures. You can configure the materials as you wish (it's a shader graph like blender, but different). I won't be covering how to do that here, it's basic shader stuff, you can look at unreal docs or most node based material editors.
   - Note: I *did* create some materials that you can create "Material Instances" from, which may simplify setup depending on how your textures are laid out. I was having issues getting the materials to actually set properly in game with somebody's avatar, but instances of those seem to work... for some reason.
1. Right Click the skeletal mesh (NOT the skeleton), and click "Assign Skeleton"
1. For the 1p model, look for `1PCharacter_Skeleton`; for the 3p model, look for `Character_Skeleton`. Assign this to your skeletal mesh.
   - You may want to watch out for any errors in this step; it may lead you to realize if any of your bones are missing. It won't show _every_ possible bone problem though.

Note: There may be other, more complex ways to handle skeletons; unreal has a ton of fancy rigging stuff. You're free to give it a go. But this assumes little unreal knowledge (and comes from someone with little unreal knowledge), so we're going for the simplest solution where all the rigging work is in blender.

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

As a last step... I'd suggest listing CharacterReplacer as a dependency of your mod. After all, it won't work without it! If you aren't planning on publishing the mod to the SML registry though, then it probably isn't necessary. You can find instructions here: https://docs.ficsit.app/satisfactory-modding/latest/Development/BeginnersGuide/ReleaseMod.html#_special_fields

And with that, you should be good to go! If you compile/install your mod along with CharacterReplacer, it should automatically detect and load your registered avatars from your new mod and any others you have, and from there, it will take care of letting you select one from the mod config page and play with others with their own selected avatars! Just make sure everyone has the same set of avatar mods.

(show image of the Root GameInstance settings, and an image of the AvatarDefinition class)

( talk about the footprint overrides at some point maybe )

## Troubleshooting

**Did you remember to save your unreal assets before packaging?** If not, go to File -> Save All. And try again. Trust me, I've forgotten *so many times.* It causes the *dumbest* bugs.

### Problems with the Textures not showing up

I don't have a whole lot to say about this, however I did find that when I later tried to make an avatar for somebody, for SOME REASON The new materials I created for them in unreal just didn't work in game. It would show us untextured. However using a texture from one of my initial models when I first made the mod worked, when I copied it and changed out all of the textures. So... I haven't figured that one out. But if you do also run into issues of the textures not showing, try creating a material instance, And select one of the materials in the character replacer mod files as the parent, or alternatively, just copy the material and then insert your textures. Hopefully that works!

### Problems with the Model behaving in game

If the model shows up in game, but it looks weird, behaves weird, etc, then you most likely messed up something either:
- during the re-rigging process in blender / your modelling software of choice, or
- while importing the model into unreal.

_Items not dropping, or dropping high in the sky? Ragdoll spazzing into the next dimension on death?_ Your model is probably scaled x100 in the editor. Delete the skeletal mesh and re-import the FBX, but set `Transform -> Import Uniform Scale` to 0.01.

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
