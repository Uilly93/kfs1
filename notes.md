format d'un Caractère VGA:
16 bits:
┌─────────────┬──────────────┐
│  8 bits     │   8 bits     │
│  Couleur    │   Caractère  │
└─────────────┴──────────────┘
  Octet haut    Octet bas

Ou en détail : 
┌────┬────┬─────────┐
│ 4  │ 4  │    8    │
│Fond│Text│ ASCII   │
└────┴────┴─────────┘

format de l'octet de couleur :
Bit:  7  6  5  4    3  2  1  0
     ┌──┬──┬──┬──┐ ┌──┬──┬──┬──┐
     │ B│ R│ G│ B│ │ I│ R│ G│ B│
     └──┴──┴──┴──┘ └──┴──┴──┴──┘
      Fond (BG)      Texte (FG)

B = Blink (clignotement) ou Bright background
R = Rouge
G = Vert  
B = Bleu
I = Intensité (bright)

```c

uint16_t caractere = (color << 8) | char_ASCII;

uint8_t color = (couleur_du_fond << 4) | couleur_du_texte;
```
