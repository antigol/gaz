App.dimension(40, 40, 100);

for (i = 0; i < 1500; ++i) {
	var r = App.rand(0.5, 1.4);
	var m = Math.pow(r, 3);
	App.radius(r);
	App.mass(m);
	
	x = App.rand(-40,40);
	y = App.rand(-40,40);
	App.position(x, y, 0);

	var phi = App.randn(0, 2.0 * Math.PI);
	var vel = App.rand(0, 3);
	App.momentum(Math.cos(phi) * vel, Math.sin(phi) * vel, 0);

	if (x > 0) {
		App.color(1,0,0);
	} else {
		App.color(0,1,0);
	}

	App.addParticle();
}

