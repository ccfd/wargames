#include "LiveTank.h"
#include "App.h"
#include "Bullet.h"
#include "stdlib.h"
#include "TankControl.h"


LiveTank::LiveTank (Player *player_, double x_, double y_, double rb_, double rh_) : player(player_), x(x_), y(y_), rb(rb_), rh(rh_), t(0) {
		rc = 0;
		gunAmmunition = 1000;
		cannonAmmunition = 10;
		lastCannonShot = 1;
		lastGunShot = 1;
	}

LiveTank::~LiveTank() {};

	void LiveTank::Draw(sf::RenderWindow* window) {
		avatar.Draw(window);
	};
	void LiveTank::Tick(App* app) {
		t += dt;
		TankControl control;
		control.azimuth = rh;
		control.cannonAmmunition = cannonAmmunition;
		control.gunAmmunition = gunAmmunition;
		control.cannonAngle = rc;
		bool canShootCannon = (cannonAmmunition > 0) && (t - lastCannonShot > cannonInterval);
		control.canShootCannon = canShootCannon;
		bool canShootGun = (gunAmmunition > 0) && (t - lastGunShot > gunInterval);
		control.canShootGun = canShootGun;
		control.headAngle = rh - rb;

		// Control
		player->Play(t, &control);

		double v1=0, v2=0, omh = 1;
		v1 = control.left;
		if (v1 >  25) v1 =  25;
		if (v1 < -25) v1 = -25;
		v2 = control.right;
		if (v2 >  25) v2 =  25;
		if (v2 < -25) v2 = -25;
		omh = control.turn;
		if (omh >  1) omh =  1;
		if (omh < -1) omh = -1;

		double v = (v1+v2)/2;
		double om = (v1-v2)/24;
		x +=  sin(rb)*v*dt;
		y += -cos(rb)*v*dt;
		rb += om*dt;
		rh += (om+omh)*dt;
		v = fabs(v1);
		if (v < fabs(v2)) v = fabs(v2);
		avatar.setPosition(x,y,rb,rh,v);
		
		if (canShootCannon && control.wannaShootCannon) {
			Shoot(app, 2,0.5,20);
			cannonAmmunition--;
			lastCannonShot = t;
		} else if (canShootGun && control.wannaShootGun) {
			Shoot(app, 1.3, 1.0,200);
			gunAmmunition--;
			lastGunShot = t;
		}
	};
		void LiveTank::Shoot(App* app, double r, double pitch, double vb) {
			double l = 25;
			avatar.Shoot(pitch);
			app->AddObject(new Bullet(x+l*sin(rh),y-l*cos(rh),vb*sin(rh),-vb*cos(rh),r));
		};
	Polygon LiveTank::Extent() {
		Polygon poly;
		double w=18,h=15;
		poly.push_back(Point(x+w*sin(rb)+h*cos(rb),y-w*cos(rb)+h*sin(rb)));
		poly.push_back(Point(x+w*sin(rb)-h*cos(rb),y-w*cos(rb)-h*sin(rb)));
		poly.push_back(Point(x-w*sin(rb)-h*cos(rb),y+w*cos(rb)-h*sin(rb)));
		poly.push_back(Point(x-w*sin(rb)+h*cos(rb),y+w*cos(rb)+h*sin(rb)));
		return poly;
	};

