# Satisfactory: Character Replacer

## Overview

This mod is a library mod to facilitate custom player avatars, alone or with your friends, in Satisfactory.

> [!IMPORTANT]
> <ins>**No custom avatars are included when you install this mod alone.**</ins> It is designed to handle as much of the work needed as possible to integrate arbitrary custom models into the game, so that modellers without coding experience can integrate their avatars without having to worry about all the fancy details.

### Installing

This mod will usually be installed as a dependency of mods containing actual avatar models; installing it alone serves little purpose, as does browsing this github unless you intend to make one of said mods.

Here's a link to the SMR page though just for fun: https://ficsit.app/mod/CharacterReplacer

### So what models are there?

The only published, public mod to my knowledge as of writing is Protogen Pioneers: https://ficsit.app/mod/ProtoPioneer

But, many people have used this so far to take their own private models and enjoy them in the game with friends!  
So, if there's a model you can think of that might be cool in the game, read on! Blender experience is probably recommended, but with enough determination, anything is possible. Or bother your modelling friend who has never touched a factory game in their life until they cave and add it for you! I'm not picky, but I'm happy to support anyone who wants to try. :) My personal experience with the modelling side is limited, but I can try to at least point you in the right direction.

### I want to put a model in!

Awesome! I've tried to make the process of adding support for new models as painless as possible; you _will_ be most likely fiddling around in blender with armatures a good bit, and you _do_ have to set up the modding env in unreal, but my code takes care of pretty much everything once you've managed to get unreal to recognize your mesh and textures. In other words, required coding experience is minimal to none.

Join the [modding discord](https://discord.ficsit.app/) if you need support! You can find my contact details on the [SMR page](https://ficsit.app/mod/CharacterReplacer), I would love to hear about your efforts and I'd be happy to clarify anything that this guide doesn't cover. I haven't made a dedicated server for the mod yet, but if you'd be interested in one, let me know as well!

With that out of the way... let's get into the fun part!

# How to Make Compatible Avatar Mods

Once you've decided on a model, here's a rough outline of what needs to happen if you want to see your lil guy in the videogame:

[Step 1.](#1-obtaining-the-player-rigs) (FModel):  Extract the player armatures / model from the game files

[Step 2.](#2-rigging-requirements-in-blender) (Blender): Rig your model to fit the armatures

[Step 3.](#3-getting-ready-for-unreal) (Unreal):  Set up the Satisfactory Modding Evironment

[Step 4.](#4-creating-your-avatar-mod) (Unreal):  Create your mod and import the rigged models into Unreal

[Step 5.](#5-registering-your-model-as-an-in-game-avatar) (Unreal):  Register your avatars with Character Replacer

> [!IMPORTANT]
> **<ins>You will need to obtain *two separate rigs*</ins>** to make an avatar: one for first person, one for third person. They're not *that* different, but both are required.

Thus, ***a note on terminology:*** I'll be short-handing in this doc "first person" to "1p" and "third person" to "3p".

---

## 1. Obtaining The Player Rigs

To get the rigs into a usable state you will need:
- FModel
- Blender 4.0
- The psk importer plugin for blender found here: https://github.com/DarklightGames/io_scene_psk_psa

### FModel
We will start with the FModel process.

Largely, the Satisfactory modding community has great docs for setting up FModel: 

https://docs.ficsit.app/satisfactory-modding/latest/Development/ExtractGameFiles.html

> [!WARNING]
> However, the following changes / differences from the above guide should be used instead:
> - Set the model export format to PSK, *not* GLTF

> [!NOTE]
> *Why not GLTF?*
> 
> While the docs do recommend using GLTF, my experience is that the armature imports completely messed up. The bones are all there, but they are in the entirely wrong positions, and attempts to import the model to Unreal result in an invisible mesh. If you get it to work, then do tell, I'd love to eliminate the 4.0 version and plugin requirements.

With that in mind, go ahead and follow those instructions to set up FModel, and come back when you're ready to extract the player meshes.

The Character Meshes can be found in `FactoryGame/Content/FactoryGame/Character/Player/Mesh/`. You'll want to right-click and select "Save Model" for:
- `SK_Pioneer_01.uasset` (the third person / 3p model)
- `SK_Pioneer_1P_01.uasset` (the first person / 1p model)

> [!NOTE]
> If you want to have the vanilla pioneer textures as well, for reference, you can find them in the `Textures/Pioneer_Textures` folder next to the `Mesh` folder. Be sure to select "Save Textures" instead.

At this point, you should have 2 PSK files, called `SK_Pioneer_01.psk` and `SK_Pioneer_1P_01.psk`.

Then, open blender, and install the `io_scene_psk_psa` plugin. If it doesn't install correctly, use a legacy steam branch or other means to get specifically blender 4.0, which should work. Then you can just import each psk *(I recommend doing so in separate blend files)*, and you should have the rigs available for your use.

> [!WARNING]
> The PSKs will likely import at 100x scale. It's an artifact of unreal units being different. Select all (the mesh and the armature) and **S**cale down to 0.01, then you should be good to go. **<ins>If you leave the models at 100x scale, you will run into strange interaction bugs during testing.</ins>**

---

## 2. Rigging Requirements in Blender

Here is where I'll talk about what you need to know to make a compatible FBX for the game. This has nothing to do with my mod, it's just how satisfactory's player meshes work.

If you want to test the rest of the process out before getting into the nitty-gritty of re-rigging (an idea which I highly recommend), skip the tips below, and continue with the [Final Edits](#final-edits-and-export), using the pioneer meshes you just imported.

### Tips on Re-Rigging

Unfortunately, the rigging process itself is a bit out of my wheelhouse. If you've done weightpainting or rigging before, you're in good shape. If you haven't, well... take a look around other modding communities that do mesh replacements, or look on youtube, or ask a friend. You essentially need to end up with the **exact** bone structure of the player rig you're trying to fit, while making sure the weightpainting still behaves decently at the end.

Here are some tips mostly specific to satisfactory:

- Keep the head on the 3p model, and remove it on the 1p model. Unless you want to try and use the base-game helmet. Then remove it on both.
- For the 1p Armature: note that the arms are separately parented from the rest of the body. In previous versions, the mesh had to be severed as well, but from the looks of it, that isn't the case anymore. Still, worth watching out for.
- For the 3p Armature: the head *must* be all weighted to a single head bone. Like, you *can* keep other bones on it, but... don't expect the physics engine to like you. It might be possible to find a way to make said bones work well, but I haven't figured it out. Let me know on the discord if you make it work!
- Don't get too fussy with materials; chances are you'll have to recreate them in Unreal from the texture files anyway. Or, even better, you can use one of the material templates provided in the core mod, if your textures work with one of them.

*Incorperating base-game assets to support customization:*

> [!WARNING]
> When considering using base-game assets in custom models, be wary of distributing copyrighted content of Coffee Stain Studios. If you do incorperate any significant amount of base game material into the model for aesthetic or other purposes, you probably shouldn't upload it to SMM.

If you re-use the clothing meshes from the original pioneer, and thus the same materials, you will automatically gain support for the vanilla customizer to recolor said clothing on your mesh as well, which can be a great point of immersion.

Alternatively, if you want to make something releasable, you can try making your own clothing, or other materials, that are compatible with the vanilla pioneer materials, and thus also recolorable. I can't really help with that, but the potential is definitely there, and there's no reason the core mod wouldn't allow you to do it. I hope to figure out how to properly support recoloring, particularly for more arbitrary materials, in future updates.

*Using the Base Game Helmet(s):*

The base game pioneer mesh renders the helmet as a separate static mesh to the main character body. Thus, you have the option of keeping that and working it into your avatar, or hiding it and just doing your own thing.
- If you do design your avatar with the helmet in mind, it'll mean the vanilla customizer will be able to be used to choose between the various helmets in game and they'll actually show up. In that case, you shouldn't have the head on the 3p model, where usually you would.
- If using the base game helmet unchanged doesn't work with your model's head/neck shape, but you still want to add helmet customization, consider just making separate 3p avatars for each helmet type. In a future update, once I figure out how to more deeply integrate into the vanilla player customizer, I intend to allow for such avatar sets to override the helmet selection, among other things.

### Shape Keys

Depending on how you re-rig your models, you may or may not end up with shape keys on your mesh, as blender refers to them. As of version 1.4.0, any shape keys on your 3P mesh (if you import them into unreal) will show up in game in the locker customizer for users to customize! So... with that in mind, make sure the only shape keys on your 3P mesh before exporting are ones that you wish to expose to the player. Feel free to add new ones as well, of course!  
Essentially:
- the 3P mesh defines the shape keys available for players to customize on your avatar
- they have an expected range of 0-1 (as is common for shape keys); you will not be able to put in values outside that range.
- these shape keys will also be applied to your 1P avatar by name (or at least it will try), so add them there too if you want them to affect first-person.
- Note that Unreal will refer to shape keys as "Morph Targets"; they are the same thing.

### Final Edits and Export

Make sure you do the following just before exporting each of the rigs of your new avatar:
- Ensure the Armature object (the lil orange person shape containing everything) is named "Armature".
	- Otherwise, Unreal will create a bone for it, and it won't match the base game skeleton. ***If both your rigs are in the same blend file, make sure the one you're exporting doesn't have a "_001" suffix when you export.***
- Ensure "Add leaf bones" is disabled in the "Export FBX" dialog.
	- Default settings should be fine for the rest. If you're only using one blend file, make sure not to export both rigs at the same time; use the filter settings to do each individually into separate FBXs.

> [!NOTE]
> Getting the model rigged and exported correctly here can be very touchy, particularly if you don't have much rigging experience. Again, unfortunately, I can't really help you much with this step, but hopefully I can get a proper guide for it here eventually.

## 2.5. Make an icon for your avatar

Later on, we're going to give your avatar an icon to display in the vanilla customizer. While you have all your art and modelling tools open, now might be a good time to make sure you have one!

You don't have to do this now, and you can use whatever image you want, but I just thought I would mention this here.

> [!NOTE]
> Do try to make sure your image isn't larger than 256x256 in its final form.

---

## 3. Getting Ready for Unreal

With all the above said and done, and you have your FBX files in hand, it'll be time to start putting things into the Unreal Engine. This is where I will direct you to the modding docs for setting up a starter project, so come back when you've got that all sorted. https://docs.ficsit.app/satisfactory-modding/latest/Development/BeginnersGuide/index.html

The Satisfactory modding community will gladly help with any problems you may have setting up the modding environment. As it says in the docs, if you need help, [join the discord](https://discord.ficsit.app/)!!

It can be a somewhat long process, but it is unfortunately required, as there is no way to add an FBX or other model file to the game without running it through the full unreal packing process in the specific custom engine version used by the satisfactory devs, and modified further by the modding team. But modding team *are* working on tools to automate as much as possible the process of setting up this env for you. So that's very nice of them. :)

## 4. Creating your Avatar Mod

Once your environment is set up, you can make a new, simple blueprint mod, and get started on the couple steps you need. Everything below will be done inside the `Content` folder of a new blueprint mod you've made via the Alpakit wizard. Remember, refer to the Modding docs or the discord for how all that stuff works!

> [!IMPORTANT]
> **<ins>Always remember to save your assets!!!</ins>** Unreal does not automatically save changes to assets like imported models, textures, materials, or blueprints or anything else.
> You *must* do this yourself, and if you forget, and then pack your mod, it *will* behave strangely and you *will* be confused. And I _will_ be sad.
> 
> Go to `File -> Save All` for easy saving.

### Pre-Req: Download the Core Mod Source

Before you get too carried away, make sure you download the source of Character Replacer from this repository; you're going to need to be able to reference some of the contents in the following steps.

You can do so by `git clone`ing into your `Mods/` folder, or just downloading the zip, and unzipping it there. It should go into a folder right next to your new avatar mod, and look fairly similar in structure.

Close, then re-open the editor to load the new files (re-compile anything if prompted), and then continue.

### Importing your Model into Unreal

You're going to want to do the following for both your first person and third person models.
Side note, as you create your assets, if you are adding more than one avatar, I'd recommend using subfolders for each one as you drag stuff in; it will get messy otherwise. :)

Drag your fbx into the editor, and you'll be greeted with the import dialog. Ensure the following non-default options are changed:
   1. Select the Skeleton for your mesh; for the 1p model it's `1PCharacter_Skeleton`, for 3p it's `Character_Skeleton`. If these are grayed out and unreal does not have "Skeletal Mesh" checked by default, then you've exported your mesh incorrectly, and probably forgot to add an armature modifier / didn't export your armature.
   2. Open the "Advanced" dropdown in the "Mesh" section and enable "Use T0 As Ref Pose"
   3. Look under the "Material" Section lower down; select "Do not create material" and optionally "Do not import textures". It probably won't import all the textures anyway, and it's easier to just import them manually once later.
   4. Hit "Import All".

Unreal will create a Skeletal Mesh and a PhysicsAsset for your avatar.

> [!WARNING]
> Did you get a popup saying "Bones cannot be merged"? Try the following:
> - Make sure you selected the right skeleton and imported the right mesh; using the 1p skeleton for the 3p avatar or vice versa will *not* work.
> - Open the model back up in blender and make sure the main Armature is *named* "Armature". It'll create extra bones otherwise.
> 
> If you're still having issues:
> - Cancel the import and try again *without* selecting a skeleton; let unreal generate one for you.
> - Right click the skeletal mesh (NOT the skeleton), and click "Assign Skeleton", then find the base game skeleton from earlier. This way, it will pop up with a list of bones, and you can see how it's parsing it.
> - Bones that you added in your own model are fine, but keep an eye out for anything in red that should be in the main skeleton.
> - Annoyingly, it will not show you everything this way, i.e. it doesn't show bones that are on the *new* skeleton but not the *old* one. Try opening both Skeleton Assets yourself in unreal and go down the lists making sure you're not missing any bones, or have any differences in structure (like an extra root bone).

> [!NOTE]
> Did you get a popup asking you to confirm adding additional bones to the skeleton?
> 
> This is probably fine, it just means you have extra bones on your avatar. In fact, keeping bones where possible is beneficial, as they will be used in ragdolls and you won't look unnaturally stiff due to missing bones.
> 
> Unfortunately, I still haven't figured out how to actually make use of said extra bones *outside* of ragdoll animations, but I'll update this if I do. If you figure something out, poke me on the discord!

### Setting up Materials and Textures

Now that both the 1p and 3p versions of the model have imported correctly and the skeletons are assigned, we can get onto importing your textures and assigning materials.

**<ins>For Textures:</ins>**
1. I'd recommend making a subfolder for them, then just drag and drop every texture in, and unreal will automatically import them.
1. Make sure your textures are being interpreted correctly by unreal. Data textures like normal maps and MSR textures should have "sRGB" off, while base color and emission textures should have it on. Hover over the asset to see how it's set, and double click to edit if it's not right.

**<ins>For Materials:</ins>**
You have two options: create Material Instances, or create new raw Materials.

It's recommended that, if possible, you make a child material of an existing Material or Material Instance. This vastly improves performance (although for player models that's really not relevant), and makes your material configurable at runtime, as well as, if you use the right instances, modifiable by other systems in the game.

You can find instance-able materials I've provided in "Character Replacer Content / Material Templates" in the Content Browser. See if any of the available templates have texture parameters that fit the textures your model uses, and if so, make a Material Instance as a child of that. You do not need to specify values for every parameter.
Otherwise, do feel free to makw your own materials, perhaps instancing those for each material slot in your model if they behave the same.

***To make a Material Instance:***
1. Right click -> Add/Import Content (Or the Add button in the Content Browser) -> Create Advanced Asset -> Material -> Material Instance
1. Name it how you like, preferably matching the material slot that you're going to use it for.
1. Open it up, scroll down a bit, and select a parent material to inherit from.
1. A list of parameters will appear above. Check the boxes for your relevant textures and assign them, then save the asset.

***To make a new Material:***
1. Right click -> Add/Import Content (Or the Add button in the Content Browser) -> Material
1. When you open the asset, you will be greeted with a node-based shader graph. Unreal has a different naming scheme than blender, so it can be annoying, but feel free to look at existing materials / the provided material templates for an idea of how to plug stuff in.
1. Remember to "apply"/save/compile the Material when you're done.

#### Material Parameters

This is a good time to talk about material parameters. As of version 1.4.0, any "parameters" you define on your materials will be read by the mod and displayed to the player for editing. Material parameters are a basic Unreal feature, so feel free to read the UE docs on them. "Vector" parmaeters (colors) and "Scalar" parameters (floats/numbers) are the only parameter types which will be read by the mod. If you want to use parameters for internal purposes and not show them, or specify the order they are presented to the player in the UI, don't worry; we'll be able to handle that when registering the model. So feel free to define whatever parameters you like.

Additional Notes:
- All parameters on all materials assigned to either mesh are drawn from to display customization options.
- If a parameter on one material shares the same type and name as a parameter on another material, they will be modified in sync.
- scalar parameters will only be able to be set to values from 0-1. If you want a scalar outside that range, simply map your range to the desired range with a LinearInterpolation node (plug the parameter value into the alpha pin).
- parameters from any vanilla materials are filtered out, *unless* you specify a specific parameter list. Even then, many may not work due to the way they are handled by the game. Best to make your own parameters.

#### Finishing up

Finally... once all your materials are set up, open your skeletal mesh assets and assign the materials / material instances you created to their respective material slots. For most avatars, they will likely be the same ones, minus any eye or other head-only material slots on the 1p version.

> [!NOTE]
> Wondering about bouncy physics on extra bones? I'll try getting a guide up on it at some point, but I had success using trail controllers in a post-process animation blueprint. Others have gotten rigidbody simulation to work as well. It can definitely be finnicky though, and head-bone physics could still be jank. Good luck!

## 5. Registering your Model as an in-game Avatar

At this point, your avatars should be fully rigged and textured and essentially all set to go; all that's left is the CharacterReplacer-specific stuff that takes your new Unreal Skeletal Meshes and plugs them into the game.

> [!NOTE]
> After watching someone, I'm going to pre-emptively apologize for not having images for this part yet, I should really add those... if you can't figure out whether you're doing it right please feel free to ask!

### Create Avatar Definitions

For each Avatar you want to add (essentially, for every 1p/3p pair of models), we need to package up the relevant information into a data file. Start with filling in these fields, and reference the appropriate sections below for more info on the rest:
   1. Create a new `Misc/Data Asset` in the folder with your avatar assets, and parent it to the `AvatarDefinition` type.
   1. Name the asset after your model (doesn't particularly matter, but I use `Avatar<name>`), and then open it.
   1. Drag in or select your 1p and 3p skeletal mesh assets into the 1p mesh / 3p mesh fields respectively.
   1. Select "keep helmet" only if you want the base pioneer's head to be rendered in addition to your mesh on the 3p avatar.
   1. Create a Locker Descriptor (see below) and assign it.

#### *Material Overrides:*

If you have multiple possible textures for the same model, you may wish to create avatars for each of them. Using the Material 1p/3p Overrides arrays, you can specify the materials to use on a per-avatar basis instead of having to copy the mesh asset and assign the materials there.
- Ensure the order you put the materials in exactly matches the order expected by the mesh you have in the associated 1p/3p field.
- You can specify only a couple materials, or leave gaps in the list, and only specified materials will be overridden.

#### *Material Parameter Whitelist:*

If you don't have any fancy material parameters (and you don't have to!), you can leave this blank.  
If you want to display all your material parameters, and you don't really care what order they go in, you can leave this blank.  
Otherwise, use this list to order your material parameters in the UI for the player and select which ones to provide for them to customize.

#### *Force Hidden Equipment:*

This set simply allows you to hide certain pieces of equipment from the avatar. I'm not even sure myself as of writing this how to tailor equipment positioning for a model, or if it's even possible; but if it just looks bad, you can use this to hide it. It's purely cosmetic; just meant to help out weird player model body types.  
This was a request from a modeller; I may add more types of equipment if there are any / if you poke me about it!

#### *Footprint Overrides:*
- You may notice a complicated looking array field in the Avatar Defintion called "Footprint Overrides". This can be used to change the footprint decals that the game applies to the ground in various areas of the map, like sand, swampland, etc. if your avatar isn't wearing boots.
- Honestly I just added this because I thought it was funny; if you want to try and use it, the base game normal maps to reference are in `FactoryGame/Content/FactoryGame/VFX/Textures/Char/Player/` in the utoc file in FModel.
- The game only seems to use Sand, Mud, and Soil at the moment, which are in the array at 1 (Surface_Sand), 5 (Surface_Moist), and 10 (Surface_Soil). Go wild, but note that they may be a little finnicky to get to display properly.

#### *Avatar Locker Descriptors:*

This defines how your avatar will show up when a player goes to the customization locker in the Hub.
- Create a new `Blueprint Class` next to your AvatarDefinition, and parent it to the `AvatarDesc` class.
- There is going to be a lot of stuff here; most of it is *useless*. Pay attention to:
   - `Item -> M Display Name`: A friendly name/title for your avatar.
   - `Item -> M Description`: A small blurb of text describing your avatar. Or, honestly, put whatever you want lol.
   - `UI -> M Small Icon`: The icon that's displayed for your avatar. Use an image that represents them, a profile photo, a screenshot of them, whatever really. Try to keep it under 256x256.
   - `UI -> Big Icon`: Use the same icon as the last.

### Register With CharacterReplacer

Once you have the avatar definitions, it's a fairly short but nerdy jaunt into making a simple blueprint to connect everything up:

1. Create a new blueprint class in your mod's Content folder and select it to be a `GameInstanceModule`; name it `RootInstance_YourmodID` (replace with your id/name; the file name is just for reference and standard, doesn't actually matter).
1. Open the new Game Instance Module, and mark it as a root module at the bottom of the details list.
1. If you don't already see a blueprint grid (the "Event Graph") in the middle of the screen, open the "full blueprint editor" with the blue link near the top of the screen.
1. Create a new variable on the left, and call it "AvatarDescriptors" or something like that; change the type to a map of `Name` -> `AvatarDefinition`.
1. Compile the blueprint with the button on the top-left.
1. Add an entry for every `AvatarDefinition` you set up using the Details pane on the right, and choose an ID for the key for each avatar. This will be how you identify each avatar in game, so just give it a simple, no-spaces alphanumeric name. The full ID will be `:<YourModReference>:<AvatarName>`, but both will work in game; the full name is just to resolve name clashes.
1. Click "Class Settings" (above the blueprint grid) and under the Details pane, "Interfaces" section, implement the `AvatarProvider` interface.
1. This creates a `Get Avatars to Register` function on the left for `AvatarProvider`; double click it to create an implementation, drag your AvatarDescriptors map variable out onto the grid, and connect it to the output of the function. (the little icons on the sides of each "node" can be connected by dragging from one to the other)
1. Compile the blueprint one final time, and save all assets.

### Configure Mod Dependencies

As a last step, list CharacterReplacer as a dependency of your mod. After all, it won't work without it! You can find instructions here under the Plugins row: https://docs.ficsit.app/satisfactory-modding/latest/Development/BeginnersGuide/ReleaseMod.html#_special_fields

I would recommend using the semver format they encourage: prefix the current CharacterReplacer version with "^". I intend to maintain the rules of semver in my versioning, so using the right dependency version on my mod will mean yours will continue to work until I denote that I'm making incompatible changes.

Also consider the "Required on Remote" setting. The way my mod works, it is entirely acceptable if you have an avatar mod locally that the server doesn't know about. You will just show up as the base pioneer or whatever people have put as their "missing" avatar.  
Not saying you _should,_ because avatars are cool, but the option exists if you want to use it for whatever reason.

--

With that, you should be good to go! If you compile/install your mod along with CharacterReplacer (Check modding docs for alpakit buttons), it should automatically detect and load your registered avatars from your new mod and any others you have; and from there, it will take care of letting you select one from the mod config page and play with others with their own selected avatars!

### Releasing your Mod

You do not have to release your mod on SMM if you don't intend your avatar to be public (or if your model(s) use copyrighted assets for whatever reason that would cause issues). In this case, you would just zip the compiled mod in your game install Mods folder and send it to whatever friends you want to play with (or enjoy it yourself!).

But if you *do* want to put it into the open for public use, by all means, please do! The previously linked docs page has what you need: https://docs.ficsit.app/satisfactory-modding/latest/Development/BeginnersGuide/ReleaseMod.html

I would recommend, when uploading, to tag it as #player and mention that it's a CharacterReplacer mod, for better searchability later.

## Troubleshooting

***Did you remember to save your unreal assets before packaging?*** If not, go to File -> Save All. Then package it again. Trust me, I've forgotten *so many times.* It causes the *dumbest* bugs.

### Known Bugs

There are some minor bugs in 1.4.0 that will be corrected eventually in later patches (I wanted to get it out for people sooner). They're not much, but they're good to know about:
- Do not use the config menu to change your avatar ID, or the /avi command; it will mess up some customization info and behave weird for a bit. Changing avatars a few times from the locker usually makes it go away. Avoid these for now; the locker is meant to be your one-stop shop for avatar editing anyways.
- if you change your avatar frequently, you might find that you join a server with the wrong one on. Do an /avi-rs or just swap away and to it again in the hub locker and you should be all good.

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

- /list-avi /lsavi
  - lists all locally registered avatar IDs in the game console, accessible with backtick/tilde key: `
- /avatar /avi <id>
  - acts as if you had set your avatar id in the mod config.
  - **Note: In 1.4.0, this will mess up some of your customization info on the avatar and it's a little buggy. Stick to using the hub locker for now.**
- /refresh-avi /avi-r
  - Locally refreshes each player's avatar, according to the selected ID that's been replicated from the server. Sometimes fixes caching issues.
- /resend-avi /avi-rs [all]
  - Reads your config and resends its ID to the server as if you'd changed avatars. With "all", forces all players to do the same. Helpful if something wasn't replicated properly.
- /offline-avi /avi-off <id> [all]
  - Sets the avatars of offline players nearby; the closest one by default, all loaded ones with `all` as the second argument. Might not work/save if you're a client, it's really just meant as a debugging command.
