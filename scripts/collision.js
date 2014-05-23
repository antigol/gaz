App.dimension(20, 20, 40);

var n = 2500;
for (i = 0; i < n; ++i) {
	var r = App.rand(0.5, 1);
	var m = Math.pow(r, 3);
	App.radius(r);
	App.mass(m);
	
	x = App.rand(-2,2);
	y = App.rand(-2,2);
	z = App.rand(-2,2);
	App.position(x, y, z);
	App.color(x,y,z);
	App.momentum(0,0,0)

	App.addParticle();
}

App.color(1, 0, 0);

App.radius(2);
App.mass(8);

var p = 200;

App.position(-100, 0, 0);
App.momentum(p,0,0)
App.addParticle();

App.position(100, 0, 0);
App.momentum(-p,0,0)
App.addParticle();

App.position(0, -100, 0);
App.momentum(0, p,0)
App.addParticle();

App.position(0, 100, 0);
App.momentum(0,-p,0)
App.addParticle();

