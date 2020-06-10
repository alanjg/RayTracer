#include "pch.h"
#include "Options.h"
#include "RayTracer.h"
#include "Test.h"

int main()
{
	std::chrono::system_clock::time_point t1 = std::chrono::system_clock::now();
	RayTracer rt;
	Test test(rt);

	global_options.max_ray_depth = 10;
	//global_options.use_distribution_tracing = true;
	//global_options.use_photon_mapping = true;
	global_options.photon_map_max_sample_distance = 20;
	//global_options.use_photon_map_approximation = true;
	//global_options.debug_caustics_photon_map = true;
	//global_options.use_caustics = true;
	global_options.num_samples = 1;
	global_options.global_photon_map_photon_count = 500000;
	global_options.global_photon_map_sample_count = 500;
	global_options.caustics_photon_map_photon_count = 50000;
	global_options.caustics_photon_map_sample_count = 60;
#ifndef _DEBUG
	global_options.num_threads = 8;
#endif // !DEBUG

	//rt.LoadScene("g:/code/raytracer/Scenes/cornell_water.txt");
	test.LoadScene8();
	//rt.RenderToFile("test.png", 2560, 1440);
	rt.RenderToFile("test.png", 400, 400);
	//rt.RenderToFile("test.png", 800, 800);

	std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();

	std::chrono::duration<double> diff = t2 - t1;
	std::cout << "Time elapsed: " << diff.count() << std::endl;

}