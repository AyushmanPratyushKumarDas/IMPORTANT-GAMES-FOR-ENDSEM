# Important logics

## ZOMBIE ARENA

### Reload

if (state == State::PLAYING)
	{
		// Reloading
		if (event.key.code == Keyboard::R)
			{
				if (bulletsSpare >= clipSize)
				{
					// Plenty of bullets. Reload.
					bulletsInClip = clipSize;
					bulletsSpare -= clipSize;
				}
				else if (bulletsSpare > 0)
				{
					// Only few bullets left
					bulletsInClip = bulletsSpare;
					bulletsSpare = 0;
				}
				else
                {
					// More here soon!!!!
				}
			}
	}

### Fire a Bullet

if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
				{

					// Pass the centre of the player 
					// and the centre of the cross-hair
					// to the shoot function
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					bulletsInClip--;
				}

			}


### Game Shuru Hone ke time

if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = createBackground(background, arena);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				//Create a horde of zombies
				   numZombies = 4;

				// Delete the previously allocated memory (if it exists)
				   delete[] zombies;
				   zombies = createHorde(numZombies, arena);
				   numZombiesAlive = numZombies;

				// Reset the clock so there isn't a frame jump
				clock.restart();
}


| Step         | Kya ho raha hai                                  |
| ------------ | ------------------------------------------------ |
| Num1–Num6    | Player ek upgrade choose karta hai               |
| Arena Setup  | 500x500 game area set hota hai                   |
| Background   | Tile map background generate hota hai            |
| Player Spawn | Player ko arena ke center mein daala jaata hai   |
| Zombies      | 4 zombies banaaye jaate hain                     |
| Clock        | Game time reset hota hai (smooth timing ke liye) |


### Key dabane pe kya Hoga
- Game states ko key se handle karna

| Key      | State      | Kya hota hai                  |
| -------- | ---------- | ----------------------------- |
| `Enter`  | PLAYING    | Game pause hota hai           |
| `Enter`  | PAUSED     | Game resume hota hai          |
| `Enter`  | GAME\_OVER | Game restart (leveling up)    |
| `R`      | PLAYING    | Reload (if bullets available) |
| `Escape` | Any        | Game band ho jata hai         |

### Khelne ke time

| Input            | Action                                                               |
| ---------------- | -------------------------------------------------------------------- |
| `W`              | Player upar chalta hai                                               |
| `S`              | Player neeche chalta hai                                             |
| `A`              | Player left jaata hai                                                |
| `D`              | Player right jaata hai                                               |
| Mouse Left Click | Agar cooldown + bullets hai, toh bullet fire hota hai mouse ki taraf |

### Update The Scene

if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			//*******Set the crosshair to the //
			//*******mouse world location****//
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());

			//Loop through each Zombie and update them
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}
			
			//********Update any bullets that are in-flight****/
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			//*******Collision detection*****/
			//*****Have any zombies been shot?****/
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
							(zombies[j].getPosition()))
						{
							//*****Stop the bullet
							bullets[i].stop();

							//*******Register the hit and see if it was a kill
							if (zombies[j].hit()) {
								//*******Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numZombiesAlive--;

								//******When all the zombies are dead (again)
								if (numZombiesAlive == 0) {
									state = State::LEVELING_UP;
								}
							}

						}
					}

				}
			}//******End zombie being shot

			 //******Have any zombies touched the player			
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
					(zombies[i].getPosition()) && zombies[i].isAlive())
				{

					if (player.hit(gameTimeTotal))
					{
						// More here later
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;

					}
				}
			}//******End player touched

		}//*******End updating the scene
		

| 🔢 Step | 📋 Kya ho raha hai                              |
| ------- | ----------------------------------------------- |
| 1       | Time update for smooth movement                 |
| 2       | Mouse position fetch + world coordinate convert |
| 3       | Crosshair ko mouse ke peeche lagana             |
| 4       | Player movement update + camera follow          |
| 5       | Zombies ka movement update                      |
| 6       | Bullets ka movement update                      |
| 7       | Bullet-Zombie collision + score/leveling check  |
| 8       | Zombie-Player collision + GAME\_OVER check      |



