App.dimension(40, 40, 10);

// toutes blanches
App.color(1, 1, 1);

for (i = 0; i < 300; ++i) {
	var r = App.rand(0.5, 2);
	var m = Math.pow(r, 3);
	App.radius(r);
	App.mass(m);
	
	App.position(App.rand(-20,20), App.rand(-20,20), 0);

	var phi = App.rand(0, 2 * Math.PI);
	var vel = App.rand(0, 5);
	App.momentum(Math.cos(phi) * vel, Math.sin(phi) * vel, 0);

	App.addParticule();
}

// sauf une
App.color(1, 0, 0);

App.radius(0.4);
App.mass(Math.pow(0.4, 3));

App.position(0, 0, 0);

var phi = App.rand(0, 2 * Math.PI);
var vel = App.rand(0, 5);
App.momentum(Math.cos(phi) * vel, Math.sin(phi) * vel, 0);

App.addParticule();

