import pygame
import random
import tkinter
from tkinter import messagebox

# Nastavení
pygame.init()
pygame.display.set_caption("Pong")
tkinter.Tk().wm_withdraw() # schová hlavní okno tkinter

# Konstanty
OKNO_SIRKA = 800
OKNO_VYSKA = 600
MAX_BODY = 1
MIC_ZACATECNI_RYCHLOST = 2
BARVA_BILA = (255, 255, 255)
BARVA_MODRA = (0, 0, 120)

screen = pygame.display.set_mode((OKNO_SIRKA, OKNO_VYSKA))
clock = pygame.time.Clock()
font = pygame.font.Font(None, 100)

# HRÁČI
hrac_vyska = 100
hrac_sirka = 10
hrac_rychlost = 8

# HRÁČ 1
hrac1_x = 50
hrac1_y = 250
hrac1_skore = 0

# HRÁČ 2
hrac2_x = 740
hrac2_y = 250
hrac2_skore = 0

# MÍČ
mic_x = 390
mic_y = 290
mic_velikost = 20
mic_rychlost = MIC_ZACATECNI_RYCHLOST
mic_smer_x = random.randint(0, 1) # 0 - doleva 1 - doprava  
mic_smer_y = random.randint(0, 1) # 0 - dolu   1 - nahoru   


# HLAVNÍ CYKLUS HRY

while True:
    # FPS limit
    clock.tick(60)

    # Nakresli pozadí
    screen.fill(BARVA_MODRA)
    pygame.draw.line(screen, BARVA_BILA, (400, 0), (400, 600), 2) # Půlící čára

    # VSTUP
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()

    # OVLÁDÁNÍ
    keys = pygame.key.get_pressed()

    if keys[pygame.K_w]:
        hrac1_y -= hrac_rychlost
    if keys[pygame.K_s]:   
        hrac1_y += hrac_rychlost
    if keys[pygame.K_UP]: 
        hrac2_y -= hrac_rychlost
    if keys[pygame.K_DOWN]:   
        hrac2_y += hrac_rychlost

    # UPDATE

    # Pohyb míče
    if mic_smer_x:
        mic_x += mic_rychlost
    else:
        mic_x -= mic_rychlost

    if mic_smer_y:
        mic_y -= mic_rychlost
    else:
        mic_y += mic_rychlost

    # Kolize míče s oknem
    if mic_y <= 0:
        mic_smer_y = 0
    if mic_y >= (OKNO_VYSKA - mic_velikost):
        mic_smer_y = 1
    
    # Kolize míče s hráčem
    hrac1_rect = pygame.Rect(hrac1_x, hrac1_y, hrac_sirka, hrac_vyska)
    hrac2_rect = pygame.Rect(hrac2_x, hrac2_y, hrac_sirka, hrac_vyska)
    mic_rect = pygame.Rect(mic_x, mic_y, mic_velikost, mic_velikost)

    if pygame.Rect.colliderect(hrac1_rect, mic_rect):
        mic_smer_x = 1
        mic_rychlost += 0.5
        mic_smer_y = random.randint(0, 1)
    if pygame.Rect.colliderect(hrac2_rect, mic_rect):
        mic_smer_x = 0
        mic_rychlost += 0.5
        mic_smer_y = random.randint(0, 1)

    # Kolize hráče s oknem
    if hrac1_y < 0:
        hrac1_y = 0
    elif hrac1_y > OKNO_VYSKA - hrac_vyska:
        hrac1_y = OKNO_VYSKA - hrac_vyska
    
    if hrac2_y < 0:
        hrac2_y = 0
    elif hrac2_y > OKNO_VYSKA - hrac_vyska:
        hrac2_y = OKNO_VYSKA - hrac_vyska

    # Padl gól? Přidej skóre
    if mic_x > OKNO_SIRKA: # Pravá strana
        hrac1_skore += 1
        # Resetuj hráče
        hrac1_y = 250
        hrac2_y = 250
        # Resetuj míč
        mic_smer_y = random.randint(0, 1)
        mic_rychlost = MIC_ZACATECNI_RYCHLOST
        mic_x = 390
        mic_y = 290
        
    elif mic_x + mic_velikost < 0: # Levá strana
        hrac2_skore += 1
        # Resetuj hráče
        hrac1_y = 250
        hrac2_y = 250
        # Resetuj míč
        mic_smer_y = random.randint(0, 1)
        mic_rychlost = MIC_ZACATECNI_RYCHLOST
        mic_x = 390
        mic_y = 290

    # Zjisti, jestli někdo vyhrál
    if hrac1_skore == MAX_BODY:
        tkinter.messagebox.showinfo("Pong", f"{hrac1_skore} : {hrac2_skore}\nHráč 1 vyhrál!")
        hrac1_skore = 0
        hrac2_skore = 0

    elif hrac2_skore == MAX_BODY:
        tkinter.messagebox.showinfo("Pong", f"{hrac1_skore} : {hrac2_skore}\nHráč 2 vyhrál!")
        hrac1_skore = 0
        hrac2_skore = 0

    # RENDER

    # Nakresli skóre
    text = font.render(str(hrac1_skore), True, BARVA_BILA)
    screen.blit(text, (180, 50))
    text = font.render(str(hrac2_skore), True, BARVA_BILA)
    screen.blit(text, (585, 50))

    # Nakresli hráče
    pygame.draw.rect(screen, BARVA_BILA, pygame.Rect(hrac1_x, hrac1_y, hrac_sirka, hrac_vyska))
    pygame.draw.rect(screen, BARVA_BILA, pygame.Rect(hrac2_x, hrac2_y, hrac_sirka, hrac_vyska))

    # Nakresli míč
    pygame.draw.rect(screen, BARVA_BILA, pygame.Rect(mic_x, mic_y, mic_velikost, mic_velikost))

    pygame.display.update()

pygame.quit()