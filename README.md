# Langtons ant
**Langtons ant simulation made in C using SDL2.**

Based on [r4v10l1/LangtonsAnt](https://github.com/r4v10l1/LangtonsAnt).

---

## Possible settings

### General

Setting         | Description                                                  | Default value
----------------|--------------------------------------------------------------|----------------
`WINDOW_W`      | Window width                                                 | 1200
`WINDOW_H`      | Window height                                                | 750
`FPS`           | Frames per seccond when space is not pressed                 | 750
`DELAY`         | Time that the program will wait when holding the space (ms)  | 50
`DEBUG_PRINT`   | Will display debug information if **1**                      | 0

### Ant

Setting         | Description                                                     | Default value
----------------|-----------------------------------------------------------------|----------------
`COLOR_NUMBER`  | Number of color asociations that the program will use (Max 9)   | 2
`BACKGROUND_R`  | Red value used for the background                               | 0
`BACKGROUND_G`  | Green value used for the background                             | 0
`BACKGROUND_B`  | Blue value used for the background                              | 0

### Possible colors
Possible colors for the configuration file. Example:

```c
color_0 = 1     // Will set color 0 (FIRST) to Red
color_1 = 3             // Will set color 1 (SECCOND) to Yellow
color_3 = 5             // Will set color 3 to Light Blue
```

Code  | Color       | RGB value
------|-------------|-------------
**1** | Red         | (255,0,0)
**2** | Orange      | (255,165,0)
**3** | Yellow      | (255,255,0)
**4** | Green       | (0,255,0)
**5** | Light Blue  | (112,163,204)
**6** | Dark Blue   | (0,0,255)
**7** | Purple      | (160,32,240)
