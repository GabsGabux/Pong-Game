#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <sstream>

int main()
{
	
	
	float aspect = sf::VideoMode::getDesktopMode().width * 1.f / sf::VideoMode::getDesktopMode().height;
	float touchscale = sf::VideoMode::getDesktopMode().height * 1.f / 1080;
	
	const float pi = 3.14159f;	
	const int gameWidth = (int)(1080 * aspect);
    const int gameHeight = 1080;
    float ballRadius = 10.f;
    sf::Vector2f paddleSize(25, 100);
	// criando  a janela
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), "SFML Pong",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
	
	sf::RectangleShape shape;
	shape.setSize(paddleSize - sf::Vector2f(3, 3));
	shape.setFillColor(sf::Color::White);
	shape.setOrigin(paddleSize / 2.f);
	shape.setPosition(10 + paddleSize.x / 2, gameHeight / 2);
	
	sf::RectangleShape shape2;
	shape2.setSize(paddleSize - sf::Vector2f(3, 3));
	shape2.setFillColor(sf::Color::White);
	shape2.setOrigin(paddleSize / 2.f);
	shape2.setPosition(gameWidth - 10 - paddleSize.x / 2, gameHeight / 2);
	
	
	sf::CircleShape ball;
    ball.setRadius(ballRadius - 1);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(ballRadius / 2, ballRadius / 2);
    
    sf::Font font;
    if (!font.loadFromFile("./Font/PixelGameFont.ttf"))
         return EXIT_FAILURE;
         
    int score = 0;
    int score2 = 0;
         
     sf::Text pause;
     pause.setFont(font);
     pause.setCharacterSize(24);
     pause.setFillColor(sf::Color::White);
     pause.setString("PONG");
     sf::FloatRect pauseRect = pause.getLocalBounds();	
	pause.setOrigin(pauseRect.left + pauseRect.width/2.0f, pauseRect.top + pauseRect.height/2.0f);
    pause.setPosition(gameWidth / 2, gameHeight / 2);
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setString("0");
    sf::FloatRect pauseRct = pause.getLocalBounds();	
	pause.setOrigin(pauseRct.left + pauseRct.width/2.0f, pauseRct.top + pauseRct.height/2.0f);
    scoreText.setPosition(280, 130);
    
    sf::Text separa;
    separa.setFont(font);
    separa.setFillColor(sf::Color::White);
    separa.setCharacterSize(20);
    separa.setString("-");
    sf::FloatRect ausRct = pause.getLocalBounds();	
	pause.setOrigin(ausRct.left + ausRct.width/2.0f, ausRct.top + ausRct.height/2.0f);
	separa.setPosition(318, 135);;
    
    
    sf::Text scoreText2;
    scoreText2.setFont(font);
    scoreText2.setCharacterSize(30);
    scoreText2.setFillColor(sf::Color::White);
    scoreText2.setString("0");
    sf::FloatRect pausRct = pause.getLocalBounds();	
	pause.setOrigin(pausRct.left + pausRct.width/2.0f, pausRct.top + pausRct.height/2.0f);
    scoreText2.setPosition(350, 130);
	
	sf::Clock AITimer;
    const sf::Time AITime   = sf::seconds(0.1f);
    const float paddleSpeed = 900.f;
    float shape2Speed  = 0.f;
    const float ballSpeed   = 600.f;
    float ballAngle         = 0.f; // pode ser mudado

	
	sf::Clock clock;
	bool isPlaying = false;
	while (window.isOpen())
	{
		
		sf::Event event;
		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
			    window.close();
		}
		if (event.type == sf::Event::TouchBegan)
        {
              if (!isPlaying)
              {
                    // recomeça do jogo
                    isPlaying = true;
                    clock.restart();
                    
                    
                    ball.setPosition(gameWidth / 2, gameHeight / 2);
                    
                    do
                    {
                        // Make sure the ball initial angle is not too much vertical
                        ballAngle = (std::rand() % 360) * 2 * pi / 360;
                     }
                     while (std::abs(std::cos(ballAngle)) < 0.7f);
              }
        }
        //quando o jogo começar
        if (isPlaying)
        {

         	float deltaTime = clock.restart().asSeconds();
         	int touchdir = 0;
         	float maxmove = 0;
         	if (sf::Touch::isDown(0))
         	{
				sf::Vector2i relativePos = sf::Touch::getPosition(0, window);
				if (relativePos.y / touchscale > shape.getPosition().y)
					touchdir = 1;
				else
					touchdir = -1;							
				maxmove = relativePos.y / touchscale - shape.getPosition().y;
        	}
            if ((touchdir == -1) &&
                  (shape.getPosition().y - paddleSize.y / 2 > 5.f))
            {
				if (maxmove > -paddleSpeed * deltaTime)
					shape.move(0.f, maxmove);
				else
					shape.move(0.f, -paddleSpeed * deltaTime);
            }
            if (((shape2Speed < 0.f) && (shape2.getPosition().y - paddleSize.y / 2 > 5.f)) ||
                ((shape2Speed > 0.f) && (shape2.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f)))
            {
                shape2.move(0.f, shape2Speed * deltaTime);
            }

            // Update the computer's paddle direction according to the ball position
            if (AITimer.getElapsedTime() > AITime)
            {
                AITimer.restart();
                if (ball.getPosition().y + ballRadius > shape2.getPosition().y + paddleSize.y / 2)
                    shape2Speed = paddleSpeed;
                else if (ball.getPosition().y - ballRadius < shape2.getPosition().y - paddleSize.y / 2)
                    shape2Speed = -paddleSpeed;
                else
                    shape2Speed = 0.f;
            }
            if ((touchdir == 1) &&
               (shape.getPosition().y + paddleSize.y / 2 < gameHeight - 5.f))
            {
				if (maxmove < paddleSpeed * deltaTime)
					shape.move(0.f, maxmove);
				else
					shape.move(0.f, paddleSpeed * deltaTime);
             }
             float factor = ballSpeed * deltaTime;
             ball.move(std::cos(ballAngle) * factor, std::sin(ballAngle) * factor);
             
             if (ball.getPosition().x - ballRadius < 0.f)
            {
            	score = score + 1;
            	std::stringstream s;
            	s << score;
            	scoreText.setString(s.str());
            	
            	isPlaying = false;
            }
            if (ball.getPosition().x + ballRadius > gameWidth)
            {
            	score2 = score2 + 1;
            	std::stringstream s;
            	s << score2;
            	scoreText2.setString(s.str());
                
                isPlaying = false;
            }
            if (ball.getPosition().y - ballRadius < 0.f)
            {
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, ballRadius + 0.1f);
            }
            if (ball.getPosition().y + ballRadius > gameHeight)
            {
                ballAngle = -ballAngle;
                ball.setPosition(ball.getPosition().x, gameHeight - ballRadius - 0.1f);
            }
             // Check the collisions between the ball and the paddles
            // Left Paddle
            if (ball.getPosition().x - ballRadius < shape.getPosition().x + paddleSize.x / 2 &&
                ball.getPosition().x - ballRadius > shape.getPosition().x &&
                ball.getPosition().y + ballRadius >= shape.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= shape.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > shape.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

      //          ballSound.play();
                ball.setPosition(shape.getPosition().x + ballRadius + paddleSize.x / 2 + 0.1f, ball.getPosition().y);
            }

            // Right Paddle
            if (ball.getPosition().x + ballRadius > shape2.getPosition().x - paddleSize.x / 2 &&
                ball.getPosition().x + ballRadius < shape2.getPosition().x &&
                ball.getPosition().y + ballRadius >= shape2.getPosition().y - paddleSize.y / 2 &&
                ball.getPosition().y - ballRadius <= shape2.getPosition().y + paddleSize.y / 2)
            {
                if (ball.getPosition().y > shape2.getPosition().y)
                    ballAngle = pi - ballAngle + (std::rand() % 20) * pi / 180;
                else
                    ballAngle = pi - ballAngle - (std::rand() % 20) * pi / 180;

 //               ballSound.play();
                ball.setPosition(shape2.getPosition().x - ballRadius - paddleSize.x / 2 - 0.1f, ball.getPosition().y);
            }
    
            
        }
            
		
		window.clear();
		
		window.draw(shape);
		window.draw(shape2);
		window.draw(ball);
		window.draw(pause);
		window.draw(scoreText);
		window.draw(scoreText2);
		window.draw(separa);
		
		window.display();
	}
	
	return 0;
}