One Hundred and One Zombies v1.0
2019 / rixwoodling.ninja
creativecommons.org/licenses/BY-NC-ND/4.0

INSTALLATION
git clone https://github.com/rixwoodling/one-hundred-and-one-zombies.git

TO PLAY
# This game only will work with python v2. 
# If there is a screaming need for it, i'll port to python v3. But until then... 
# type this in the command-line:
python 101zombies.py

ABOUT
Are you tough enough to fight through a horde of one-hundred-and-one zombies? 

The point of this text-based action game is to slaughter all zombies while staying alive. The zombies come in groups of varying sizes and you will decide either to FIGHT or to RUN away from each group you encounter. Both fighting and running have benefits and drawbacks. A good deal of this game is based on luck, but knowing when to fight and when to run will increase your chances of making it to the end. 

GAMEPLAY
The game starts off with a fresh horde of 101 zombies, your health at maximum, and a machete as a weapon. Each round provides just enough information to help you decide whether the number of zombies you face calls for an offensive or defensive strategy.
Below, is an example of one round of gameplay. The number '66' indicates the total amount of zombies currently left in the horde. The five dots '.....', as well as '5 zombies approach' indicates how many zombies you face this round. The narration also indicates which weapon you are using, and what your odds are to fighting without incurring damage.

66 .....
5 zombies approach.
You're feeling alright.
Armed with a shotgun,
you've seen worse.
You (f)ight or (r)un? f
*****
Zombies do some damage,
but you kill them all!

In this round, the decision to fight was made. All zombies were slaughtered extremely graphically, as indicated by the five asterisks '*****'. Finally, the message "Zombies do some damage, but you kill them all!" means that while your odds of fighting without damage were good, the zombies still managed to cause damage to you in the melee.

FIGHTING
Fighting reduces the number of zombies in the horde. Fighting either produces a win, which is killing all zombies in the round without incurring damage to yourself, or produces a loss, which is killing all zombies in the round but incurring damage to yourself. In either scenario, all of the zombies in the round are counted as killed and are subtracted from the remainder of zombies left in the horde. 

Winning the fight without incurring damage is the best outcome you can have in the game. Not only is your health level left intact, but there is an added bonus of weapon upgrades. Weapon upgrades occur randomly but only after winning a fight. Each upgrade provides a better and stronger weapon than the one before. As a technical note, each weapon upgrade that occurs also increases the maximum number of zombies possible within a round. It should also be noted that fatigue still accrues but not as much as when losing a fight.

Losing the fight can be both good and bad. The only benefit from losing a fight is you reduce the same amount of zombies as if you had won the fight without incurring damage. But your health takes a ding, there are no chances for weapon upgrades for that round, and your fatigue accrual is stronger.

RUNNING
Running provides the necessary counterbalance to fighting when groups of zombies are too large or when health is critically low. Like fighting, running also has two possible outcomes. Both outcomes however immediately resets fatigue levels, which have accrued while fighting. Running does not kill any zombies and the total number of zombies in the horde remains the same into the next round.

Running away without incurring damage is the second best outcome you can have in the game. In addition to what has already been said about the benefits of running, a chance for Medikits, like weapons, also randomly drop if you do not incur damage.  
Medikits immediately heal you after a round in doses of either +5 or +10. Acquiring medikits are the only way regain health.

Running away and incurring damage is likely the worst outcome of the game as well as the most humiliating. While fatigue levels still return to normal, there are no medikit drops and your health takes a ding. To be fair though, losing a fight can actually deliver a more serious ding to health than incurring damage while running away when you consider the added effects of fatigue.

FAQ
Q: Shouldn't i always run from large groups of zombies? 
A: Sometimes fighting a large group of zombies against the odds pays off, especially early in the game rather than later when your health is lower. With that said, the more time you spend on the battlefield, the more likely your health will drain.

Q: What if i just fight and never run?
A: The odds of winning with this strategy are likely impossible. Again, fatigue accrues after each consecutive fight, which then gets added to what you incur as damage, which ultimately means massive health losses!

Q: What if i just run and never fight?
A: You will likely be kept alive indefinitely by using up medipacks meant for real heroes that fight! 

Q: Hey, where are the 3D graphics?
A: You get to use the fastest, and goriest, GPU available: your imagination.

Anyway, that's about it. Now, go kill some zombies!

Rix

