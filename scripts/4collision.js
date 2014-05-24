App.dimension(10, 10, 3);

App.radius(1);
App.mass(1);

p = 20;
a = 5;

App.color(1,0,0);
App.position(-a,0,0);
App.momentum(p,0,0);
App.addParticle();

App.color(1,1,0);
App.position(a,0,0);
App.momentum(-p,0,0);
App.addParticle();


App.color(1,1,1);
App.position(0,a,0);
App.momentum(0,-p,0);
App.addParticle();

App.color(0,0,1);
App.position(0,-a,0);
App.momentum(0,p,0);
App.addParticle();
