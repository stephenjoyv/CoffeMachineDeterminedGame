#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
sf::Sprite draw(sf::RenderTexture& space, sf::Sprite str, sf::Sprite cu, sf::Sprite cfm, sf::Sprite cfp);
class BaseObject
{
public:
	BaseObject()
	{
		drawable = true;
	}

	virtual void draw() {
		if(drawable)(*spacepoint).draw(ObjectSprite);
	}
	virtual void setRender(sf::RenderTexture& space) {
		spacepoint = &space;
	}
	void setDrawable(bool state) {
		drawable = state;
	}

protected:
	bool drawable;
	sf::Texture ObjectTexture;
	sf::Sprite ObjectSprite;
	sf::RenderTexture* spacepoint;
	
};

class indicator:public BaseObject  {
	sf::Texture greenTexture, yellowTexture, redTexture;
public:
	indicator() {
		ObjectTexture.loadFromFile("images/indicator.png");
		greenTexture.loadFromFile("images/indicator_g.png");
		yellowTexture.loadFromFile("images/indicator_y.png");
		redTexture.loadFromFile("images/indicator_r.png");
		ObjectSprite.setPosition(835, 465);
	}
	void model(short type) {
		switch (type) {
		case 1:
		{
			ObjectSprite.setTexture(ObjectTexture);
			break;
		}
		case 2:
		{
			ObjectSprite.setTexture(greenTexture);
			break;
		}
		case 3:
		{
			ObjectSprite.setTexture(yellowTexture);
			break;

		}
		case 4:
		{
			ObjectSprite.setTexture(redTexture);
			break;
		}
		}
	}

}; 
class coffe_stream:public BaseObject {
	sf::RectangleShape stream_shape;
	int Ysize,Ycoord;
	sf::Vector2f defcords;
public:
	coffe_stream(short px, sf::Color color) {
		Ysize = 20, Ycoord = 678;
		stream_shape.setSize(sf::Vector2f(17, Ysize));
		stream_shape.setTextureRect(sf::IntRect(0, 0, 30, 200));
		stream_shape.setPosition(px, Ycoord);
		stream_shape.setFillColor(color);
		defcords.x = 17;
		defcords.y = 20;

		drawable = false;
	}
	void draw() override {
		if(drawable)spacepoint->draw(stream_shape);
	}
	void change_stream(int sy) {
		stream_shape.setSize(sf::Vector2f(stream_shape.getSize().x, stream_shape.getSize().y + sy));
	}
	void move_stream(int py) {
		stream_shape.move(0, py);
	}
	void standart() {
		stream_shape.setPosition(stream_shape.getPosition().x, Ycoord);
		stream_shape.setSize(sf::Vector2f(17,Ysize));
	}
};

class button:public BaseObject
{
	sf::Texture scTexture;
	int x, y;
public:
	button(int cx, int cy) {
		ObjectTexture.loadFromFile("images/button.png");//not pressed
		scTexture.loadFromFile("images/buttonp.png");// pressed
		x = cx, y = cy;
		ObjectSprite.setPosition(x, y);
	}
	void model(short type) {
		switch (type)
		{
		case 1: {
			ObjectSprite.setTexture(ObjectTexture);
			break;
		}
		case 2: {
			ObjectSprite.setTexture(scTexture);
			break;
		}
		}
	}
};
class cup:public BaseObject {
	sf::Texture amTexture, capTexture;
	short x, y;
public:
	cup(int px, int py) {
		x = px;
		y = py;
		ObjectTexture.loadFromFile("images/cup.png");
		amTexture.loadFromFile("images/cup_ame.png");
		capTexture.loadFromFile("images/cup_cap.png");
		model(1);
		ObjectSprite.setPosition(x, y);
	}
	void cupmove(short mx) {
		ObjectSprite.move(mx,0);
	}
	short px() {
		return ObjectSprite.getPosition().x;
	}
	void model(int type) {
		switch (type)
		{
		case 1: {
			ObjectSprite.setTexture(ObjectTexture);
			break;
		}
		case 2: {
			ObjectSprite.setTexture(amTexture);
			break;
		}
		case 3: {
			ObjectSprite.setTexture(capTexture);
			break;
		}
		}
	}
	bool give(bool& ready) {
		if (ObjectSprite.getPosition().x != (- 130)) {
			cupmove(-10);
			return false;
		}
		else{
			ready = false;
			model(1);
			cupmove(860);
			std::cout << ObjectSprite.getPosition().x << " " << ObjectSprite.getPosition().y << "\n";
			std::cout << "at pos\n";
			return true;
		}
	}
};

class coffemac:public BaseObject
{
	button left{ 830,505 }, right{875,505}; //(buttonim1, buttonim2, space)
	indicator panel;
	sf::Texture arrow_tx, circle_tx;
	sf::Sprite arrow_sp, circle_sp;
	int tmprnd,dragcount=0,l=0,r=0;
public:
	coffemac() {
		ObjectTexture.loadFromFile("images/coffemac.png");
		ObjectSprite.setTexture(ObjectTexture);
		ObjectSprite.setPosition(530, 250);

		circle_tx.loadFromFile("images/circle.png");
		circle_sp.setTexture(circle_tx);
		circle_sp.setPosition(530+92, 250+212);

		arrow_tx.loadFromFile("images/arrow.png");
		arrow_sp.setTexture(arrow_tx);
		arrow_sp.setPosition(530+138, 250+257);
		//std::cout << "left vertex " << arrow_sp.getGlobalBounds().height << " " << arrow_sp.getGlobalBounds().width<<'\n';
		arrow_sp.setOrigin(2  , 33 );
		//std::cout << "left vertex " << arrow_sp.getGlobalBounds().height << " " << arrow_sp.getGlobalBounds().width;

		
	};
	void setRender(sf::RenderTexture& space) override {
		spacepoint = &space;
		left.setRender(*spacepoint);
		right.setRender(*spacepoint);
		panel.setRender(*spacepoint);
	}
	void draw() override {
		(*spacepoint).draw(ObjectSprite);
		(*spacepoint).draw(circle_sp);
		(*spacepoint).draw(arrow_sp);
		panel.draw();
		left.draw();
		right.draw();
	}
	void touched(short order, sf::Time& timer,sf::Clock& clock,bool& timer1_on, bool& timer2_on,bool& thatcoffe) {
		//std::cout << "bound  " << mx<<" "<<my << '\n';
		short rndord = rand() % 11 + 1, tmpord;
		if (rndord == 1) {
			if (order == 1)
			{
				tmpord = 0;
			}
			else tmpord = 1;
			thatcoffe = false;
		}
		else {
			tmpord = order;
			thatcoffe = true;
		}
		
		if (tmpord == 1)
		{
			left.model(2);
			panel.model(2);
			timer1_on = true;
			clock.restart();
			return;
		}
		if (tmpord==0)
		{
			right.model(2);
			panel.model(2);
			timer2_on = true;
			clock.restart();
			return;
		}
		
	}
	bool touched(sf::Time& timer, sf::Clock& clock, bool& timer1_on, bool& timer2_on,
		std::vector<coffe_stream>& m, cup &cups) {
		 if (timer1_on) {
			 float tmp = timer.asMilliseconds();
			 if (tmp >= 1000 && tmp <= 1100) {
				 left.model(1);
				 panel.model(3);
				 if(!tmprnd) tmprnd = rand() % 5 + 1;
				 return true;
			 }
			 else if (tmprnd == 1) {
				 std::cout << "works\n";
				 dragCoffemac(cups);
				 if (dragcount == 501) {
					 tmprnd = 0;
					 dragcount = 0;
					 timer = sf::milliseconds(1101);
					 clock.restart();
				 }
				 return true;
			 }
			 else if (tmp >= 1800 && tmp <= 2200) {
				 m[0].setDrawable(true);
				 m[1].setDrawable(true);
				 //std::cout << tmp;
				 //std::cout << "isworks!!!\n";
				 m[0].change_stream(3);
				 m[1].change_stream(3);
				 rotate_arrow(3);
				 return true;
			 }
			 else if (tmp >= 2300 && tmp <= 4000) {
				 rotate_arrow(2);
				 return true;
			 }
			 else if (tmp >= 4000 && tmp <= 4500) {
				 m[0].move_stream(3);
				 m[0].change_stream(-3);
				 m[1].move_stream(3);
				 m[1].change_stream(-3);
				 if (arrow_sp.getRotation() <= 357) rotate_arrow(3);
				 return true;
			 }
			 else if (tmp >= 5000 && tmp <= 5200) {
				 m[0].setDrawable(false);
				 m[1].setDrawable(false);
				 panel.model(4);
				 cups.model(2);
				 setRotate_arrow(0);
				 return true;
			 }
			 else if (tmp >= 6000) {
				 panel.model(1);
				 timer1_on = false;
				 timer = sf::Time::Zero;
				 m[0].standart();
				 m[1].standart();
				 tmprnd = 0;
				 return true;
			 }
		}
		else if (timer2_on)
		{
			 float tmp = timer.asMilliseconds();
			 if (tmp >= 1000 && tmp <= 1100) {
				 right.model(1);
				 panel.model(3);
				 if (!tmprnd) tmprnd = rand() % 5 + 1;
				 return true;
			 }
			 else if (tmprnd == 1) {
				 std::cout << "works\n";
				 dragCoffemac(cups);
				 if (dragcount==501){
					 tmprnd = 0;
					 dragcount = 0;
					 timer = sf::milliseconds(1101);
					 clock.restart();
				 }
				 return true;
			 }
			 else if (tmp >= 1800 && tmp <= 2200) {
				 m[0].setDrawable(true);
				 m[2].setDrawable(true);
				 /*std::cout << tmp;
				 std::cout << "isworks!!!\n";*/
				 m[0].change_stream(3);
				 m[2].change_stream(3);
				 rotate_arrow(3);
				 return true;
			 }
			 else if (tmp >= 2300 && tmp <= 4000) {
				 rotate_arrow(2);
				 return true;
			 }
			 else if (tmp >= 4000 && tmp <= 4500) {
				 m[0].move_stream(3);
				 m[0].change_stream(-3);
				 m[2].move_stream(3);
				 m[2].change_stream(-3);
				 if (arrow_sp.getRotation() <= 357) rotate_arrow(3);
				 return true;
			 }
			 else if (tmp >= 5000 && tmp<=5200) {
				 m[0].setDrawable(false);
				 m[2].setDrawable(false);
				 panel.model(4);
				 cups.model(3);
				 setRotate_arrow(0);
				 return true;
			 }
			 else if (tmp >= 6000) {
				 panel.model(1);
				 timer2_on = false;
				 timer = sf::Time::Zero;
				 m[0].standart();
				 m[2].standart();
				 tmprnd = 0;
				 return true;
			 }
		}
		
		return false;
	}
	void rotate_arrow(float angle) {
		arrow_sp.rotate(angle);
	}
	void setRotate_arrow(float angle) {
		arrow_sp.setRotation(angle);
	}
	void moveM(short x,short y) {
		ObjectSprite.move(x, y);
		arrow_sp.move(x, y);
		circle_sp.move(x, y);
	}
	void dragCoffemac(cup& t) {
		if (tmprnd==1 && dragcount<=500)
		{
			std::cout << "yes\n";
			if (dragcount % 2 == 0)
			{
				if (dragcount==0)
				{
					panel.setDrawable(false);
					left.setDrawable(false);
					right.setDrawable(false);
				}
				else if (dragcount == 500) {
					std::cout << "istrue\n";
					panel.setDrawable(true);
					left.setDrawable(true);
					right.setDrawable(true);
					moveM(-12, 0);
					t.cupmove(-12);
				}
				moveM(12, 0);
				t.cupmove(12);
				dragcount++;
				l++;
			}
			else {
					t.cupmove(-12);
					moveM(-12, 0);
				dragcount++;
				r++;
				
			}
			std::cout << l << " " << r << "\n";
		}
		
	}
};
class coffe_pocket: public BaseObject {
	sf::Texture gr_tx;
	sf::Sprite gr_sp;
	bool grdrawable;
	short grposX[5] = { 0,0,0,0,0 }, grposY[5] = { 0,0,0,0,0 };
	float grangle[5] = { -90,-90,-90,-90,-90 };
public:
	coffe_pocket() {
		ObjectTexture.loadFromFile("images/coffepocket.png");
		gr_tx.loadFromFile("images/grains1.png");
		ObjectSprite.setTexture(ObjectTexture);
		gr_sp.setTexture(gr_tx);
		gr_sp.setPosition(950, 260);
		gr_sp.setScale(0.7, 0.9);
		gr_sp.rotate(-90);
		ObjectSprite.setPosition(1125, 525);
		grdrawable = false;
	}
	bool touched(int mx, int my, sf::Time& timer, sf::Clock& clock, bool& move_on) {
		//std::cout << "bound  " << mx << " " << my << '\n';
		//std::cout << "globalbounds" << pocket_sp.getGlobalBounds().left << " " << pocket_sp.getGlobalBounds().top << '\n';
		if (ObjectSprite.getGlobalBounds().contains(mx, my) && !move_on)
		{
			move_on = true;
			clock.restart();
			return true;
		}
		int state = timer.asMilliseconds();
		if (move_on && state <= 1200)
		{
			ObjectSprite.move(0, -3);
			std::cout << drawable << "\n";
			return true;
		}
		else if (move_on && state <= 2100)
		{
			ObjectSprite.move(-4, 0);
			return true;
		}
		else if (move_on && state <= 3500)
		{
			ObjectSprite.rotate(-1);
			return true;
		}
		else if (move_on && state <= 4000)
		{
			grdrawable = true;
			gr_sp.rotate(-3);
			gr_sp.move(0, 1.5);
			int tx = gr_sp.getPosition().x, ty = gr_sp.getPosition().y,posxtmp = tx,posytmp=ty;
			float angle = gr_sp.getRotation(),angletmp = angle;
			for (int i = 0; i < 4; i++)
			{
				if (i % 2 == 0)
				{
					angletmp = angletmp - 1 * i;
					grangle[i] = angletmp;
				}
				else
				{
					angletmp = angletmp + i;
					grangle[i] = angletmp;
				}
				posxtmp -= 8;
				grposX[i] = posxtmp;
				posytmp += 16;
				grposY[i] = posytmp;
			}
			grposX[4] = tx; grposY[4] = ty;
			grangle[4] = angle;
			return true;
		}
		else if (move_on && state <= 4400)
		{
			gr_sp.move(0.000004, 2.99999f);
			int tx = gr_sp.getPosition().x, ty = gr_sp.getPosition().y,posYtmp=ty;
			int angle = gr_sp.getRotation();
			for (int i = 0; i < 4; i++)
			{
				grangle[i] = angle;
				grposX[i] = tx;
				posYtmp += 4;
				grposY[i] = posYtmp;
			}
			grposX[4] = tx;
			grposY[4] = ty;
			return true;
		}
		else if (move_on && state <= 4500) {
			grdrawable = false;
			gr_sp.setPosition(950, 260);
			gr_sp.setRotation(-90);
			return true;
		}
		else if (move_on && state <= 5900)
		{
			ObjectSprite.rotate(1);
			return true;
		}
		else if (move_on && state <= 6800)
		{
			ObjectSprite.move(4, 0);
			return true;
			}
		else if (move_on && state <= 8000) {
			ObjectSprite.move(0, 3);
			return true;
		}

		move_on =  false;

		return false;
	}
	void draw() override {
		if (drawable)
		{
			spacepoint->draw(ObjectSprite);
			if (grdrawable)
			{
				for (int i = 0; i < 5; i++)
				{
					gr_sp.setPosition(grposX[i], grposY[i]);
					gr_sp.setRotation(grangle[i]);
					spacepoint->draw(gr_sp);
				}
			}

		}	
	}
};
sf::Sprite draw(sf::RenderTexture& space, std::vector<coffe_stream> str,cup cu, coffemac cfm, coffe_pocket cfp,sf::Sprite bgt) {
	space.clear(sf::Color::White);
	//std::cout << "works\n";
	
	space.draw(bgt);
	cfp.draw();
	cfm.draw();
	for (int i = 0; i < 3; i++)
	{
		str[i].draw();
	}
	cu.draw();
	space.display();
	sf::Sprite sprite;
	sprite.setTexture(space.getTexture());
	return sprite;
}