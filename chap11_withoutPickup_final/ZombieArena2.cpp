#include <SFML/Graphics.hpp>
#include "Player.cpp"
#include "CreateHorde.cpp"
#include "Zombie.cpp"
#include "Bullet.cpp"

using namespace sf;

int createBackground(VertexArray& rVA, IntRect arena);

enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

int main() {
    State state = State::GAME_OVER;

    // Basic Setup
    Vector2f resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height);
    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);
    View mainView(FloatRect(0, 0, resolution.x, resolution.y));

    Clock clock;
    Time gameTimeTotal;
    Vector2f mouseWorldPosition;
    Vector2i mouseScreenPosition;

    // Player and Arena
    Player player;
    IntRect arena;
    VertexArray background;
    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background_sheet.png");

    // Zombies
    int numZombies = 0, numZombiesAlive = 0;
    Zombie* zombies = nullptr;

    // Bullets
    Bullet bullets[100];
    int currentBullet = 0, bulletsSpare = 24, bulletsInClip = 6, clipSize = 6;
    float fireRate = 10;
    Time lastPressed;

    // Crosshair
    window.setMouseCursorVisible(true);
    Sprite spriteCrosshair;
    Texture textureCrosshair;
    textureCrosshair.loadFromFile("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);

    // Score
    int score = 0, hiScore = 0;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) window.close();

                if (event.key.code == Keyboard::Return) {
                    if (state == State::GAME_OVER) state = State::LEVELING_UP;
                    else if (state == State::LEVELING_UP) {
                        state = State::PLAYING;
                        arena = { 0, 0, 500, 500 };
                        int tileSize = createBackground(background, arena);
                        player.spawn(arena, resolution, tileSize);
                        delete[] zombies;
                        zombies = createHorde(numZombies = 4, arena);
                        numZombiesAlive = numZombies;
                        clock.restart();
                    }
                    else if (state == State::PLAYING) state = State::PAUSED;
                    else if (state == State::PAUSED) { state = State::PLAYING; clock.restart(); }
                }

                if (state == State::PLAYING && event.key.code == Keyboard::R) {
                    int bulletsToReload = std::min(clipSize - bulletsInClip, bulletsSpare);
                    bulletsInClip += bulletsToReload;
                    bulletsSpare -= bulletsToReload;
                }
            }
        }

        if (state == State::PLAYING) {
            float dt = clock.restart().asSeconds();
            gameTimeTotal += seconds(dt);

            // Input
            auto key = Keyboard::isKeyPressed;
            key(Keyboard::W) ? player.moveUp()    : player.stopUp();
            key(Keyboard::S) ? player.moveDown()  : player.stopDown();
            key(Keyboard::A) ? player.moveLeft()  : player.stopLeft();
            key(Keyboard::D) ? player.moveRight() : player.stopRight();

            // Shooting
            if (Mouse::isButtonPressed(Mouse::Left) &&
                gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate &&
                bulletsInClip > 0) {
                bullets[currentBullet++ % 100].shoot(
                    player.getCenter().x, player.getCenter().y,
                    mouseWorldPosition.x, mouseWorldPosition.y);
                lastPressed = gameTimeTotal;
                bulletsInClip--;
            }

            // Update
            mouseScreenPosition = Mouse::getPosition();
            mouseWorldPosition = window.mapPixelToCoords(mouseScreenPosition, mainView);
            spriteCrosshair.setPosition(mouseWorldPosition);

            player.update(dt, mouseScreenPosition);
            mainView.setCenter(player.getCenter());

            for (int i = 0; i < numZombies; i++)
                if (zombies[i].isAlive())
                    zombies[i].update(dt, player.getCenter());

            for (auto& b : bullets)
                if (b.isInFlight()) b.update(dt);

            // Bullet-Zombie Collisions
            for (auto& b : bullets) {
                if (!b.isInFlight()) continue;
                for (int j = 0; j < numZombies; j++) {
                    if (zombies[j].isAlive() && b.getPosition().intersects(zombies[j].getPosition())) {
                        b.stop();
                        if (zombies[j].hit()) {
                            score += 10;
                            if (score > hiScore) hiScore = score;
                            if (--numZombiesAlive == 0) state = State::LEVELING_UP;
                        }
                        break;
                    }
                }
            }

            // Zombie-Player Collisions
            for (int i = 0; i < numZombies; i++) {
                if (zombies[i].isAlive() &&
                    player.getPosition().intersects(zombies[i].getPosition()) &&
                    player.hit(gameTimeTotal) &&
                    player.getHealth() <= 0) {
                    state = State::GAME_OVER;
                }
            }
        }

        // Drawing
        if (state == State::PLAYING) {
            window.clear();
            window.setView(mainView);
            window.draw(background, &textureBackground);
            for (int i = 0; i < numZombies; i++) window.draw(zombies[i].getSprite());
            for (auto& b : bullets) if (b.isInFlight()) window.draw(b.getShape());
            window.draw(player.getSprite());
            window.draw(spriteCrosshair);
            window.display();
        }
    }

    delete[] zombies;
    return 0;
}
