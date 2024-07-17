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

The Satisfactory modding community will also gladly help with any problems you may have setting up the modding environment. It is a somewhat long process, but it is unfortunately required, as there is no way to add an fbx or other model file to the game without running it through the full unreal packing process in the specific custom engine version used by the satisfactory devs and modified further by the modding team. The modding team are working on tools to automate the process for you as much as possible, though. So that's very nice of them. :)

## After Completing the PreReqs

With all the above said and done, once you have your FBX, and your environment is set up, you can make a new, simple blueprint mod, and get started on the couple steps you need.

Side note, as you create your assets, if you are adding more than one avatar, I'd recommend using subfolders for each one; the organization doesn't matter, so you can put things wherever you want within the Content folder. :)

### Importing your Model into Unreal

You're going to want to do the following for both your first person and third person models (although ideally they can share the same textures):

1. Drag your fbx into the editor, and import it with default settings (**todo**: maybe..?)
1. Unreal will create a Skeletal Mesh, a Skeleton, and a PhysicsAsset. We will leave the latter two of these alone.
1. Right Click the skeletal mesh, and click "Assign Skeleton"
1. For the first-person model, look for `1PCharacter_Skeleton`; for the third-person model, look for `Character_Skeleton`. Assign this to your skeletal mesh.
   - You may want to watch out for any errors in this step; it may lead you to realize if any of your bones are missing. However it also might not tell you all of them, so check the troubleshooting section if you have problems.
1. Now import and set up your textures. You're going to have to create unreal materials and set them up to your liking, dragging in your textures as needed. Normal 3D modelling stuff, unreal edition.

### Registering your Model as an in-game Avatar

Once you've imported your model, assigned the skeleton, and recreated the textures, we can get on to the CharacterReplacer-specific stuff that does the magic.

1. Download/Clone this repository source as a mod next to your new mod, and restart the editor to load the classes and data.
1. For each Avatar you want to add...
   1. Create a new Misc/Data Asset in your Content folder, and parent it to the `AvatarDefinition` type.
   1. Open the asset, and fill in the fields; drag in or select your skeletal mesh asset for first and third person, and select "keep helmet" only if you want the base pioneer's head to be rendered in addition to your mesh... for some reason. The head is a separate, static mesh in the game, normally, but doesn't have to be for you. It might behave a little funky, though.
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

### Problems with the Model behaving in game

If the model shows up in game, but it looks weird, behaves weird, etc, then you most likely messed up something either:
- during the re-rigging process in blender / your modelling software of choice, or
- while importing the model into unreal.

If interaction is behaving weirdly, and items are not dropping on the ground, and the model is scaled way up in the editor... make sure it isn't scaled way up. Sometimes while getting the model, it ends up at 100x scale, so you'll need to scale it back down either while exporting the fbx or importing it to unreal.

If the model is broken, or doesn't show up, or doesn't move or animate, try making sure all the bones match exactly as expected, and that your model *does have* all the bones of the actual pioneer model. Certain methods of obtaining the base model from the game may end up with some socket bones or other bones missing, so double check against the actual skeleton in unreal if possible.

On that note, when assigning the original skeleton to your custom mesh, keep an eye out for anything red or off or missing in the bone list during assignment.

Note that extra bones on your avatar are not simulated at the moment... until I figure out how to make it work in game. If you know about unreal physics assets in game, and have magic to make it work, feel free to give it a go, and tell me if you have any success!

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
