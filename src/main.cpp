#include "RectTools.hpp"
#include "GameAudio.hpp"
#include "../Map/MapLoader.hpp"
#include "Enemy.hpp"
#include "Constants.hpp"
#include "MyClock.hpp"
#include <sstream>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.hpp"
#include <fstream>
#include <string>

#define NB_TILEMAP_WIDTH 40
#define NB_TILEMAP_HEIGHT 41
#define MAX_TILE_ITEMS NB_TILEMAP_WIDTH * NB_TILEMAP_HEIGHT
#define TILE_SIZE 48
#define MAX_ENEMIES 32
#define NB_LEVELS 3


void setSprite(sf::Sprite &sprite, sf::Texture &texture,sf::IntRect bounds){
	sprite.setTexture(texture);
	sprite.setTextureRect(bounds);
}

void loadLevel(bool &loadTextures, int levelId, std::vector<sf::Sprite*> &collisionTiles, std::vector<std::vector<sf::Sprite*>> &levelsSprites, std::vector<sf::Sprite*> &topCollisionTiles, std::vector<std::vector<sf::Sprite*>> &levelsTopSprites, Player &player, sf::FloatRect &start, std::vector<Enemy *> &enemies,const std::vector<Point> &enemiesPos,std::vector<sf::Sprite *>&enemiesCollisionTiles, std::vector<std::vector<sf::Sprite*>>&ennemiesColSpriteVec,sf::View &view, std::vector<sf::Sprite *>&rings,std::vector<Point> &ringPos){
	/*Collision tiles*/
	collisionTiles.clear();
	for(int j = 0;j < levelsSprites[levelId].size();++j){
			if(levelsSprites[levelId][j])collisionTiles.push_back(levelsSprites[levelId][j]);
	}
	topCollisionTiles.clear();
	for(int j = 0;j < levelsTopSprites[levelId].size();++j){
			if(levelsTopSprites[levelId][j])topCollisionTiles.push_back(levelsTopSprites[levelId][j]);
	}

	/*Player start pos*/
	player.getCurrentSprite().setPosition(sf::Vector2f(start.left,start.top));
	view.setCenter(start.left,start.top);

	/*Enemies*/
	enemies.clear();
	for(auto it = enemiesPos.begin(); it != enemiesPos.end();++it){
		enemies.push_back(new Enemy(it->x,it->y));
	}
	enemiesCollisionTiles.clear();
	for(int j = 0;j < ennemiesColSpriteVec[levelId].size();++j){
			if(ennemiesColSpriteVec[levelId][j])enemiesCollisionTiles.push_back(ennemiesColSpriteVec[levelId][j]);
	}

	/*Rings*/
	rings.clear();
	for(auto it = ringPos.begin(); it != ringPos.end();++it){
		rings.push_back(new sf::Sprite());
		rings.back()->move(it->x,it->y);
	}

	loadTextures = true;

	player.IframeCounter = 0;
	player.isDead = false;

	std::cerr<<"Loaded level "<<levelId<<std::endl;
}

int main(){
	sf::RenderWindow window(sf::VideoMode(1280,720),"Piknic!");

	GameAudio gameAudio;

	sf::Font font;
	if (!font.loadFromFile("ressources/Go-Bold.ttf"))
	{
    	throw std::runtime_error("ressources/Go-Bold.ttf");
	}

	//TODO can delete this ? Use maps[currentlevel].collisiontiles
	std::vector<sf::Sprite *>collisionTiles, topCollisionTiles;

	int currentLevel = 0;

	std::vector<sf::Sprite *> rings;

	std::vector<Enemy *> enemies;
	std::vector<sf::Sprite *>enemiesCollisionTiles;

	/*Loading Sprites*/
	sf::Texture pikachuSpriteSheet;
	sf::Texture sonicTilemap;
	sf::Texture enemiesSpriteSheet;

	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("ressources/bg1ext.png");

	sf::Texture ringTexture;
	ringTexture.loadFromFile("ressources/ring.png");

	sf::Sprite ringSpriteHUD;
	ringSpriteHUD.setTexture(ringTexture);
	ringSpriteHUD.setScale(sf::Vector2f(0.20,0.20));
	ringSpriteHUD.move(0,50);

	sf::Sprite tiles[MAX_TILE_ITEMS];
	for(int i = 0; i < MAX_TILE_ITEMS;++i){
		setSprite((tiles[i]),sonicTilemap,sf::IntRect(TILE_SIZE * (i % NB_TILEMAP_WIDTH), TILE_SIZE * (i / NB_TILEMAP_WIDTH) , TILE_SIZE, TILE_SIZE));
	}

    if(!pikachuSpriteSheet.loadFromFile("ressources/pikachu.png")){
        throw std::runtime_error("No file found for pikachu.png!");
    }

	Player player;
	//Default sprite
	sf::Sprite pikachuIdle1Sprite;
	setSprite(pikachuIdle1Sprite, pikachuSpriteSheet,sf::IntRect(27,17,36,38));
	player.setCurrentSprite(pikachuIdle1Sprite);
	//TODO axe of thinking if sprites have inconsistent height
	//player.currentSprite.setOrigin(0,player.currentSprite.getGlobalBounds().left+player.currentSprite.getGlobalBounds().height);

	/*	Creating player animation*/
	sf::Image pikaImages = pikachuSpriteSheet.copyToImage();

	sf::Texture pikaIdleR[1];
	pikaIdleR[0].loadFromImage(pikaImages,sf::IntRect(0,141,52,30));
	player.createNewAnimation(PIKA_IDLE_R,1,1.0f,pikaIdleR);

	sf::Texture pikaIdleL[1];
	pikaIdleL[0].loadFromImage(pikaImages,sf::IntRect(240,715,52,30));
	player.createNewAnimation(PIKA_IDLE_L,1,1.0f,pikaIdleL);
	
	sf::Texture pikaRunningR[4];
	pikaRunningR[0].loadFromImage(pikaImages,sf::IntRect(57,140,52,31));
	pikaRunningR[1].loadFromImage(pikaImages,sf::IntRect(117,140,52,31));
	pikaRunningR[2].loadFromImage(pikaImages,sf::IntRect(177,140,52,31));
	pikaRunningR[3].loadFromImage(pikaImages,sf::IntRect(237,140,52,31));
	player.createNewAnimation(PIKA_RUN_R,4,0.5f,pikaRunningR);

	sf::Texture pikaRunningL[4];
	pikaRunningL[0].loadFromImage(pikaImages,sf::IntRect(183,714,52,30));
	pikaRunningL[1].loadFromImage(pikaImages,sf::IntRect(123,714,52,30));
	pikaRunningL[2].loadFromImage(pikaImages,sf::IntRect(63,714,52,30));
	pikaRunningL[3].loadFromImage(pikaImages,sf::IntRect(4,714,52,30));
	player.createNewAnimation(PIKA_RUN_L,4,0.5f,pikaRunningL);

	sf::Texture pikaSkullBashR[4];
	pikaSkullBashR[0].loadFromImage(pikaImages,sf::IntRect(29,321,52,30));
	pikaSkullBashR[1].loadFromImage(pikaImages,sf::IntRect(88,321,52,30));
	pikaSkullBashR[2].loadFromImage(pikaImages,sf::IntRect(148,321,52,30));
	pikaSkullBashR[3].loadFromImage(pikaImages,sf::IntRect(209,321,52,30));
	player.createNewAnimation(PIKA_SKULLBASH_R,4,0.5f,pikaSkullBashR);

	sf::Texture pikaSkullBashL[4];
	pikaSkullBashL[0].loadFromImage(pikaImages,sf::IntRect(237,238,52,30));
	pikaSkullBashL[1].loadFromImage(pikaImages,sf::IntRect(177,235,52,30));
	pikaSkullBashL[2].loadFromImage(pikaImages,sf::IntRect(210,273,52,30));
	pikaSkullBashL[3].loadFromImage(pikaImages,sf::IntRect(149,272,52,30));
	player.createNewAnimation(PIKA_SKULLBASH_L,4,0.5f,pikaSkullBashL);

	sf::Texture pikaChargeR[1];
	pikaChargeR[0].loadFromImage(pikaImages,sf::IntRect(127,192,52,30));
	player.createNewAnimation(PIKA_CHARGE_R,1,0.5f,pikaChargeR);

	sf::Texture pikaChargeL[1];
	pikaChargeL[0].loadFromImage(pikaImages,sf::IntRect(198,193,52,30));
	player.createNewAnimation(PIKA_CHARGE_L,1,0.5f,pikaChargeL);

	sf::Texture pikaDead[1];
	pikaDead[0].loadFromImage(pikaImages,sf::IntRect(141,446,58,30));
	player.createNewAnimation(PIKA_DEAD,1,0.5f,pikaDead);

	/*Set default animation*/
	player.getCurrentSprite().move(60,100);
	player.lastBoundingBox = player.currentSprite.getGlobalBounds();
	/*	*/

	if(!sonicTilemap.loadFromFile("Map/sonicTilemap.png")){
		throw std::runtime_error("No file found for sonicTilemap.png!");
	}
	if(!enemiesSpriteSheet.loadFromFile("ressources/s1enemies.png")){
		throw std::runtime_error("No file found for s1enemies.png");
	}

	sf::Sprite crabEnemySprite;
	setSprite(crabEnemySprite,enemiesSpriteSheet,sf::IntRect(6,91,42,31));
	crabEnemySprite.move(100,200);
	Enemy crabEnemy;
	crabEnemy.currentSprite = crabEnemySprite;
	enemies.push_back(&crabEnemy);

	/*Creating levels matrix*/
	Map *maps[NB_LEVELS];
	std::vector<std::vector<sf::Sprite*>> levelsSprites, levelsTopSprites , levelsEnemiesCollSprites, levelsBackgroundSprites;

	float maxWidth = 0, maxHeight = 0;

	for(int i = 0; i < NB_LEVELS; ++i){
		maps[i] = createMapFromJSON("Map/level" + std::to_string(i) + ".json");
		std::vector<sf::Sprite*> thisLevelSpriteVec, ennemiesColSpriteVec, thisLevelTopSpriteVec, thisLevelBackgroundSpriteVec;

		//TODO Background layer

		int collisionLayerId = -1, topCollisionLayerId = -1, enemyCollisionLayerId = -1, backgroundTilesLayerId = -1;
		for(int j = 0; j < maps[i]->layers.size(); ++j){
			std::cerr<<"Layer Name : "<<maps[i]->layers[j]->name<<std::endl;
			if(maps[i]->layers[j]->name == "CollisionTiles")collisionLayerId = j;
			if(maps[i]->layers[j]->name == "TopCollisionTiles")topCollisionLayerId = j;
			if(maps[i]->layers[j]->name == "EnemyCollision")enemyCollisionLayerId = j;
			if(maps[i]->layers[j]->name == "Background")backgroundTilesLayerId = j;
		}
		std::cerr<<collisionLayerId<<' ' << topCollisionLayerId << " "<< enemyCollisionLayerId<<" "<<backgroundTilesLayerId<<std::endl;
		for(int j = 0;j < maps[i]->width;++j){
			for(int k = 0;k < maps[i]->heigth;++k){
				/*Overall collision Layer*/
				if(collisionLayerId >= 0 && !maps[i]->layers[collisionLayerId]->matrix[j][k] -1 == -1){
					sf::Sprite *sprt = new sf::Sprite(tiles[maps[i]->layers[collisionLayerId]->matrix[j][k] -1]);
					thisLevelSpriteVec.push_back(sprt);
					thisLevelSpriteVec.back()->move(TILE_SIZE *j, TILE_SIZE * k);
				}

				/*Top only collision layer*/
				if(topCollisionLayerId >= 0 && !maps[i]->layers[topCollisionLayerId]->matrix[j][k] -1 == -1){
					sf::Sprite *sprt = new sf::Sprite(tiles[maps[i]->layers[topCollisionLayerId]->matrix[j][k] -1]);
					thisLevelTopSpriteVec.push_back(sprt);
					thisLevelTopSpriteVec.back()->move(TILE_SIZE *j, TILE_SIZE * k);
				}
				
				/*BG layer*/
				if(backgroundTilesLayerId >= 0 && !maps[i]->layers[backgroundTilesLayerId]->matrix[j][k] -1 == -1){
					sf::Sprite *sprt = new sf::Sprite(tiles[maps[i]->layers[backgroundTilesLayerId]->matrix[j][k] -1]);
					thisLevelBackgroundSpriteVec.push_back(sprt);
					thisLevelBackgroundSpriteVec.back()->move(TILE_SIZE *j, TILE_SIZE * k);
				}

				/*Enemy collision layer*/
				if(enemyCollisionLayerId >= 0 && !maps[i]->layers[enemyCollisionLayerId]->matrix[j][k] -1 == -1){
					sf::Sprite *sprt = new sf::Sprite(tiles[maps[i]->layers[enemyCollisionLayerId]->matrix[j][k] -1]);
					ennemiesColSpriteVec.push_back(sprt);
					ennemiesColSpriteVec.back()->move(TILE_SIZE *j, TILE_SIZE * k);
				}
			}
		}

		std::cerr<<"Nb of bg tiles : "<<thisLevelBackgroundSpriteVec.size()<<std::endl;

		levelsSprites.push_back(thisLevelSpriteVec);
		levelsTopSprites.push_back(thisLevelTopSpriteVec);
		levelsBackgroundSprites.push_back(thisLevelBackgroundSpriteVec);
		levelsEnemiesCollSprites.push_back(ennemiesColSpriteVec);

		if(maps[i]->width > maxWidth)maxWidth = maps[i]->width;
		if(maps[i]->heigth > maxHeight)maxHeight = maps[i]->heigth;
	}

	std::cerr<<maxWidth<<" "<<maxHeight<<std::endl;
	backgroundTexture.setRepeated(true);
	sf::Sprite bgSprite(backgroundTexture, sf::IntRect(0,0,maxWidth*TILE_SIZE,maxHeight*TILE_SIZE));

	sf::View view(sf::FloatRect(0.0f,0.f, 1280.f/1.5, 720.f/1.5));

	bool loadTexture = true;

	/*Loading 1st level */
	loadLevel(loadTexture, currentLevel,collisionTiles,levelsSprites,topCollisionTiles, levelsTopSprites, player,maps[currentLevel]->start,enemies,maps[currentLevel]->enemiesPos,enemiesCollisionTiles,levelsEnemiesCollSprites,view,rings,maps[currentLevel]->ringPos);

	/*Gameloop*/
	bool upFlag,downFlag,leftFlag,rightFlag;
	upFlag = downFlag = leftFlag = rightFlag = false;

	sf::Clock clock;

	MyClock myclock(60.0f);
	float deltaTime;
	float averageTime = 0;

	sf::Text fpsCountText;
	fpsCountText.setFont(font);
	fpsCountText.setCharacterSize(24);
	fpsCountText.setFillColor(sf::Color::White);
	fpsCountText.setString("fps");
	fpsCountText.setPosition(window.getSize().x -100,0);

	sf::Text ringCountText;
	ringCountText.setFont(font);
	ringCountText.setCharacterSize(24);
	ringCountText.setFillColor(sf::Color::White);
	ringCountText.setString("__ rings");
	ringCountText.setPosition(50,55);

	sf::Text speedText;
	speedText.setFont(font);
	speedText.setCharacterSize(24);
	speedText.setFillColor(sf::Color::White);
	speedText.setString("__ mph");

	//FIXME issue with speed and framerate
	window.setFramerateLimit(60);

	while(window.isOpen()){

        deltaTime = clock.restart().asSeconds();
		myclock.add(deltaTime);
		averageTime = myclock.avg();

        float fps = 1.f / averageTime;
		fpsCountText.setString("fps : " + std::to_string((int)fps));
		/*Inputs*/
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				window.close();
			}else if (event.type == sf::Event::KeyPressed){
				if(event.key.code == sf::Keyboard::Left) leftFlag = true;
				else if (event.key.code == sf::Keyboard::Right) rightFlag = true;
				else if (event.key.code == sf::Keyboard::Up) upFlag = true;
				else if (event.key.code == sf::Keyboard::Z) upFlag = true;
				else if (event.key.code == sf::Keyboard::Down) downFlag = true;
				else if (event.key.code == sf::Keyboard::R) loadLevel(loadTexture, currentLevel,collisionTiles,levelsSprites,topCollisionTiles, levelsTopSprites, player,maps[currentLevel]->start,enemies,maps[currentLevel]->enemiesPos,enemiesCollisionTiles,levelsEnemiesCollSprites,view,rings,maps[currentLevel]->ringPos);
			}else if (event.type == sf::Event::KeyReleased){
				if(event.key.code == sf::Keyboard::Left) leftFlag = false;
				else if (event.key.code == sf::Keyboard::Right) rightFlag = false;
				else if (event.key.code == sf::Keyboard::Up) upFlag = false;
				else if (event.key.code == sf::Keyboard::Z) upFlag = false;
				else if (event.key.code == sf::Keyboard::Down) downFlag = false;
			}
		}
		
		sf::FloatRect *movementRect = nullptr;
		sf::FloatRect *currentRect = nullptr;
		sf::FloatRect *lastRect = nullptr;

		/*Game logics*/
		player.updateIframe(averageTime);

		/** Calculating move vector **/
		player.resetForces();
		player.setKeyboardForces(averageTime, upFlag,downFlag,leftFlag,rightFlag);
		player.setFrictionForces(averageTime);
		player.setVelocityInMoveX();
		player.setGravityForces(averageTime);

		/** Adjust vector to handle wall collision **/
		// std::cerr<<player.currentSprite.getGlobalBounds().left << " > "<<futurRect.left<<std::endl;
		// std::cerr<<player.currentSprite.getGlobalBounds().top << " > "<<futurRect.top<<std::endl;
		//Init with ridiculous huge value in order to perform < test afterwards
		float collisionReadjustX = BIG_FLOAT , collisionReadjustY= BIG_FLOAT;
		int dirMulX = 1, dirMulY = 1;
		//std::cerr<<"Vector move before : "<<player.moveX<<", "<<player.moveY<<std::endl;

		//X-Collision
		collisionReadjustX = BIG_FLOAT;
		for(int i=0; i < collisionTiles.size();++i){
			sf::FloatRect futurRect = player.currentSprite.getGlobalBounds();
			futurRect.left += player.moveX;
			//Move the player up a bit to avoid unintended collision
			futurRect.top -= 1;
			movementRect = getContainingRect(player.currentSprite.getGlobalBounds(),futurRect);
			if(movementRect->intersects(collisionTiles[i]->getGlobalBounds())){
				sf::FloatRect intersection;
				movementRect->intersects(collisionTiles[i]->getGlobalBounds(),intersection);
				
				float xOffset = intersection.width;
				if(movementRect->left < collisionTiles[i]->getGlobalBounds().left){
					dirMulX = -1;
				}
				// std::cerr<<"X gap to hug wall : "<<xOffset * dirMulX <<std::endl;
				//If the tile is closer (movementX less than previous, store it)
				if(collisionReadjustX > xOffset)collisionReadjustX = xOffset;
			}
		}
		if(collisionReadjustX < BIG_FLOAT){
			player.moveX += collisionReadjustX * dirMulX;
			//FIXME Without this player slowly embeds into wall, find a fix
			player.velocity = 0;
		}

		//Y-Collision
		collisionReadjustY= BIG_FLOAT;
		// Predict movement
		sf::FloatRect futurRect = player.currentSprite.getGlobalBounds();
		futurRect.top += player.moveY;
		movementRect = getContainingRect(player.currentSprite.getGlobalBounds(),futurRect);
		
		/// Collision with full tiles
		for(int i = 0;i < collisionTiles.size();++i){
			if(movementRect->intersects(collisionTiles[i]->getGlobalBounds())){
				sf::FloatRect intersection;
				movementRect->intersects(collisionTiles[i]->getGlobalBounds(),intersection);

				float yOffset = intersection.height;
				if(movementRect->top < collisionTiles[i]->getGlobalBounds().top){
					dirMulY = -1;
				}
				// std::cerr<<"Y gap to hug wall : "<<yOffset<<" Dir modofier : " << dirMulY<< std::endl;
				//If the tile is closer (movementY less than previous, store it)
				if(collisionReadjustY > yOffset)collisionReadjustY = yOffset;
    		}
		}
		/// Collision with top tiles TODO
		for(int i = 0;i < levelsTopSprites[currentLevel].size();++i){
			if(movementRect->intersects(levelsTopSprites[currentLevel][i]->getGlobalBounds())){
				sf::FloatRect intersection;
				movementRect->intersects(levelsTopSprites[currentLevel][i]->getGlobalBounds(),intersection);

				float yOffset = intersection.height;
				//If top collision AND the tile is closer (movementY less than previous, store it)
				// std::cerr<<movementRect->top <<" "<< levelsTopSprites[currentLevel][i]->getGlobalBounds().top<< " >> "<< (movementRect->top < levelsTopSprites[currentLevel][i]->getGlobalBounds().top) << std::endl;
				// std::cerr<<collisionReadjustY << " "<< yOffset<<">>"<<(collisionReadjustY > yOffset)<<std::endl;
				//FIXME 2nd condition might fuck up with low FPS
				if(movementRect->top < levelsTopSprites[currentLevel][i]->getGlobalBounds().top && yOffset <= GRAVITY * (averageTime * 2) && collisionReadjustY > yOffset){
					collisionReadjustY = yOffset;
					dirMulY = -1;
				}
    		}
		}
		///Set Y adjust
		if(collisionReadjustY < BIG_FLOAT){
			// std::cerr << "Readjusting y by " << collisionReadjustY * dirMulY<<std::endl;
			player.moveY += collisionReadjustY * dirMulY;
			if(dirMulY == -1 && !player.isJumping) player.hasTouchedGround();
		}

		//std::cerr<<"Vector move after : "<<player.moveX<<", "<<player.moveY<<std::endl;

		player.doVelocityMove();

		delete movementRect;
		movementRect = getContainingRect(*player.getCurrentRect(),*player.getLastRect());
		delete currentRect;
		currentRect = player.getCurrentRect();
		delete lastRect;
		lastRect = player.getLastRect();

		/** Handling collision after move **/

		for(std::vector<Enemy *>::iterator ptr = enemies.begin(); ptr < enemies.end();++ptr){
			(*ptr) -> resetForces();
			(*ptr) -> setMovementForces(averageTime);
			(*ptr) -> setGravityForces(averageTime);
			(*ptr) -> doMove();
			for(int j = 0; j < collisionTiles.size();++j){
				if((*ptr) -> isColliding(collisionTiles[j]->getGlobalBounds())){
					(*ptr) -> handleWallCollision(collisionTiles[j]->getGlobalBounds());
				}
			}

			for(int j = 0; j < enemiesCollisionTiles.size();++j){
				if((*ptr) -> isColliding(enemiesCollisionTiles[j]->getGlobalBounds())){
					(*ptr) -> handleWallCollision(enemiesCollisionTiles[j]->getGlobalBounds());
				}
			}

			if((*ptr)->isColliding(player.currentSprite.getGlobalBounds())){
				bool destroyEnemy = player.handleEnemyCollision((*ptr)->currentSprite.getGlobalBounds());
				if(destroyEnemy)enemies.erase(ptr);
				else player.getHit();
			}
		}

		for(int i = 0; i < rings.size(); ++i){
			if(rings[i]->getGlobalBounds().intersects(player.getCurrentSprite().getGlobalBounds())){
				bool pickRing = player.handleRingCollision(rings[i]->getGlobalBounds());
				if(pickRing){
					rings.erase(rings.begin()+i);
					gameAudio.playRingPickup();
				}
			}
		}

		if(player.getCurrentSprite().getGlobalBounds().intersects(maps[currentLevel]->exit)){
			++currentLevel %= NB_LEVELS;
			loadLevel(loadTexture,currentLevel,collisionTiles,levelsSprites, topCollisionTiles, levelsTopSprites, player,maps[currentLevel]->start,enemies,maps[currentLevel]->enemiesPos,enemiesCollisionTiles,levelsEnemiesCollSprites,view,rings,maps[currentLevel]->ringPos);
		}

		if(player.currentSprite.getGlobalBounds().top > maps[currentLevel]->heigth * TILE_SIZE){
			player.fallFromMap();
		}

		/*Moving "camera" view*/

		float playerX = player.currentSprite.getGlobalBounds().left + (player.currentSprite.getGlobalBounds().width/2);
		float playerY = player.currentSprite.getGlobalBounds().top + (player.currentSprite.getGlobalBounds().height/2);
		
		float cameraX,cameraY;

		if((playerX + view.getSize().x/2) > TILE_SIZE * maps[currentLevel]->width){
			cameraX = TILE_SIZE * maps[currentLevel]->width - view.getSize().x/2;
		}else if(playerX - view.getSize().x/2 < 0){
			cameraX = view.getSize().x/2;
		}else{
			cameraX = playerX;
		}
		if((playerY + view.getSize().y/2) > TILE_SIZE * maps[currentLevel]->heigth){
			cameraY = TILE_SIZE * maps[currentLevel]->heigth - view.getSize().y/2;
		}else if(playerY - view.getSize().y/2 < 0){
			cameraY = view.getSize().y/2;
		}else{
			cameraY = playerY;
		}
		player.updateAnimation(averageTime);

		ringCountText.setString(" : " + std::to_string(player.ringCounter));
		speedText.setString("Speed : " + std::to_string((int)player.velocity * 10) + " Mph");

		if(!player.isDead)view.setCenter(cameraX,cameraY);

		/*Drawing*/
		window.setView(view);
		window.clear();
		window.draw(bgSprite);
		// std::cerr<<"I frame counter"<<player.IframeCounter<<std::endl;
		for(int i = 0; i < levelsBackgroundSprites[currentLevel].size(); ++i){
			window.draw(*levelsBackgroundSprites[currentLevel][i]);
		}
		for(int i = 0; i < levelsSprites[currentLevel].size(); ++i){
			// if(levelsSprites[currentLevel][i]){
				window.draw(*levelsSprites[currentLevel][i]);
			// }
		}
		for(int i = 0; i < levelsTopSprites[currentLevel].size(); ++i){
			window.draw(*levelsTopSprites[currentLevel][i]);
		}

		
		for(std::vector<Enemy *>::iterator ptr = enemies.begin(); ptr < enemies.end();++ptr){
			if(!(*ptr)->spriteInited){
				setSprite((*ptr)->currentSprite,enemiesSpriteSheet,sf::IntRect(6,91,42,31));
				(*ptr)->spriteInited = true;
			}
			window.draw((*ptr) -> currentSprite);
		}

		for(std::vector<sf::Sprite *>::iterator it = rings.begin();it != rings.end(); ++it){
			if(loadTexture){
				(*it)->setTexture(ringTexture);
				(*it)->setScale(.15,.15);
			}
			window.draw(**it);
		}

		if(player.IframeCounter <= I_FRAME_DUR) player.currentSprite.setColor(sf::Color(255, 255, 255, 128));
		window.draw(player.getCurrentSprite());
		player.currentSprite.setColor(sf::Color(255,255,255,255));

		/*Debug*/
		
		sf::RectangleShape rectMv(sf::Vector2f(movementRect->width,movementRect->height));
		rectMv.move(sf::Vector2f(movementRect->left,movementRect->top));
		rectMv.setFillColor(sf::Color(255,0,0,200));
		sf::RectangleShape rectCurr(sf::Vector2f(currentRect->width,currentRect->height));
		rectCurr.move(sf::Vector2f(currentRect->left,currentRect->top));
		rectCurr.setFillColor(sf::Color(0,255,0,50));
		sf::RectangleShape rectOld(sf::Vector2f(lastRect->width,lastRect->height));
		rectOld.move(sf::Vector2f(lastRect->left,lastRect->top));
		rectOld.setFillColor(sf::Color(0,0,255,50));
		window.draw(rectMv);
		window.draw(rectCurr);
		window.draw(rectOld);

		sf::RectangleShape exitRect(sf::Vector2f(maps[currentLevel]->exit.width, maps[currentLevel]->exit.height));
		exitRect.move(sf::Vector2f(maps[currentLevel]->exit.left, maps[currentLevel]->exit.top));
		exitRect.setFillColor(sf::Color(100,100,100,200));
		window.draw(exitRect);
		
		/*End Debug*/


		loadTexture = false;
		window.setView(window.getDefaultView());
		window.draw(fpsCountText);
		window.draw(ringSpriteHUD);
		window.draw(ringCountText);
		window.draw(speedText);
		window.display();
	}

	/*End, free memory*/

	return 0;
}