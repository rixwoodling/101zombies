#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# 101 Zombies v1.0 / 2019 rixwoodling.ninja
# (c) rix woodling
# .........................................

import random

# define game functions

def help():
    h1 = "\n" + "101 ZOMBIES\n"
    h2 = "Choose to FIGHT or RUN away\n"
    h3 = "from the zombie horde! Chance\n"
    h4 = "for new weapons when fighting.\n"
    h5 = "However, consecutive fighting\n"
    h6 = "increases zombie damage. Chance\n"
    h7 = "for medikits when running. How-\n"
    h8 = "ever, constant running is for\n"
    h9 = "weak little girls! Good luck.\n"
    return (h1+h2+h3+h4+h5+h6+h7+h8+h9)

def prob():
    player = (((health)+(w*10))/20)
    enemy = ((zombies*5)/10)
    try:
        roll = (100 * float(player)/float(player+enemy))
    except:
        roll = 0
    y = (int(roll/10))
    return(float(roll))
    
def xhealth():
    if health > 100: return 100
    elif health < 0: return 0
    else: return health

def pain():
    return(random.randint(1,15))

def medikit():
    if random.randint(1,100) > 30:
        sizes = [5, 10]
        size = (random.choice(sizes))
        print("Medikit found! +" + str(size))
        return(size)
    else: 
        return 0

def prompt():
    game = input("You (f)ight or (r)un? ")
    if game == "f": 
        attack = 0
        path = random.randint(1,100)
        if path < prob():
            return 1
        if path > prob():
            return 2
    elif game == "r":
        path = random.randint(1,100)
        if path > 49:
            return 3
        if path < 50:
            return 4
    elif game != "":
        return(prompt())
    elif game == "":
        return(prompt())
        
# game setup        

health = 100
horde = 101
w = 0
fatigue = 0
zombies = 0
weapon = ["a machete","a chainsaw",
            "a shotgun","an AK-47",
            "a minigun","a flamethrower",
            "double flamethrowers",
            "the death ray"]
emo = ("dead","very fucked up","fucked up",
        "like total shit","like shit","wounded",
        "banged up","alright","great", 
        "fuckin’ great","godlike")
luck = ["you got no chance in hell.",
        "odds are slim.","you’d better run.",
        "it’s dicey at best.",
        "it’ll be a tough fight.",
        "it’s a toss up.","you’ve seen worse.",
        "the odds are with you.",
        "this shouldn’t be a problem.","easy.",
        "you can’t lose!"]

print(help())

# main game loop
while True:

    print(horde, end=' ')

    # zombie dots
    z = random.randint(1,w+10)
    zombies = z

    if horde >= zombies:
        pass
    else:
        zombies = horde
    print("." * zombies)

    # zombies text
    if zombies == 1:
        print("A lone zombie approaches.")
    else:
        print("%s zombies approach." % zombies)

    # health text
    checkup = emo[int(xhealth()/10)]
    print("You’re feeling %s." % (checkup))
    
    # weapon text
    print("Armed with %s," % (weapon[w]))
    
    # probability formula
    y = (int(prob()/10))
    print(luck[y])
    
    # fight prompt
    fight = prompt()
    if fight == 1: 
        fatigue = fatigue + 1.5
        print("*" * zombies)
        horde = horde - zombies
        print("\bYou fight and win!")
        if random.randint(1,100) > 70:
            if w < 7:
                w = w + 1
                print("You find %s! +%s" % (weapon[w], str(w)))
            else:
                pass
        else: pass

    elif fight == 2:
        fatigue = fatigue + 2.5
        health = (health - (pain() + fatigue))
        print("*" * zombies)
        horde = horde - zombies
        if health <= 0:
            pass
        else:
            print("\bZombies do some damage")
            print("but you kill ‘em all!")
        t = xhealth()
    elif fight == 3:
        attack = 0
        print("~" * zombies)
        print("\bYou retreat unscathed!")
        health = (health + medikit())
        t = xhealth()
    elif fight == 4:
        attack = 0
        health = (health - pain())
        if health <= 0:
            pass
        else:
            print("~" * zombies)
            print("\bYou barely get out alive!")
        t = xhealth()
    
    print("\b")

    # ending screens

    if horde == 0:
        print("Few ever make it to the end.")
        print("But you slaughtered them all.")
        print("You are a fucking HERO!\n")
        break
    elif health <= 0:
        print("Unfortunately, your wounds")
        print("bring you down. And now you")
        print("turn into a zombie!")
        print("GAME OVER!\n")
        break
    else:
        pass
