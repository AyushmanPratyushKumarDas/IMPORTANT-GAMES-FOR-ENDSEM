void Zombie::update(float elapsedTime,
Vector2f playerLocation)
{
float playerX = playerLocation.x;
float playerY = playerLocation.y;

    // Update the zombie position variables
    if (playerX > m_Position.x)
    {
    	m_Position.x = m_Position.x +
    				   m_Speed * elapsedTime;
    }

    if (playerY > m_Position.y)
    {
    	m_Position.y = m_Position.y +
    				   m_Speed * elapsedTime;
    }

    if (playerX < m_Position.x)
    {
    	m_Position.x = m_Position.x -
    				   m_Speed * elapsedTime;
    }

    if (playerY < m_Position.y)
    {
    	m_Position.y = m_Position.y -
    				   m_Speed * elapsedTime;
    }

    // Move the sprite
    m_Sprite.setPosition(m_Position);

    // Face the sprite in the correct direction
    float angle = (atan2(playerY - m_Position.y,
    					 playerX - m_Position.x) *
    			   180) /
    			  3.141;

    m_Sprite.setRotation(angle);

}


#include "Zombie.cpp"

Zombie *createHorde(int numZombies, IntRect arena)
{
	Zombie *zombies = new Zombie[numZombies];

	int maxY = arena.height - 20;
	int minY = arena.top + 20;
	int maxX = arena.width - 20;
	int minX = arena.left + 20;

	for (int i = 0; i < numZombies; i++)
	{

		// Which side should the zombie spawn
		srand((int)time(0) * i);
		int side = (rand() % 4);
		float x, y;

		switch (side)
		{
		case 0:
			// left
			x = minX;
			y = (rand() % maxY) + minY;
			break;

		case 1:
			// right
			x = maxX;
			y = (rand() % maxY) + minY;
			break;

		case 2:
			// top
			x = (rand() % maxX) + minX;
			y = minY;
			break;

		case 3:
			// bottom
			x = (rand() % maxX) + minX;
			y = maxY;
			break;
		}

		// Bloater, crawler or runner
		srand((int)time(0) * i * 2);
		int type = (rand() % 3);

		// Spawn the new zombie into the array
		zombies[i].spawn(x, y, type, i);
	}
	return zombies;
}


| Variable Name    | Type          | Purpose                                                              |
| ---------------- | ------------- | -------------------------------------------------------------------- |
| `BLOATER_SPEED`  | `const float` | Speed of a bloater zombie (slow but tanky).                          |
| `CHASER_SPEED`   | `const float` | Speed of a chaser zombie (very fast).                                |
| `CRAWLER_SPEED`  | `const float` | Speed of a crawler zombie (very slow).                               |
| `BLOATER_HEALTH` | `const float` | Health of bloater zombie.                                            |
| `CHASER_HEALTH`  | `const float` | Health of chaser zombie.                                             |
| `CRAWLER_HEALTH` | `const float` | Health of crawler zombie.                                            |
| `MAX_VARRIANCE`  | `const int`   | Range for randomness in zombie speed.                                |
| `OFFSET`         | `const int`   | Used to keep speed variation within a range (typically 70% to 100%). |
| `m_Position`     | `Vector2f`    | Current position of the zombie.                                      |
| `m_Texture`      | `Texture`     | Texture (image) of the zombie.                                       |
| `m_Sprite`       | `Sprite`      | Sprite for drawing the zombie.                                       |
| `m_Speed`        | `float`       | Actual randomized movement speed.                                    |
| `m_Health`       | `float`       | Current health of the zombie.                                        |
| `m_Alive`        | `bool`        | True if zombie is alive.                                             |

| Function Signature                                 | Purpose                                                              |
| -------------------------------------------------- | -------------------------------------------------------------------- |
| `void spawn(float x, float y, int type, int seed)` | Spawns the zombie at given position, with type and randomized speed. |
| `bool hit()`                                       | Called when bullet hits; reduces health and returns `true` if dead.  |
| `bool isAlive()`                                   | Returns `true` if zombie is still alive.                             |
| `FloatRect getPosition()`                          | Returns global bounds (used for collision detection).                |
| `Sprite getSprite()`                               | Returns the sprite for rendering.                                    |
| `void update(float elapsedTime, Vector2f player)`  | Moves zombie toward player and rotates it to face the player.        |
