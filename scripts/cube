App.dimension(20, 20, 20);

App.radius(0.45);
App.mass(1);

App.momentum(0, 0, 0);
App.g(0,0,-1);

dis = 0.1;

n = 3;
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

