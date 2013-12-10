function geo= new_geometry()
global c;      % sound celerity

geo.beacon.f_base = 39250 ;
geo.beacon.df=500;

geo.beacon.n=1;
geo.beacon.v{geo.beacon.n}=[0;0;2];
geo.beacon.f{geo.beacon.n}=geo.beacon.f_base;
geo.beacon.name{geo.beacon.n}='SO';


geo.beacon.n=geo.beacon.n+1;
geo.beacon.v{geo.beacon.n}=[5;0;2];
geo.beacon.f{geo.beacon.n}= geo.beacon.f{geo.beacon.n-1}+ geo.beacon.df;
geo.beacon.name{geo.beacon.n}='SE';

geo.beacon.n=geo.beacon.n+1;
geo.beacon.v{geo.beacon.n}=[0;10;2];
geo.beacon.f{geo.beacon.n}= geo.beacon.f{geo.beacon.n-1}+ geo.beacon.df;
geo.beacon.name{geo.beacon.n}='NO';

geo.beacon.n=geo.beacon.n+1;
geo.beacon.v{geo.beacon.n}=[5;10;2];
geo.beacon.f{geo.beacon.n}= geo.beacon.f{geo.beacon.n-1}+ geo.beacon.df;
geo.beacon.name{geo.beacon.n}='NE';

v_min = geo.beacon.v{1};
v_max = geo.beacon.v{1};
v_min(3)=0;

for k=1:geo.beacon.n
   v_min = min(v_min,geo.beacon.v{k});
   v_max = max(v_min,geo.beacon.v{k});
end

geo.res = 0.1 ; % résolution de 10 cm
geo.grid.x= v_min(1) : geo.res : v_max(1) ;
geo.grid.y= v_min(2) : geo.res : v_max(2) ;
geo.grid.z= v_min(3) : geo.res : v_max(3) ;

geo.grid.nx=length(geo.grid.x);
geo.grid.ny=length(geo.grid.y);
geo.grid.nz=length(geo.grid.z);

geo.grid.v_min = v_min;
geo.grid.v_max = v_max;
geo.d_max = sqrt(sum((v_max-v_min).^2));
geo.delay_spread = geo.d_max/c;

rpos=rand(3,1);
geo.target.v = (rpos.*v_min+(1-rpos).*v_max)/2;
