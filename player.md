void Player::spawn(IntRect arena, Vector2f resolution, int tileSize)
{
	// Place the player in the middle of the arena
	m_Position.x = arena.width / 2;
	m_Position.y = arena.height / 2;

	// Copy the details of the arena to the player's m_Arena
	m_Arena.left = arena.left;
	m_Arena.width = arena.width;
	m_Arena.top = arena.top;
	m_Arena.height = arena.height;

	// Remember how big the tiles are in this arena
	m_TileSize = tileSize;

	// Strore the resolution for future use
	m_Resolution.x = resolution.x;
	m_Resolution.y = resolution.y;
}


void Player::update(float elapsedTime, Vector2i mousePosition)
{

	if (m_UpPressed)
	{
		m_Position.y -= m_Speed * elapsedTime;
	}

	if (m_DownPressed)
	{
		m_Position.y += m_Speed * elapsedTime;
	}

	if (m_RightPressed)
	{
		m_Position.x += m_Speed * elapsedTime;
	}

	if (m_LeftPressed)
	{
		m_Position.x -= m_Speed * elapsedTime;
	}

	m_Sprite.setPosition(m_Position);

	// Keep the player in the arena
	if (m_Position.x > m_Arena.width - m_TileSize)
	{
		m_Position.x = m_Arena.width - m_TileSize;
	}

	if (m_Position.x < m_Arena.left + m_TileSize)
	{
		m_Position.x = m_Arena.left + m_TileSize;
	}

	if (m_Position.y > m_Arena.height - m_TileSize)
	{
		m_Position.y = m_Arena.height - m_TileSize;
	}

	if (m_Position.y < m_Arena.top + m_TileSize)
	{
		m_Position.y = m_Arena.top + m_TileSize;
	}

	// Calculate the angle the player is facing
	float angle = (atan2(mousePosition.y - m_Resolution.y / 2,
						 mousePosition.x - m_Resolution.x / 2) *
				   180) /
				  3.141;

	m_Sprite.setRotation(angle);
}


| Variable Name    | Type          | Purpose                                                          |
| ---------------- | ------------- | ---------------------------------------------------------------- |
| `START_SPEED`    | `const float` | Initial speed of the player (200).                               |
| `START_HEALTH`   | `const float` | Starting and default max health (100).                           |
| `m_Position`     | `Vector2f`    | Current position of the player in the world.                     |
| `m_Sprite`       | `Sprite`      | Sprite used to draw the player.                                  |
| `m_Texture`      | `Texture`     | Texture loaded from player image file.                           |
| `m_Resolution`   | `Vector2f`    | Screen resolution used to calculate mouse angle and orientation. |
| `m_Arena`        | `IntRect`     | Bounds of the arena (playable area).                             |
| `m_TileSize`     | `int`         | Size of each tile in the arena grid.                             |
| `m_UpPressed`    | `bool`        | Flag to check if "up" movement key is pressed.                   |
| `m_DownPressed`  | `bool`        | Flag to check if "down" movement key is pressed.                 |
| `m_LeftPressed`  | `bool`        | Flag to check if "left" movement key is pressed.                 |
| `m_RightPressed` | `bool`        | Flag to check if "right" movement key is pressed.                |
| `m_Health`       | `int`         | Current health value of the player.                              |
| `m_MaxHealth`    | `int`         | Maximum health player can have.                                  |
| `m_LastHit`      | `Time`        | Time when player was last hit (for cooldown).                    |
| `m_Speed`        | `float`       | Current speed of player (may increase via upgrades).             |


| Function Signature                                         | Purpose                                                        |
| ---------------------------------------------------------- | -------------------------------------------------------------- |
| `Player()`                                                 | Constructor to initialize sprite, health, speed, and texture.  |
| `void spawn(IntRect, Vector2f, int)`                       | Spawn player in center of given arena and store arena details. |
| `bool hit(Time)`                                           | Reduce health if enough time has passed since last hit.        |
| `Time getLastHitTime()`                                    | Returns time when player was last hit.                         |
| `FloatRect getPosition()`                                  | Returns bounding rectangle of the sprite (for collision).      |
| `Vector2f getCenter()`                                     | Returns current position (center) of the player.               |
| `float getRotation()`                                      | Returns the rotation angle of the sprite (facing direction).   |
| `Sprite getSprite()`                                       | Returns the sprite object to be drawn.                         |
| `int getHealth()`                                          | Returns current health value.                                  |
| `void moveLeft()`, `moveRight()`, `moveUp()`, `moveDown()` | Set respective movement flags to `true`.                       |
| `void stopLeft()`, `stopRight()`, `stopUp()`, `stopDown()` | Set respective movement flags to `false`.                      |
| `void update(float, Vector2i)`                             | Updates position based on movement flags, keeps within arena,  |
|                                                            | and rotates player to face mouse position.                     |
| `void upgradeSpeed()`                                      | Increases player speed by 20%.                                 |
| `void upgradeHealth()`                                     | Increases max health by 20%.                                   |
| `void increaseHealthLevel(int amount)`                     | Heals player but not beyond `m_MaxHealth`.                     |
