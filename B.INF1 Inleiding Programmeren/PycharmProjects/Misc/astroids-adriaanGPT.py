import pygame
import random
import math
import sys
import os
import pygame

# Initialize Pygame
pygame.init()

# Screen dimensions
WIDTH, HEIGHT = 600, 800
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("PyInvaders")

# Load or initialize high score
if os.path.exists("highscore.txt"):
    with open("highscore.txt", "r") as f:
        high_score = int(f.read())
else:
    high_score = 0

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 50, 50)
GREEN = (50, 255, 50)
BLUE = (50, 50, 255)
YELLOW = (255, 255, 0)
ORANGE = (255, 165, 0)
PURPLE = (150, 0, 200)
CYAN = (0, 255, 255)
MAGENTA = (255, 0, 255)
ENEMY_COLORS = [(200, 50, 50), (50, 200, 50), (50, 50, 200), (200, 200, 50)]
PLAYER_COLOR = (50, 200, 50)
BULLET_COLOR = (255, 255, 0)
BOSS_COLOR = (150, 0, 200)

# Game variables
clock = pygame.time.Clock()
FPS = 60
running = True
game_state = "menu"  # Can be 'menu', 'playing', 'paused', 'game_over'
score = 0
coins = 0
shield_active = False
lives = 10
level = 1
font = pygame.font.SysFont(None, 36)
aimbot_level = 0  # Aimbot upgrade level

# Player properties
player_scale = 4  # Increased scale for larger characters
player_pixels = [
    "........",
    "...XX...",
    "..XXXX..",
    ".XXXXXX.",
    "XXXXXXXX",
    "XX.XX.XX",
    "XXXXXXXX",
    ".X.XX.X.",
    "........"
]
player_size = (len(player_pixels[0]) * player_scale, len(player_pixels) * player_scale)
player_pos = [WIDTH // 2 - player_size[0] // 2, HEIGHT - 100]
player_speed = 5

# Enemy properties
enemy_types = {
    'basic': {
        'pixels': [
            "........",
            "..XXXX..",
            ".XXXXXX.",
            "XX.XX.XX",
            "XXXXXXXX",
            "..X..X..",
            ".X.X.X.X",
            "........"
        ],
        'color': ENEMY_COLORS[0],
        'speed': 2,
        'score': 10,
        'coins': 5
    },
    'fast': {
        'pixels': [
            "........",
            ".XXXXXX.",
            "XX....XX",
            "X.XX.XX.",
            "X.XX.XX.",
            "XX....XX",
            ".XXXXXX.",
            "........"
        ],
        'color': ENEMY_COLORS[1],
        'speed': 3,
        'score': 15,
        'coins': 7
    },
    'strong': {
        'pixels': [
            "........",
            "..XXXX..",
            ".XXXXXX.",
            "XX.XX.XX",
            "XXXXXXXX",
            "XX.XX.XX",
            "XXXXXXXX",
            "........"
        ],
        'color': ENEMY_COLORS[2],
        'speed': 1,
        'score': 20,
        'coins': 10,
        'health': 2  # Requires two hits
    }
}
enemy_size = None  # Will be set later
enemies = []
enemy_rows = 5
enemy_cols = 10
enemy_padding = 20
enemy_direction = 1  # 1 for right, -1 for left

# Boss properties
boss_fight = False
boss_defeated = False
boss_scale = 5
boss_pixels = [
    "..........",
    "...XXXX...",
    "..XXXXXX..",
    ".XX.XX.XX.",
    "XXXXXXXXXX",
    "XX.XXXX.XX",
    "XXXXXXXXXX",
    ".X.XXXX.X.",
    ".........."
]
boss_size = (len(boss_pixels[0]) * boss_scale, len(boss_pixels) * boss_scale)
boss_pos = [WIDTH // 2 - boss_size[0] // 2, 50]
boss_speed = 3
boss_health = 50  # Boss health
boss_direction = 1
boss_surface = None  # Will be created later

# Bullet properties
bullet_size = (5, 10)
bullets = []
bullet_speed = -7  # Player bullet speed (negative value moves upwards)
enemy_bullets = []
enemy_bullet_speed = 5  # Enemy bullet speed
boss_bullets = []
boss_bullet_speed = 6  # Boss bullet speed
shoot_cooldown = 500  # milliseconds
last_shot = pygame.time.get_ticks()

# Shop variables
shop_items = {
    '1': {'name': 'Increase Bullet Speed', 'cost': 20},
    '2': {'name': 'Increase Player Speed', 'cost': 30},
    '3': {'name': 'Shield', 'cost': 50},
    '4': {'name': 'Extra Life', 'cost': 100},
    '5': {'name': 'Aimbot', 'cost': 150 + aimbot_level * 100}
}
shop_cooldown = 500  # milliseconds
last_shop = pygame.time.get_ticks()

# Power-up variables
power_ups = []
score_multiplier = 1  # For power-up 'double_points'
power_up_duration = 5000  # milliseconds
active_power_ups = {}

# Explosion animations
explosions = []
explosion_images = []
for i in range(9):
    img = pygame.Surface((64, 64), pygame.SRCALPHA)
    pygame.draw.circle(img, (255, 150 + i * 10, 0), (32, 32), 32 - i * 3)
    explosion_images.append(img)

# Enemy firing variables
enemy_fire_interval = 1000  # milliseconds
last_enemy_fire = pygame.time.get_ticks()

# Pixel art definitions
def create_pixel_art_surface(pixel_data, color, scale=2):
    """Creates a Pygame Surface from pixel data."""
    width = len(pixel_data[0])
    height = len(pixel_data)
    surface = pygame.Surface((width * scale, height * scale), pygame.SRCALPHA)
    for y, row in enumerate(pixel_data):
        for x, pixel in enumerate(row):
            if pixel == 'X':
                rect = pygame.Rect(x * scale, y * scale, scale, scale)
                surface.fill(color, rect)
    return surface

# Player surface
player_surface = create_pixel_art_surface(player_pixels, PLAYER_COLOR, scale=player_scale)

# Enemy surfaces
for enemy_type in enemy_types.values():
    enemy_type['surface'] = create_pixel_art_surface(enemy_type['pixels'], enemy_type['color'], scale=4)
enemy_size = enemy_types['basic']['surface'].get_size()

# Boss surface
boss_surface = create_pixel_art_surface(boss_pixels, BOSS_COLOR, scale=boss_scale)

# Power-up pixel art
def get_power_up_surface(power_up_type):
    if power_up_type == 'rapid_fire':
        color = CYAN
        symbol = [
            "..X..",
            "..X..",
            "XXXXX",
            "..X..",
            "..X.."
        ]
    elif power_up_type == 'double_points':
        color = ORANGE
        symbol = [
            "XXXXX",
            "X...X",
            "..X..",
            ".X...",
            "XXXXX"
        ]
    elif power_up_type == 'invincibility':
        color = YELLOW
        symbol = [
            ".XXX.",
            "X...X",
            "X.X.X",
            "X...X",
            ".XXX."
        ]
    elif power_up_type == 'speed_boost':
        color = MAGENTA
        symbol = [
            "X....",
            "XX...",
            "XXX..",
            "XXXX.",
            "XXXXX"
        ]
    return create_pixel_art_surface(symbol, color, scale=3)

def create_enemies():
    global enemies
    enemies = []
    for row in range(enemy_rows):
        for col in range(enemy_cols):
            x = col * (enemy_size[0] + enemy_padding) + 50
            y = row * (enemy_size[1] + enemy_padding) + 50
            # Randomly select enemy type
            enemy_type = random.choice(list(enemy_types.keys()))
            enemy_info = enemy_types[enemy_type]
            rect = pygame.Rect(x, y, *enemy_size)
            enemies.append({'rect': rect, 'type': enemy_type, 'health': enemy_info.get('health', 1)})

def display_menu():
    menu_open = True
    while menu_open:
        screen.fill(BLACK)
        title = font.render("Welcome to PyInvaders", True, WHITE)
        start_text = font.render("Press ENTER to Start", True, WHITE)
        high_score_text = font.render(f"High Score: {high_score}", True, WHITE)
        screen.blit(title, (WIDTH // 2 - title.get_width() // 2, 200))
        screen.blit(start_text, (WIDTH // 2 - start_text.get_width() // 2, 300))
        screen.blit(high_score_text, (WIDTH // 2 - high_score_text.get_width() // 2, 400))
        pygame.display.flip()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                menu_open = False
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RETURN:
                    menu_open = False
                    global game_state
                    game_state = "playing"
        clock.tick(30)

def display_pause_menu():
    pause_open = True
    while pause_open:
        pause_text = font.render("Game Paused", True, WHITE)
        resume_text = font.render("Press P to Resume", True, WHITE)
        screen.blit(pause_text, (WIDTH // 2 - pause_text.get_width() // 2, 300))
        screen.blit(resume_text, (WIDTH // 2 - resume_text.get_width() // 2, 350))
        pygame.display.flip()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pause_open = False
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_p:
                    pause_open = False
                    global game_state
                    game_state = "playing"
        clock.tick(30)

def display_shop():
    global coins, bullet_speed, player_speed, shield_active, lives, last_shot, aimbot_level
    shop_open = True
    while shop_open:
        screen.fill(BLACK)
        title = font.render("Shop - Press Number to Buy | ESC to Exit", True, WHITE)
        coins_text = font.render(f"Coins: {coins}", True, WHITE)
        screen.blit(title, (WIDTH // 2 - title.get_width() // 2, 50))
        screen.blit(coins_text, (WIDTH // 2 - coins_text.get_width() // 2, 100))
        y_offset = 150
        for key, item in shop_items.items():
            if item['name'] == 'Aimbot':
                item_name = f"{item['name']} Level {aimbot_level + 1}"
                item_cost = 150 + aimbot_level * 100
                item_status = ''
            else:
                item_name = item['name']
                item_cost = item['cost']
                item_status = ''
            item_text = font.render(f"{key}. {item_name} - {item_cost} Coins{item_status}", True, WHITE)
            screen.blit(item_text, (WIDTH // 2 - item_text.get_width() // 2, y_offset))
            y_offset += 50
        pygame.display.flip()

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_ESCAPE:
                    shop_open = False
                elif event.unicode in shop_items:
                    purchase_item(shop_items[event.unicode])

        # Cap the frame rate in the shop loop
        clock.tick(30)

    # Reset last_shot after shop closes to prevent shooting delay
    last_shot = pygame.time.get_ticks()

def purchase_item(item):
    global coins
    item_cost = item['cost']
    if item['name'] == 'Aimbot':
        item_cost = 150 + aimbot_level * 100  # Increase cost with level
    if coins >= item_cost:
        coins -= item_cost
        apply_upgrade(item['name'])
    else:
        insufficient_funds()

def insufficient_funds():
    message = font.render("Not enough coins!", True, RED)
    screen.blit(message, (WIDTH // 2 - message.get_width() // 2, HEIGHT - 100))
    pygame.display.flip()
    pygame.time.wait(1000)

def apply_upgrade(upgrade_name):
    global bullet_speed, player_speed, shield_active, lives, aimbot_level
    if upgrade_name == 'Increase Bullet Speed':
        bullet_speed -= 2  # Negative value increases speed upwards
    elif upgrade_name == 'Increase Player Speed':
        player_speed += 2
    elif upgrade_name == 'Shield':
        shield_active = True
    elif upgrade_name == 'Extra Life':
        lives += 1
    elif upgrade_name == 'Aimbot':
        aimbot_level += 1  # Increase aimbot level
        # Update the shop item cost
        shop_items['5']['cost'] = 150 + aimbot_level * 100

def apply_power_up(power_up_type):
    global bullet_speed, score_multiplier, shoot_cooldown, active_power_ups, shield_active, player_speed
    if power_up_type == 'rapid_fire':
        shoot_cooldown = 200  # Decrease cooldown
        active_power_ups['rapid_fire'] = pygame.time.get_ticks()
    elif power_up_type == 'double_points':
        score_multiplier = 2
        active_power_ups['double_points'] = pygame.time.get_ticks()
    elif power_up_type == 'invincibility':
        shield_active = True
        active_power_ups['invincibility'] = pygame.time.get_ticks()
    elif power_up_type == 'speed_boost':
        player_speed += 3
        active_power_ups['speed_boost'] = pygame.time.get_ticks()

def reset_power_ups():
    global bullet_speed, score_multiplier, shoot_cooldown, shield_active, player_speed
    current_time = pygame.time.get_ticks()
    if 'rapid_fire' in active_power_ups:
        if current_time - active_power_ups['rapid_fire'] > power_up_duration:
            shoot_cooldown = 500  # Reset to default
            del active_power_ups['rapid_fire']
    if 'double_points' in active_power_ups:
        if current_time - active_power_ups['double_points'] > power_up_duration:
            score_multiplier = 1
            del active_power_ups['double_points']
    if 'invincibility' in active_power_ups:
        if current_time - active_power_ups['invincibility'] > power_up_duration:
            shield_active = False
            del active_power_ups['invincibility']
    if 'speed_boost' in active_power_ups:
        if current_time - active_power_ups['speed_boost'] > power_up_duration:
            player_speed -= 3
            del active_power_ups['speed_boost']

def add_explosion(position):
    explosions.append({'images': explosion_images.copy(), 'pos': position})

def update_explosions():
    for explosion in explosions[:]:
        if explosion['images']:
            img = explosion['images'].pop(0)
            screen.blit(img, (explosion['pos'][0] - img.get_width() // 2, explosion['pos'][1] - img.get_height() // 2))
        else:
            explosions.remove(explosion)

def save_high_score():
    global score, high_score
    if score > high_score:
        high_score = score
        with open("highscore.txt", "w") as f:
            f.write(str(high_score))

create_enemies()

# Game loop
while running:
    if game_state == "menu":
        display_menu()
    elif game_state == "paused":
        display_pause_menu()
    elif game_state == "game_over":
        # Save high score
        save_high_score()
        # Game over screen
        screen.fill(BLACK)
        game_over_text = font.render("Game Over!", True, WHITE)
        final_score_text = font.render(f"Final Score: {score}", True, WHITE)
        high_score_text = font.render(f"High Score: {high_score}", True, WHITE)
        coins_earned_text = font.render(f"Total Coins: {coins}", True, WHITE)
        restart_text = font.render("Press R to Restart or Q to Quit", True, WHITE)
        screen.blit(game_over_text, (WIDTH // 2 - game_over_text.get_width() // 2, HEIGHT // 2 - 100))
        screen.blit(final_score_text, (WIDTH // 2 - final_score_text.get_width() // 2, HEIGHT // 2 - 50))
        screen.blit(high_score_text, (WIDTH // 2 - high_score_text.get_width() // 2, HEIGHT // 2))
        screen.blit(coins_earned_text, (WIDTH // 2 - coins_earned_text.get_width() // 2, HEIGHT // 2 + 50))
        screen.blit(restart_text, (WIDTH // 2 - restart_text.get_width() // 2, HEIGHT // 2 + 100))
        pygame.display.flip()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_r:
                    # Reset game variables
                    score = 0
                    coins = 0
                    lives = 10
                    level = 1
                    aimbot_level = 0
                    player_speed = 5
                    bullet_speed = -7
                    active_power_ups = {}
                    enemies = []
                    create_enemies()
                    game_state = "playing"
                elif event.key == pygame.K_q:
                    running = False
                    pygame.quit()
                    sys.exit()
        clock.tick(30)
    else:
        # Main game loop
        clock.tick(FPS)
        screen.fill(BLACK)

        reset_power_ups()

        # Event handling
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                save_high_score()
                pygame.quit()
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_p:
                    game_state = "paused"

        # Key states
        keys = pygame.key.get_pressed()
        if keys[pygame.K_LEFT] and player_pos[0] > 0:
            player_pos[0] -= player_speed
        if keys[pygame.K_RIGHT] and player_pos[0] < WIDTH - player_size[0]:
            player_pos[0] += player_speed
        if keys[pygame.K_SPACE]:
            now_shot = pygame.time.get_ticks()
            if now_shot - last_shot > shoot_cooldown:
                bullet_rect = pygame.Rect(player_pos[0] + player_size[0] // 2 - bullet_size[0] // 2,
                                          player_pos[1], *bullet_size)
                # Add homing bullets with turning rate based on aimbot_level
                if aimbot_level > 0:
                    # Find the nearest enemy or boss
                    if enemies or (boss_fight and not boss_defeated):
                        if boss_fight and not boss_defeated:
                            target = 'boss'  # Indicate that the target is the boss
                        else:
                            target_enemy = min(enemies, key=lambda e: math.hypot(e['rect'].centerx - bullet_rect.centerx, e['rect'].centery - bullet_rect.centery))
                            target = target_enemy
                        # Initial velocity towards the target
                        if target == 'boss':
                            target_rect = pygame.Rect(boss_pos[0], boss_pos[1], *boss_size)
                        else:
                            target_rect = target['rect']
                        dx = target_rect.centerx - bullet_rect.centerx
                        dy = target_rect.centery - bullet_rect.centery
                        angle = math.atan2(dy, dx)
                        speed = 10  # Bullet speed
                        vx = speed * math.cos(angle)
                        vy = speed * math.sin(angle)
                        bullets.append({'rect': bullet_rect, 'vx': vx, 'vy': vy, 'target': target, 'turning_rate': 0.05 + aimbot_level * 0.02})
                    else:
                        # No targets, shoot straight
                        bullets.append({'rect': bullet_rect, 'vx': 0, 'vy': bullet_speed, 'target': None, 'turning_rate': 0})
                else:
                    bullets.append({'rect': bullet_rect, 'vx': 0, 'vy': bullet_speed, 'target': None, 'turning_rate': 0})
                last_shot = now_shot
        if keys[pygame.K_s]:
            now_shop = pygame.time.get_ticks()
            if now_shop - last_shop > shop_cooldown:
                display_shop()
                last_shop = now_shop

        # Update bullets
        for bullet in bullets[:]:
            # Homing behavior
            if bullet['target'] and bullet['turning_rate'] > 0:
                # Update target position
                if bullet['target'] == 'boss' and boss_fight and not boss_defeated:
                    # Target is the boss; update to current boss position
                    target_rect = pygame.Rect(boss_pos[0], boss_pos[1], *boss_size)
                elif bullet['target'] in enemies:
                    # Target is an enemy
                    target_rect = bullet['target']['rect']
                else:
                    # Target is invalid
                    bullet['target'] = None
                    target_rect = None

                if target_rect:
                    # Calculate desired direction
                    dx = target_rect.centerx - bullet['rect'].centerx
                    dy = target_rect.centery - bullet['rect'].centery
                    desired_angle = math.atan2(dy, dx)
                    current_angle = math.atan2(bullet['vy'], bullet['vx'])
                    # Calculate angle difference
                    angle_diff = desired_angle - current_angle
                    # Ensure angle_diff is between -pi and pi
                    angle_diff = (angle_diff + math.pi) % (2 * math.pi) - math.pi
                    # Limit turning rate
                    max_turn = bullet['turning_rate']
                    if abs(angle_diff) > max_turn:
                        angle_diff = max_turn if angle_diff > 0 else -max_turn
                    # Update bullet velocity
                    new_angle = current_angle + angle_diff
                    speed = math.hypot(bullet['vx'], bullet['vy'])
                    bullet['vx'] = speed * math.cos(new_angle)
                    bullet['vy'] = speed * math.sin(new_angle)
            # Update bullet position
            bullet['rect'].x += bullet['vx']
            bullet['rect'].y += bullet['vy']
            # Remove bullets that are off-screen
            if bullet['rect'].y < 0 or bullet['rect'].x < 0 or bullet['rect'].x > WIDTH or bullet['rect'].y > HEIGHT:
                bullets.remove(bullet)
                continue
            # Bullet collision with enemies or boss
            if boss_fight and not boss_defeated:
                boss_rect = pygame.Rect(boss_pos[0], boss_pos[1], *boss_size)
                if bullet['rect'].colliderect(boss_rect):
                    bullets.remove(bullet)
                    boss_health -= 1
                    add_explosion(bullet['rect'].center)
                    if boss_health <= 0:
                        boss_defeated = True
                        score += 100 * score_multiplier
                        coins += 50
            else:
                for enemy in enemies[:]:
                    if bullet['rect'].colliderect(enemy['rect']):
                        bullets.remove(bullet)
                        enemy['health'] -= 1
                        add_explosion(bullet['rect'].center)
                        if enemy['health'] <= 0:
                            enemies.remove(enemy)
                            enemy_info = enemy_types[enemy['type']]
                            score += enemy_info['score'] * score_multiplier
                            coins += enemy_info['coins']
                            # Chance to drop a power-up
                            if random.random() < 0.1:  # 10% chance
                                power_up_type = random.choice(['rapid_fire', 'double_points', 'invincibility', 'speed_boost'])
                                power_up_rect = pygame.Rect(enemy['rect'].x + enemy_size[0] // 2 - 10, enemy['rect'].y, 20, 20)
                                power_ups.append({'rect': power_up_rect, 'type': power_up_type, 'surface': get_power_up_surface(power_up_type)})
                        break

        # Update enemy bullets
        for e_bullet in enemy_bullets[:]:
            e_bullet.y += enemy_bullet_speed
            if e_bullet.y > HEIGHT:
                enemy_bullets.remove(e_bullet)
            elif e_bullet.colliderect(pygame.Rect(*player_pos, *player_size)):
                if shield_active:
                    shield_active = False  # Shield absorbs one hit
                    enemy_bullets.remove(e_bullet)
                else:
                    lives -= 1
                    enemy_bullets.remove(e_bullet)
                    add_explosion(player_pos)
                    if lives <= 0:
                        game_state = "game_over"

        # Update boss bullets
        for b_bullet in boss_bullets[:]:
            b_bullet.y += boss_bullet_speed
            if b_bullet.y > HEIGHT:
                boss_bullets.remove(b_bullet)
            elif b_bullet.colliderect(pygame.Rect(*player_pos, *player_size)):
                if shield_active:
                    shield_active = False
                    boss_bullets.remove(b_bullet)
                else:
                    lives -= 1
                    boss_bullets.remove(b_bullet)
                    add_explosion(player_pos)
                    if lives <= 0:
                        game_state = "game_over"

        if boss_fight and not boss_defeated:
            # Update boss movement
            boss_pos[0] += boss_speed * boss_direction
            if boss_pos[0] <= 0 or boss_pos[0] + boss_size[0] >= WIDTH:
                boss_direction *= -1
                boss_pos[1] += 20  # Boss moves down when hitting the wall
            # Boss firing bullets
            if random.random() < 0.02:
                b_bullet_rect = pygame.Rect(boss_pos[0] + boss_size[0] // 2 - bullet_size[0] // 2, boss_pos[1] + boss_size[1], *bullet_size)
                boss_bullets.append(b_bullet_rect)
        else:
            # Update enemies
            if not boss_fight:
                move_down = False
                for enemy in enemies:
                    enemy_info = enemy_types[enemy['type']]
                    enemy['rect'].x += enemy_info['speed'] * enemy_direction
                    if enemy['rect'].right >= WIDTH or enemy['rect'].left <= 0:
                        move_down = True
                if move_down:
                    enemy_direction *= -1
                    for enemy in enemies:
                        enemy['rect'].y += enemy_size[1] // 2

            # Enemies fire bullets at controlled intervals
            current_time = pygame.time.get_ticks()
            if current_time - last_enemy_fire > enemy_fire_interval:
                if enemies:
                    firing_enemies = random.sample(enemies, k=max(1, len(enemies) // 5))  # Only a fraction of enemies fire
                    for enemy in firing_enemies:
                        e_bullet_rect = pygame.Rect(enemy['rect'].centerx - bullet_size[0] // 2, enemy['rect'].bottom, *bullet_size)
                        enemy_bullets.append(e_bullet_rect)
                last_enemy_fire = current_time

        # Power-ups movement and collection
        for power_up in power_ups[:]:
            power_up['rect'].y += 2  # Power-up falls down
            if power_up['rect'].y > HEIGHT:
                power_ups.remove(power_up)
            elif power_up['rect'].colliderect(pygame.Rect(*player_pos, *player_size)):
                apply_power_up(power_up['type'])
                power_ups.remove(power_up)

        # Check for game over (enemy reaches bottom)
        for enemy in enemies:
            if enemy['rect'].bottom >= HEIGHT - 100:
                game_state = "game_over"

        # Level progression
        if not enemies and not boss_fight and level % 3 == 0:
            boss_fight = True  # Start boss fight every 3 levels
        elif boss_defeated and boss_fight:
            boss_fight = False
            boss_defeated = False
            boss_health = 50 + level * 10  # Increase boss health with levels
            boss_pos = [WIDTH // 2 - boss_size[0] // 2, 50]
            level += 1
            enemy_fire_interval = max(500, enemy_fire_interval - 50)  # Increase enemy firing rate
            create_enemies()
        elif not enemies and not boss_fight:
            level += 1
            enemy_fire_interval = max(500, enemy_fire_interval - 50)
            create_enemies()

        # Draw player
        screen.blit(player_surface, player_pos)
        if shield_active:
            # Draw shield around player
            pygame.draw.ellipse(screen, BLUE, (player_pos[0] - 10, player_pos[1] - 10, player_size[0] + 20, player_size[1] + 20), 2)

        # Draw enemies
        if not boss_fight:
            for enemy in enemies:
                enemy_info = enemy_types[enemy['type']]
                screen.blit(enemy_info['surface'], (enemy['rect'].x, enemy['rect'].y))

        # Draw boss
        if boss_fight and not boss_defeated:
            screen.blit(boss_surface, boss_pos)
            # Draw boss health bar
            health_bar_width = boss_size[0]
            health_bar_height = 10
            health_ratio = boss_health / (50 + (level - 1) * 10)
            pygame.draw.rect(screen, RED, (boss_pos[0], boss_pos[1] - 20, health_bar_width, health_bar_height))
            pygame.draw.rect(screen, GREEN, (boss_pos[0], boss_pos[1] - 20, health_bar_width * health_ratio, health_bar_height))

        # Draw bullets
        for bullet in bullets:
            pygame.draw.rect(screen, BULLET_COLOR, bullet['rect'])
        for e_bullet in enemy_bullets:
            pygame.draw.rect(screen, RED, e_bullet)
        for b_bullet in boss_bullets:
            pygame.draw.rect(screen, PURPLE, b_bullet)

        # Draw power-ups
        for power_up in power_ups:
            screen.blit(power_up['surface'], (power_up['rect'].x, power_up['rect'].y))

        # Update and draw explosions
        update_explosions()

        # Draw score
        score_text = font.render(f"Score: {score}", True, WHITE)
        screen.blit(score_text, (10, 10))

        # Draw coins
        coins_text = font.render(f"Coins: {coins}", True, WHITE)
        screen.blit(coins_text, (10, 40))

        # Draw lives
        lives_text = font.render(f"Lives: {lives}", True, WHITE)
        screen.blit(lives_text, (10, 70))

        # Draw level
        level_text = font.render(f"Level: {level}", True, WHITE)
        screen.blit(level_text, (WIDTH - 150, 10))

        # Draw shield status
        if shield_active:
            shield_text = font.render("Shield Active", True, WHITE)
            screen.blit(shield_text, (10, 100))

        # Draw Aimbot status
        if aimbot_level > 0:
            aimbot_text = font.render(f"Aimbot Level: {aimbot_level}", True, WHITE)
            screen.blit(aimbot_text, (10, 130))

        # Draw active power-ups
        y_offset = 160
        for power_up in active_power_ups:
            pu_text = font.render(f"{power_up.title()} Active", True, WHITE)
            screen.blit(pu_text, (10, y_offset))
            y_offset += 30

        pygame.display.flip()

pygame.quit()
