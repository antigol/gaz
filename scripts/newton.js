App.dimension(20, 1.1, 1.1);

App.radius(1);
App.mass(1);

App.momentum(15, 0, 0);

var c = 1;
for (x = -15; x <= 15; x += 5) {
	App.position(x, 0, 0);
	
	App.color(c%2, c%3, c%5); ++c;
	
	App.addParticule();

	App.momentum(0, 0, 0);
}
