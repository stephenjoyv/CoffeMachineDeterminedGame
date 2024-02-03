#include "Header.h"
int main()
{
    setlocale(LC_ALL, "rus");
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(1920, 950), "SFML works!");
    sf::RenderTexture workspace;
    coffemac l;
    cup t{ 730,750 };
    std::vector <coffe_stream> cfs = { {765,sf::Color(68, 45, 37)} ,{ 819,sf::Color(221, 238, 255)},
        { 819,sf::Color(255, 255, 255) } };
    coffe_pocket p;
    sf::Sprite frontft,bgs;
    sf::Texture bgt;
    sf::Clock clock, pocket_clock, randclock;
    sf::Time timer, pocket_time, randtime;
    sf::Font font;
    sf::RectangleShape kvadrat;
    sf::Text text;
    sf::Color bg = sf::Color(134, 216, 147);
    bool timer1_on = false, timer2_on = false, move_on = false,
        action = false, waiting = true, ready = false, makecoffe = false, thatcoffe = false;
    short order, chance = 0,ordersgiven=0;
    bgt.loadFromFile("images/kitchen.jpg");
    bgs.setTexture(bgt);
    //bgs.setScale(1, 0.5);
    bgs.move(0, -700);
    workspace.create(1920, 950);
    l.setRender(workspace);
    t.setRender(workspace);
    for (int i = 0; i < 3; i++)
    {
        cfs[i].setRender(workspace);
    }
    p.setRender(workspace);

    
    window.setFramerateLimit(60);

    timer = sf::Time::Zero;
    font.loadFromFile("Montserrat-ExtraBold.ttf");
    kvadrat.setSize(sf::Vector2f(700, 50));
    kvadrat.setPosition(20, 20);
    kvadrat.setFillColor(sf::Color(0, 0, 0, 0));
    kvadrat.setOutlineThickness(5);
    kvadrat.setOutlineColor(sf::Color::Black);
    text.setFont(font);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Black);
    text.setPosition(60, 20);
    window.clear(bg);
    window.draw(draw(workspace, cfs, t, l, p, bgs));
    window.display();
   





   
    while (window.isOpen())
    {
        sf::Event event;
        sf::Mouse mouse;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (action && mouse.isButtonPressed(mouse.Left) && event.type == sf::Event::MouseButtonPressed)
            {
                
                auto pos = mouse.getPosition(window);
                
                if(!move_on && ordersgiven == 5) {
                    if (p.touched(pos.x, pos.y, pocket_time, pocket_clock, move_on)) {
                        pocket_time = sf::Time::Zero;
                        pocket_clock.restart();
                        window.clear(bg);
                        window.draw(draw(workspace, cfs, t, l, p,bgs));
                        window.draw(kvadrat);
                        window.draw(text);
                        window.display();
                    }
                }
                

            }
                     
        }
        //Приготовление и выдача заказа
        if (action) {
            if (makecoffe && ordersgiven != 5) {
                l.touched(order, timer, clock, timer1_on, timer2_on, thatcoffe);
                window.clear(bg);
                window.draw(draw(workspace, cfs, t, l, p,bgs));
                window.draw(kvadrat);
                window.draw(text);
                window.display();
                makecoffe = false;
            }
            if (timer1_on || timer2_on) {
                if (l.touched(timer, clock, timer1_on, timer2_on,cfs,t)) {
                    
                                       
                    window.clear(bg);
                    window.draw(draw(workspace, cfs, t, l, p,bgs));
                    window.draw(kvadrat);
                    window.draw(text);
                    window.display();
                }
                else {
                    ready = true;
                }
                timer += clock.getElapsedTime();
                clock.restart();
            }
            else if (ready) {
                window.clear(bg);
                if (t.give(ready)) {
                    waiting = true;
                    action = false;
                    window.draw(draw(workspace, cfs, t, l, p, bgs));
                    if (thatcoffe) text.setString(L"Спасибо!");
                    else text.setString(L"Вы сделали не тот кофе!");
                    window.draw(kvadrat);
                    window.draw(text);
                    ordersgiven++;
                    randtime = sf::Time::Zero;
                    randclock.restart();
                }
                else {
                    window.draw(draw(workspace, cfs, t, l, p, bgs));

                }
                window.display();
            }
            if (move_on) {
                if (p.touched(0, 0, pocket_time, pocket_clock, move_on)) {
                    window.clear(bg);
                    window.draw(draw(workspace, cfs, t, l, p, bgs));
                    //window.draw(kvadrat);
                    //window.draw(text);
                    window.display();
                }
                else {
                    ordersgiven = 0;
                    action = false;
                    waiting = true;
                }
                pocket_time += pocket_clock.getElapsedTime();
                pocket_clock.restart();
            }
        }
        
        //Ожидание заказа
        if (waiting) {
            if (ordersgiven == 5) {
                window.clear(bg);
                window.draw(draw(workspace, cfs, t, l, p,bgs));
                kvadrat.setPosition(20, 100);
                kvadrat.setSize(sf::Vector2f(700, 100));
                window.draw(kvadrat);
                kvadrat.setPosition(20, 20);
                kvadrat.setSize(sf::Vector2f(700, 50));
                sf::Text warning{ L"Внимание, кофе закончилось,\nзаправьте аппарат",font,40 };
                warning.setPosition(20, 100);
                warning.setFillColor(sf::Color::Red);
                window.draw(warning);
                window.display();
                waiting = false;
                action = true;
            }
            else {
                randtime += randclock.getElapsedTime();
                randclock.restart();
                if ((int)randtime.asSeconds() >= 1) {
                    chance = rand() % 5 + 1;
                    std::cout << '\n' << ((int)randtime.asMicroseconds() + 100) << " " << chance << '\n';
                    randtime = sf::Time::Zero;
                }

                if (chance == 5) {
                    order = rand() % 2;

                    window.clear(bg);
                    window.draw(draw(workspace, cfs, t, l, p,bgs));
                    window.draw(kvadrat);
                    if (order) {
                        text.setString(L"Приготовьте американо");
                    }
                    else {
                        text.setString(L"Приготовьте капучино");
                    }
                    window.draw(text);
                    chance = 0;
                    window.display();
                    makecoffe = true;
                    waiting = false;
                    action = true;
                }
            }
        }
    }
    return 0;
}