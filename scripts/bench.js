dim = 100;
App.dimension(dim, dim, dim);

App.radius(1);
App.mass(1);

var n = 10000;

for (i = 0; i < n; ++i) {
	App.color(App.rand(0,1), App.rand(0,1), App.rand(0,1));

	App.position(
		App.rand(-dim,dim), 
		App.rand(-dim,dim),
		App.rand(-dim,dim));

	var phi = App.rand(0, 2 * Math.PI);
	var vel = App.rand(0, 30);
	var z = App.rand(-vel, vel);
	var xy = Math.sqrt(Math.pow(vel,2) - Math.pow(z,2));
	App.momentum(
		Math.cos(phi) * xy,
		Math.sin(phi) * xy,
		z);

	App.addParticle();
}
