# RayTracer
 
This is a ray tracer with support for global illumination using photon mapping and path tracing.

Objects are defined with the IObject interface, and stored in an OctTree for intersection acceleration.

The core ray tracing functionality can be found in the RayTracer::Trace method.