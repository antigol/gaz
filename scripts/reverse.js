App.dimension(10, 10, 10);

App.radius(0.2);
App.mass(1);

App.momentum(0, 0, 10);

dis = 0.001;

n = 1;
for (x = -n; x <= n; x++) {
for (y = -n; y <= n; y++) {
for (z = -n; z <= n; z++) {
	h = (x + y + z + 3*n) / (6*n) * 360;
	App.colorHSV(h,1,1);
		
	App.position(
		x + App.rand(-dis, dis), 
		y + App.rand(-dis, dis), 
		z);
	App.addParticle();
}
}
}

