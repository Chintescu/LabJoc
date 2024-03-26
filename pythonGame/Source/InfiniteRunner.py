import random
import pygame
import threading
import serial
import time
pygame.init()

lock = threading.Lock()
data = None

def set_data(incomingData):
    global data
    with lock:
        data = incomingData

def get_data():
    global data
    with lock:
        return data

# arduino conection
def arduino_serial():
    arduinoSerial = serial.Serial('COM7', 9600)
    while True:
        incomingData = str(arduinoSerial.readline(), 'UTF-8')
        if 'jump' in incomingData:
            set_data(incomingData)

# game constants
white = (255,255,255)
black = (0,0,0)
green = (0,255,0)
red = (255,0,0)
orange = (255, 165,0)
yellow = (255, 255 ,0)
WIDTH = 450
HEIGHT = 300

#game variables
score = 0;
player_x = 50
player_y = 200
y_change = 0
gravity = 0.7
x_change = 0
obstacles = [480,450,600]
obstacle_speed = 2
active = False
speed_change = True

screen = pygame.display.set_mode([WIDTH, HEIGHT])
pygame.display.set_caption('Infinite Runner')
background = black
fps = 60
font = pygame.font.Font('freesansbold.ttf',16)
timer = pygame.time.Clock()

running = True
thread1 = threading.Thread(target=arduino_serial)
thread1.start()
while running:
    timer.tick(fps)
    screen.fill(background)
   # incoming_data = str(Arduino_Serial.readline(), 'UTF-8')
    if not active:
        instruction_text = font.render(f'Space bar to Start', True, white, black)
        screen.blit(instruction_text, (140,50))
        instruction_text2 = font.render(f'Space bar Jumps', True, white, black)
        screen.blit(instruction_text2, (140,90))
    
    score_text = font.render(f'Score: {score}', True, white, black)
    screen.blit(score_text, (160,250))
    floor = pygame.draw.rect(screen, white, [0,220,WIDTH,5])
    player = pygame.draw.rect(screen, green, [player_x, player_y, 20,20])
    obstacle0 = pygame.draw.rect(screen, red, [obstacles[0] , 200, 20, 20])
    obstacle1 = pygame.draw.rect(screen, orange, [obstacles[1] , 200, 20, 20])
    obstacle2 = pygame.draw.rect(screen, yellow, [obstacles[2] , 200, 20, 20])
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN and not active:
            if event.key == pygame.K_SPACE:
                obstacles = [480,600,700]
                player_x = 50
                score = 0
                active = True
                obstacle_speed = 2;
        if event.type == pygame.KEYDOWN and active:
            if event.key == pygame.K_SPACE and y_change ==0:
                y_change = 16
                
    sendedData = get_data()
    if sendedData is not None:
        if 'jump' in sendedData:
            if active and y_change ==0:
                y_change = 16
                set_data(None)
            if not active:
                obstacles = [480,600,700]
                player_x = 50
                score = 0
                active = True
                obstacle_speed =2
                set_data(None)
  
    
   # incoming_data = "not"
    for i in range(len(obstacles)):
        if active:
            obstacles[i] -= obstacle_speed
            if obstacles[i] < -20:
                obstacles[i] = random.randint(520,820)
                score +=1
                if score %10 == 0:
                    speed_change = True
            if player.colliderect(obstacle0) or player.colliderect(obstacle1) or player.colliderect(obstacle2):
                active = False
            
    if score%10 == 0 and speed_change:
        speed_change = False
        obstacle_speed +=1
    
    if y_change >0 or player_y < 200:
        player_y -=y_change
        y_change -= gravity
    if player_y > 200:
        player_y = 200
    if player_y == 200 and y_change <0:
        y_change = 0
        
    pygame.display.flip()
pygame.quit()