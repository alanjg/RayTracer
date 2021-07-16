#pragma once

enum class DebugOutputLevel { DEBUG_OUTPUT_NONE, DEBUG_OUTPUT_PROGRESS, DEBUG_OUTPUT_DIAGNOSTIC, DEBUG_OUTPUT_WARNINGS };

struct Options
{
	// number of threads to use
	int num_threads;

	// terminate recursive ray tracing at this depth and return Black
	int max_ray_depth;

	// use distribution tracing(trace diffuse rays)
	bool use_distribution_tracing;

	// Build and use the global photon map
	bool use_photon_mapping;

	// Number of photons to use when populating the global photon map
	int global_photon_map_photon_count;

	// Number of photons to use when sampling from the global photon map
	int global_photon_map_sample_count;

	// Maximum distance to sample photon map
	double photon_map_max_sample_distance;

	// Use the photon map to evaluate all rays
	bool use_photon_map_approximation;

	// Use the caustics photon map to evaluate all rays
	bool use_caustics_photon_map_approximation;

	// Build and use the photon map for caustics
	bool use_caustics;

	// Number of photons to use when populating the global photon map
	int caustics_photon_map_photon_count;

	// Number of photons to use when sampling from the caustics photon map
	int caustics_photon_map_sample_count;

	// Debug visualization of the caustics photon map
	bool debug_caustics_photon_map;

	// Number of rays to sample at each pixel
	int num_samples;

	// Maximum number of objects allowed per octtree node
	unsigned int max_objects_per_octtree_node;

	DebugOutputLevel debugOutputLevel;

	Options()
	{
		// Default values
		num_threads = 1;
		max_ray_depth = 10;
		use_distribution_tracing = false;
		use_photon_mapping = false;
		global_photon_map_photon_count = 100000;
		global_photon_map_sample_count = 500;
		use_photon_map_approximation = false;
		use_caustics_photon_map_approximation = false;
		use_caustics = false;
		caustics_photon_map_photon_count = 20000;
		caustics_photon_map_sample_count = 20;
		debug_caustics_photon_map = false;
		photon_map_max_sample_distance = 0.3;
		num_samples = 100;
		max_objects_per_octtree_node = 10;
		debugOutputLevel = DebugOutputLevel::DEBUG_OUTPUT_PROGRESS;
	}
};

extern Options global_options;