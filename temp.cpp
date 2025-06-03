#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
using namespace sf;
int main()
{
    VideoMode vm(960, 540);
    RenderWindow window(vm, "rectangle");
    View gameView(FloatRect(0, 0, 960, 540));
    window.setView(gameView);

    //background
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("imgs/background.png");
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale((float)window.getSize().x/backgroundTexture.getSize().x, (float)window.getSize().y/backgroundTexture.getSize().y);
    background.setPosition(0, 0);

    CircleShape circle;
    circle.setRadius(50);            // Set radius of circle
    circle.setFillColor(Color::Red); // Set fill color
    circle.setPosition(400, 220);

    // lets make a rectangle and place it in the center of the screen
    RectangleShape rect;
    rect.setSize(Vector2f(300, 150));
    rect.setFillColor(Color::Red);
    rect.setPosition(window.getSize().x / 2 - rect.getSize().x/2, window.getSize().y / 2 - rect.getSize().x/2);

    // lets load a texture
    Texture ball;
    ball.loadFromFile("imgs/ball.png");
    Sprite ballsprite;
    ballsprite.setTexture(ball);
    ballsprite.setPosition(300, 200);
    ballsprite.setScale(0.09, 0.09);

    // lets do a text
    Text demotxt;
    Font font;
    font.loadFromFile("Game_Bubble.ttf");
    demotxt.setFont(font);
    // demotxt.setString("Hello world");
    demotxt.setCharacterSize(50);
    demotxt.setFillColor(Color::Green);
    demotxt.setPosition(300, 220);

    // random moving shits
    Texture eagleTexture;
    eagleTexture.loadFromFile("imgs/dick.png");
    Sprite eagle1, eagle2;
    eagle1.setTexture(eagleTexture);
    eagle1.setPosition(20, 0);
    eagle1.setScale(0.1, 0.1);

    eagle2.setTexture(eagleTexture);
    eagle2.setPosition(37, 0);
    eagle2.setScale(0.1, 0.1);

    float eagle1Speed = 0.5f;
    bool eagleA1ctive = false;
    float eagle2Speed = 0.8f;
    bool eagleA2ctive = false;

    // superman
    Texture supermanTexture;
    supermanTexture.loadFromFile("imgs/sup.png");
    Sprite superman;
    superman.setTexture(supermanTexture);
    superman.setPosition(0, 540);
    superman.setScale(0.1, 0.1);
    float supermanSpeed = 0.8f;
    bool supermanActive = false;
    // timebar
    RectangleShape timebar;
    float timebarwidth = 300.0f; // Corrected for 30 seconds (30 * 10 width per second)
    float timebarheight = 30;
    timebar.setFillColor(Color::Red);
    float timeremaining = 30.0f;
    float timebarwidthpersecond = 10.0f;
    timebar.setSize(Vector2f(timebarwidth, timebarheight));
    timebar.setPosition((960 / 2) - (timebarwidth / 2), 540 - timebarheight - 20);

    Clock clock;
    while (window.isOpen())
    {
        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
            {
                window.close();
            }
        }
        // TIMEBAR
        Time dt = clock.restart();
        timeremaining -= dt.asSeconds();
        timebar.setSize(Vector2f(timebarwidthpersecond * timeremaining, timebarheight));
        if (timeremaining <= 0.0f)
        {
            demotxt.setString("Game Over!");
            timeremaining = 30.0f;
            timebar.setSize(Vector2f(timebarwidth, timebarheight));
        }
        // MOVEMENT of eagle
        if (!eagleA1ctive)
        {
            eagle1Speed = (rand() % 200) + 100;
            eagleA1ctive = true;
        }
        else
        {
            eagle1.setPosition(eagle1.getPosition().x + eagle1Speed * dt.asSeconds(), eagle1.getPosition().y);
            if (eagle1.getPosition().x > 960)
            {
                eagle1.setPosition(-200 + rand() % 200, rand() % 200);
            }
        }
        if (!eagleA2ctive)
        {
            eagle2Speed = (rand() % 200) + 100;
            eagleA2ctive = true;
        }
        else
        {
            eagle2.setPosition(eagle2.getPosition().x + eagle2Speed * dt.asSeconds(), eagle2.getPosition().y);
            if (eagle2.getPosition().x > 960)
            {
                eagle2.setPosition(-200 + rand() % 200, rand() % 100);
            }
        }

        // MOVEMENT of superman
        if (!supermanActive)
        {
            supermanSpeed = (rand() % 250) + 250;
            supermanActive = true;
        }
        else
        {
            superman.setPosition(superman.getPosition().x, superman.getPosition().y - supermanSpeed * dt.asSeconds());
            if (superman.getPosition().y < 0)
            {
                superman.setPosition(rand() % 900, 540);
            }
        }

        // MOVEMENT of ball
        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            ballsprite.setPosition(ballsprite.getPosition().x + 1, ballsprite.getPosition().y);
            if (ballsprite.getPosition().x > 960)
            {
                ballsprite.setPosition(0, ballsprite.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::A))
        {
            ballsprite.setPosition(ballsprite.getPosition().x - 1, ballsprite.getPosition().y);
            if (ballsprite.getPosition().x < 0)
            {
                ballsprite.setPosition(960, ballsprite.getPosition().y);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            ballsprite.setPosition(ballsprite.getPosition().x, ballsprite.getPosition().y - 1);
            if (ballsprite.getPosition().y < 0)
            {
                ballsprite.setPosition(ballsprite.getPosition().x, 540);
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::S))
        {
            ballsprite.setPosition(ballsprite.getPosition().x, ballsprite.getPosition().y + 1);
            if (ballsprite.getPosition().y > 540)
            {
                ballsprite.setPosition(ballsprite.getPosition().x, 0);
            }
        }

        window.clear(Color::Black);
        // window.draw(background);
        window.draw(ballsprite);
        // window.draw(circle);
        // window.draw(rect);
        window.draw(demotxt);
        window.draw(eagle1);
        window.draw(eagle2);
        window.draw(superman);
        window.draw(timebar);

        window.display();
    }
}