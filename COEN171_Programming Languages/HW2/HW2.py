#!/usr/bin/python

from Tkinter import *
from random import randint
from time import sleep,time
from math import sqrt


H = 600
W = 800
scorePoint = 0
NUM_MINES = 10
BUB_CHANCE = 10
SHIP_RADIUS = 30;
SHIP_LIFE = 4;
GAP = 100
GAMEOVER = False

window = Tk()
window.title("Submarine Game")
c = Canvas(window, width = W, height = H, bg ="#CAE1FF")
p=PhotoImage(file="water.gif")
c.create_image(0,0, image=p)
c.pack()

parts = list()
#Draw a red submarine
ship_h = c.create_polygon(370,415,425,400,425,430,outline= "black", fill="red") 
ship_c = c.create_oval(300,400,400,430,outline="black",fill="red")
ship_p = c.create_rectangle(350,387,360,400,outline= "black",fill="red")
ship_p2 = c.create_rectangle(340,387,350,390,outline= "black",fill="red")
parts.append(ship_h)
parts.append(ship_p)
parts.append(ship_p2)

#move the submarine
#move up
SHIP_SPD = 10
def move_ship_up(event):
	x1,y1,x2,y2 = c.coords(ship_c)
	if y1 - SHIP_SPD > 0 and not GAMEOVER:
		c.move(ship_c,0,-SHIP_SPD)
		for x in range(len(parts)):
			c.move(parts[x],0,-SHIP_SPD)
window.bind("<Up>",move_ship_up)

#move down
SHIP_SPD1 = -10
def move_ship_down(event):
	x1,y1,x2,y2 = c.coords(ship_c)
	if y2 - SHIP_SPD < 640 and not GAMEOVER:
		c.move(ship_c,0,-SHIP_SPD1)
		for x in range(len(parts)):
			c.move(parts[x],0,-SHIP_SPD1)
window.bind("<Down>",move_ship_down)

#move right
SHIP_SPD2 = 0
def move_ship_right(event):
	x1,y1,x2,y2 = c.coords(ship_c)
	if x1 - SHIP_SPD1 < 690 and not GAMEOVER:
		c.move(ship_c,10,-SHIP_SPD2)
		for x in range(len(parts)):
			c.move(parts[x],10,-SHIP_SPD2)
window.bind("<Right>",move_ship_right)

#move left
SHIP_SPD3 = 0
def move_ship_left(event):
	x1,y1,x2,y2 = c.coords(ship_c)
	if x1 - SHIP_SPD1 > 10 and not GAMEOVER:
		c.move(ship_c,-10,-SHIP_SPD3)
		for x in range(len(parts)):
			c.move(parts[x],-10,-SHIP_SPD3)
window.bind("<Left>",move_ship_left)

bubble_id=list()
bubble_r=list()
bubble_speed=list()

def create_mines():
	xCoord = randint(80,730)
	yCoord = 640
	radius = randint(10,30)
	speed = randint(10,30)
	bubble = c.create_oval(xCoord,yCoord,xCoord+2*radius,yCoord+2*radius, outline="blue")
	bubble_r.append(radius)
	bubble_speed.append(speed) 
	bubble_id.append(bubble)
	
def move_mines():
	for x in range(len(bubble_id)):
		c.move(bubble_id[x],0, -bubble_speed[x])
		
def delBubbles(index):
	del bubble_r[index]
	del bubble_speed[index]
	c.delete(bubble_id[index])
	del bubble_id[index]

def getCenter(id):
	pos = c.coords(id)
	centerX = (pos[0] + pos[2])/2
	centerY = (pos[1] + pos[3])/2
	return centerX, centerY
	
def distance(bubble_b):
	x1,y1 = getCenter(ship_c)
	x2,y2 = getCenter(bubble_b)
	
	distance = sqrt((x2-x1)**2 + (y2-y1)**2)
	return distance

def collision():
	for x in range(len(bubble_id)-1,-1,-1):
		d = distance(bubble_id[x]);
		b = SHIP_RADIUS + bubble_r[x]
		if d < b:
			delBubbles(x)
			global SHIP_LIFE
			SHIP_LIFE-=1
			updateShipLife()
			
def clean_up_bubbles():
	for id in range(len(bubble_id)-1,-1,-1):
		x,y = getCenter(bubble_id[id])
		if y < -GAP:
			delBubbles(id)
			
def updateShipLife():
	if SHIP_LIFE == 0:
		return
	if SHIP_LIFE == 3:
		c.delete(parts[2])
		del parts[2]
	if SHIP_LIFE == 2:
		c.delete(parts[1])
		del parts[1]
	if SHIP_LIFE == 1:
		c.delete(parts[0])
		del parts[0]
		
c.create_text(150,30,text="Score",fill="black",font=('Comic Sans',30))
score_text = c.create_text(150,55,fill="black")

def show_score(score):
	c.itemconfig(score_text,text=str(score),font=('Comic Sans',30))

def finalScore(score):
	c.create_text(W/2, H/2, text='GAME OVER, YOUR SHIP EXPLODED!', fill='white',
		font=('Comic Sans', 40))
	c.create_text(W/2, H/2 + 30, text='Score: ' + str(score),
		fill='white', font=('Comic Sans', 30))

while SHIP_LIFE > 0:
	move_mines()
	clean_up_bubbles()
	if scorePoint > 500 or scorePoint > 1000 or scorePoint > 2000:
		BUB_CHANCE/=2
	if randint(1,BUB_CHANCE) == 1:
		create_mines()
	#clean_up_mines()
	collision()
	scorePoint+=1
	show_score(scorePoint)
	window.update()
	sleep(0.01)
	
GAMEOVER = True
finalScore(scorePoint)

window.mainloop()