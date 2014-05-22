App.dimension(100, 100, 100);

App.radius(1);
App.mass(1);

n = 10000;
for (i = 0; i < n; ++i) {
	App.color(App.rand(0,1), App.rand(0,1), App.rand(0,1));

	App.position(App.rand(-20,20), App.rand(-20,20), App.rand(-20,20));

	var phi = App.rand(0, 2 * Math.PI);
	var vel = App.rand(0, 30);
	var z = App.rand(-vel, vel);
	App.momentum(Math.cos(phi) * Math.sqrt(Math.pow(vel,2) - Math.pow(z,2)), 
				 Math.sin(phi) * Math.sqrt(Math.pow(vel,2) - Math.pow(z,2)),
				 z);

	App.addParticle();
}
