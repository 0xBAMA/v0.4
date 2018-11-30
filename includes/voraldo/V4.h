#include "../CImg.h"		//single-header library for image display and saving
//License below


#include "../json.hpp"  	//single-header library for output file formatting
//MIT License


#include "../linalg.h"      //single-header library for linear algebra (vectors)
//Public Domain License

//Cimg license - from Cimg.eu - CImg is a free, open-source library distributed
//under the CeCILL-C (close to the GNU LGPL) or CeCILL (compatible with the GNU GPL)
//licenses. It can be used in commercial applications.

#include <cstdlib>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>

//#include <bitset>
//not in use right now

using vec = linalg::aliases::double3;
using vec2 = linalg::aliases::double2;
using mat = linalg::aliases::double3x3;
using intvec = linalg::aliases::int3;
using veContainer = std::vector<vec>;

using std::cout;
using std::endl;


struct RGB{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

struct Vox{
	RGB color;

	int state; //right now I am visualizing this as a way to
	//associate it with a particular system in a ship or vehicle
	//potentially for highlighting or to represent a material
	//if you wanted to calculate a center of gravity.

	//Additionally - 'state' may want to be a value you keep in the save file
//---

	//unsigned char light_intensity;
	//unsigned char ao_intensity;

	unsigned char size; //drop this?

	float alpha;

	bool mask;
};

class Voraldo{
public:

	Voraldo();
	~Voraldo();

	void init_block(int x, int y, int z, bool noise_fill);


//not that important right now
	void load(){return;}
	void save(){return;}

//dimensions
	int get_x_res(){return x_res;}
	int get_y_res(){return y_res;}
	int get_z_res(){return z_res;}

//draw functions - generally will set any effected voxel to 'set'
	//except for in situations where that voxel was previously masked

//in order to mask a cell, you will use a Vox value with mask set to true

	//draw at point
	void draw_point(vec point, Vox set);

	//draw along 3 dimensional line segment
	void draw_line_segment(vec v1, vec v2, Vox set);

	//draw 3 dimensional triangle, with verticies point 0, point 1, point 2
	void draw_triangle(vec v0, vec v1, vec v2, Vox set);

	//draw sphere - centered at center_point, of radius radius
	void draw_sphere(vec center_point, double radius, Vox set);

	//draw ellipsoid - centered at center_point, radii in radii
	void draw_ellipsoid(vec center_point, vec radii, Vox set);

	//draw cylinder - bvec and tvec establish the endpoints, by center, and radius is the radius
	void draw_cylinder(vec bvec, vec tvec, double radius, Vox set);

	//draw tube - just like a cylinder, but with an inner radius that will establish thickness
	void draw_tube(vec bvec, vec tvec, double inner_radius, double outer_radius, Vox set);

	//draw blockoid - draw all voxels within the axis-aligned block defined by mindex and maxdex
	void draw_blockoid(vec mindex, vec maxdex, Vox set);

	//draw quadrilateral hexahedron - fill the space defined by 8 points - refer to docs
	void draw_quadrilateral_hexahedron(vec a, vec b, vec c, vec d, vec e, vec f, vec g, vec h, Vox set);

	//...


//array access functions

	void set_data_by_3D_index(int x, int y, int z, Vox set);
	Vox  get_data_by_3D_index(int x, int y, int z);

	int get_array_index_for_3D_index(int x, int y, int z);

	void clear_all();




//mask functions

	void unmask_all();
	void invert_mask();
	void mask_all_nonzero();

//display

	void display(std::string filename);


//holding named voxel values
	std::map<std::string, Vox> name_to_Vox_map;

//holding named colors
	std::map<std::string, RGB> name_to_RGB_map;

private:

//want to know if a point is above or below a plane? below returns true, for reasons
	bool planetest(vec plane_point, vec plane_normal, vec test_point);


//block dimensions
	int x_res;
	int y_res;
	int z_res;

//the number of voxels
	int num_cells;


//the voxels
	Vox *data;



};

class Car{
public:
	Car();
	void init(Voraldo *block);
	void draw(bool platform, double x_rot, double y_rot, double z_rot);

private:
	vec center;

	vec car_x_vec;
	vec car_y_vec;
	vec car_z_vec;

	vec RRhub;
	vec RRinner;
	vec RRouter;

	vec LRhub;
	vec LRinner;
	vec LRouter;

	vec RFhub;
	vec RFinner;
	vec RFouter;

	vec LFhub;
	vec LFinner;
	vec LFouter;

	vec Rdiff;
	vec Rdiff_Roffset;
	vec Rdiff_Loffset;
	vec Rdiff_Foffset;

	vec Fdiff;
	vec Fdiff_Roffset;
	vec Fdiff_Loffset;
	vec Fdiff_Boffset;

//display versions (d_)
	vec d_car_x_vec;
	vec d_car_y_vec;
	vec d_car_z_vec;

	vec d_RRhub;
	vec d_RRinner;
	vec d_RRouter;

	vec d_LRhub;
	vec d_LRinner;
	vec d_LRouter;

	vec d_RFhub;
	vec d_RFinner;
	vec d_RFouter;

	vec d_LFhub;
	vec d_LFinner;
	vec d_LFouter;

	vec d_Rdiff;
	vec d_Rdiff_Roffset;
	vec d_Rdiff_Loffset;
	vec d_Rdiff_Foffset;

	vec d_Fdiff;
	vec d_Fdiff_Roffset;
	vec d_Fdiff_Loffset;
	vec d_Fdiff_Boffset;

	Voraldo *V_object;

	void draw_platform();
	void draw_driveline_and_axles();
	void draw_hubs();
	void draw_rear_diff();
	void draw_front_diff();
	void draw_wheels_and_tires();
};
