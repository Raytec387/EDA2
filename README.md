The project name : Red rain
Author: Rui Lin, Zihao Zhou, Marti Melcio

Red Rain is a Role Playing Game, this game uses the C programming language, and it executes the codes in the terminal. The game story will be based on a self-created story called Red Rain. The background is in Middle Ages.

The characteristic of the game:

Character creation:	Each character has a value of ATK, DEF, HP_LIMIT,a bool check if it is a player,name, hp(used in the battle, the character dies when hp = 0) and the array of the skills. Where the skill has value, cooldown, etc.

Open world: The game world is structured as a Scenario Graph. Using the console and graph data, Each scenario has a segment of the story and involves some form of battle or decision-making. Upon completing a scenario (by winning all battles within it), the player can move to any of the adjacent scenarios in the graph.

Scenario: Each scenario shows a main text that presents part of the story and asks the player to make a decision. This decision will decide how many enemies there are(1 or 3) during the battle, the player will face in battle immediately after.

Battle system: Turn-based combat system, queue-based combat system, each character's turn will be stored in the queue. Once the character’s turn reaches, character can do a manipulation, for user he can use a skill or basic atk, for enemies they will apply the skill randomly.

Exit game: Enter Ctrl + c in the terminal.

The library used in the project: cJSON.h and cJSON.c, 
the original author:	Dave Gamble         URL of the cJSON: https://github.com/DaveGamble/cJSON


	




The first session 24/04/2024 - 08/05/2024:
  1. We create a branch called character in Github, and in there we will upload files such as common.h, character.h, etc.
  In character.h, we implement the data structure of the main character, the Skill of the main character, and the enemy.

  2. The game story will be based on a self-created story called Red Rain. The background is in Middle Ages.
