| Variable               | 📋 Kya store karta hai                         |
| ---------------------- | ---------------------------------------------- |
| `m_Position`           | Bullet ki current position (x, y)              |
| `m_BulletShape`        | SFML shape object jo bullet draw karta hai     |
| `m_InFlight`           | Bullet ud raha hai ya nahi                     |
| `m_BulletSpeed`        | Bullet ki speed (1000 pixels/sec)              |
| `m_BulletDistanceX`    | Har frame mein x-axis par kitna chalega bullet |
| `m_BulletDistanceY`    | Har frame mein y-axis par kitna chalega bullet |
| `m_XTarget/YTarget`    | Target position jahan bullet shoot kiya gaya   |
| `m_MinX/Y`, `m_MaxX/Y` | Bullet ke allowed limits (range = 1000 pixels) |

| 🔧 Function     | 📋 Kaam kya karta hai                              |
| --------------- | -------------------------------------------------- |
| `shoot()`       | Bullet ko fire karta hai towards a direction       |
| `update()`      | Har frame bullet move karta hai                    |
| `stop()`        | Bullet ko rokta hai                                |
| `isInFlight()`  | Check karta hai ki bullet abhi active hai ya nahi  |
| `getShape()`    | Drawing ke liye bullet ka shape return karta hai   |
| `getPosition()` | Collision detection ke liye shape return karta hai |


float gradient = (startY - targetY) / (startX - targetX);
if (gradient < 0) gradient *= -1;

m_BulletDistanceY = (gradient / (1 + gradient)) * m_BulletSpeed;
m_BulletDistanceX = (1 / (1 + gradient)) * m_BulletSpeed;


void Bullet::shoot(float startX, float startY, float targetX, float targetY)
{
	// Keep track of the bullet
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	float gradient = (startY - targetY) / (startX - targetX);
	//******************************************************/

	// Any gradient less than zero needs to be negative
	if (gradient < 0)
	{
		gradient *= -1;
	}
	m_BulletDistanceY = (gradient / (1 + gradient)) * m_BulletSpeed; // y = s * sin A

	// Fraction of 1 px the bullet travels horizontally in each frame
	m_BulletDistanceX = (1 / (1 + gradient)) * m_BulletSpeed; // x = s * cos A
	//*************************************************************/

	// Point the bullet in the right direction
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}

	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	// Finally, assign the results to the
	// member variables
	m_XTarget = targetX;
	m_YTarget = targetY;

	// Set a max range of 1000 pixels
	float range = 1000;
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	// Position the bullet ready to be drawn
	m_BulletShape.setPosition(m_Position);
}

void Bullet::shoot(float startX, float startY, float targetX, float targetY) {
    m_InFlight = true;
    m_Position = { startX, startY };

    // Calculate distance between points
    float dx = targetX - startX;
    float dy = targetY - startY;

    // Calculate the overall distance (hypotenuse)
    float distance = std::sqrt(dx * dx + dy * dy);

    // Normalize direction and scale by speed
    m_BulletDistanceX = (dx / distance) * m_BulletSpeed;
    m_BulletDistanceY = (dy / distance) * m_BulletSpeed;

    // Set bullet's travel limits (1000 pixels in all directions)
    float range = 1000;
    m_MinX = startX - range;
    m_MaxX = startX + range;
    m_MinY = startY - range;
    m_MaxY = startY + range;

    // Position the bullet for rendering
    m_BulletShape.setPosition(m_Position);
}




void Bullet::update(float elapsedTime)
{
	// Update the bullet position variables
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// Move the bullet
	m_BulletShape.setPosition(m_Position);

	// Has the bullet gone out of range?
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
}