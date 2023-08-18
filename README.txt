LINK TO VIDEO:
https://drive.google.com/file/d/1iEtavBgoKU3BPtl0Tx_01x64fol7QDIC/view?usp=sharing



WARNING
the game is a bit heavy considering all the objects that are moving. Please close other programs to make sure that the game runs smoothly without lag



HOW TO RUN THE PROGRAM

in the windows edition of the original code base, only 1 file (main.cpp) was present. For the sake of modularity "main.cpp" was split into "lasso.h", "movingObject.h", "coin.h", "gameVariables.h" and "main.cpp"

to run the code, place all the above files in one folder, build and run "main.cpp"



GAME RULES

You are a cowboy in the wild west. As you traverse across the lonely wastelands you stumble across some mysterious swamp from which coins fly out. Being direly in need of money you use your lasso to catch these coins, and hopefully catch enough to buy your favourite brand of beer at the local bar

There are 3 types of objects
1. coin (yellow)
2. bomb (red)
3. magnet (blue)

Capturing a coin will increase your score by 1, and letting it fall will decrease score by 1.

For every 5 coins you capture, your level increases (max 5 levels). With an increase in level, the number of coins and bombs increases. 

At the start of every level a magnet is released.

You have 3 lives at the start of the game. With each bomb you capture, 1 life is taken away. 
When all 3 lives have been exhausted, your game is over and you get the option to save your score on "scores.txt" (file will be created if it does not exist)



KEY CONTROLS

w - throw lasso
s - yank lasso

d - move the rod to the right
a - move the rod to the left

SPACE - to open the loop of the lasso
SPACE (again) - to close the loop of the lasso, if the loop is open

] - increase speed
[ - decrease speed



IMPROVEMENTS MADE
1. Coins start from random positions along x-axis and follow random parabolic paths
2. Multiple coins
3. 5 Levels with increasing difficulty
4. Multiple bombs and a life system (3 lives)
5. Magnets to attract coins for a brief period of time
6. File system to add player's score

7. Aesthetic Changes: 
changed the speed and gravity, 
changed colours (added sky and grass), 
made objects bounce off walls, 
changed the mechanics of lasso looping, 
added a legend which tells which object has which color,
text box with Magnet ON & Magnet OFF
