The project name : Red rain
Author: Rui Lin, Zihao Zhou, Marti Melcio

Red Rain is a Role Playing Game, this game uses the C programming language, and it executes the codes in the terminal. The game story will be based on a self-created story called Red Rain. The background is in Middle Ages.

The characteristic of the game:

Character creation:	Each character has a value of ATK, DEF, HP_LIMIT,a bool check if it is a player,name, hp(used in the battle, the character dies when hp = 0) and the array of the skills. Where the skill has value, cooldown, etc.

Open world: The game world is structured as a Scenario Graph. Using the console and graph data, Each scenario has a segment of the story and involves some form of battle or decision-making. Upon completing a scenario (by winning all battles within it), the player can move to any of the adjacent scenarios in the graph.

Scenario: Each scenario shows a main text that presents part of the story and asks the player to make a decision. This decision will decide how many enemies there are(1 or 3) during the battle, the player will face in battle immediately after. Harder battle will increase the atk,def,hp_limit of the main character.

Battle system: Turn-based combat system, queue-based combat system, each character's turn will be stored in the queue. Once the character’s turn reaches, character can do a manipulation, for user he can use a skill or basic atk, for enemies they will apply the skill randomly.

Start the game: game.exe to start,if there is no game.exe compile the code in the terminal with the following command:
gcc skill.c scenario.c common.c cJSON.c character.c game.c main.c combat.c  -o game -Wall
to download gcc:  https://sourceforge.net/projects/gcc-win64/

Exit game: Enter Ctrl + c in the terminal.

Available option in the menu:
0.-Go back:finish the game, or go back the previous step
1.-Start a new game:start the game
2.-Load Game: load the game if you have saved the game
3.-Save Game: save the game
4.-Change the skill of main character: Change the skill
5.-Show how many times is the skill used: Show the used skill and the counts of the usage

Combat:
1.-Basic atack: damage based on the atk of the main charcter
2.-Use a skill, you can check the skill and its description once you click "Change the skill of main character", each skill holds different cooldown.
3.-Time Strike : Ultimate skill, only can use once in each battle, randomly choose a used skill in the battle, the value of the skill will be doubled.

The library used in the project: cJSON.h and cJSON.c, 
the original author:	Dave Gamble         URL of the cJSON: https://github.com/DaveGamble/cJSON


	




The first session 24/04/2024 - 08/05/2024:
  1. We create a branch called character in Github, and in there we will upload files such as common.h, character.h, etc.
  In character.h, we implement the data structure of the main character, the Skill of the main character, and the enemy.

  2. The game story will be based on a self-created story called Red Rain. The background is in Middle Ages.
